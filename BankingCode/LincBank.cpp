#define DEBUG true
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Accounts/Account.h"
#include "Accounts/Current.h"
#include "Accounts/Savings.h"
#include "Exceptions/IncorrectAmountOfArguments.h"
#include "Exceptions/IncorrectArgumentTypes.h"
#include "Exceptions/IncorrectArgumentValue.h"
#include "InputParser.h"
#include "Transaction.h"

void printOptions()
{
	std::cout << "options / help / ?: view these options again\n" << std::endl;
	std::cout << "open type initial_deposit: open an account\n\t{ Type -> Current (1), Savings (2) or ISA (3) }\n" << std::endl;
	std::cout << "view [index]: view balance and recent transactions\n\t{ *Index - optional, starting from 1 }\n" << std::endl;
	std::cout << "withdraw sum: withdraw funds from most recently viewed account\n" << std::endl;
	std::cout << "deposit sum: deposit funds into most recently viewed account\n" << std::endl;
	std::cout << "transfer src dest sum: transfer funds between accounts\n" << std::endl;
	std::cout << "project years: project balance forward in time\n" << std::endl;
	std::cout << "search type amount timestamp: find transaction by following parametrs\n\t{ Type -> Initial Deposit (0), Deposit (1), Withdraw (2), Transfer (3) }\n\t{ Timestamp -> HH:MM:SS DD/MM/YYYY }\n" << std::endl;
	std::cout << "exit: close this application" << std::endl;
}

int main()
{
	std::vector <std::string> parameters;
	std::string userCommand;

	std::vector <Accounts::Account*> openedAccounts;
	Accounts::Account* lastSelectedAccount = NULL;

	std::cout << "~~~ Welcome to LincBank! ~~~" << std::endl;
	printOptions();

	while (userCommand != "exit")
	{
		// clear ready for next command
		parameters.clear(); 
		std::cout << std::endl << ">>> ";

		// Extract and store input
		std::getline(std::cin, userCommand);

		// Initialize C-like string and save it
		char* cstr = new char[userCommand.length() + 1];
		strcpy(cstr, userCommand.c_str());

		// Split input string into parts
		char* token;
		token = strtok(cstr, " ");

		// Save each input parametr
		while (token != NULL)
		{
			parameters.push_back(token);
			token = strtok(NULL, " ");
		}

		std::string command = "";

		try 
		{
			// If no command - ignore
			if (parameters.empty()) continue;

			// Get main command parametr
			command = parameters[0];
			
			// Display the various commands to the user
			if (command.compare("options") == 0 || command.compare("?") == 0 || command.compare("help") == 0)
			{
				printOptions();
			}

			// Open account
			else if (command.compare("open") == 0)
			{
				// Check if correct amount of arguments passed
				if (parameters.size() != 3) throw Exceptions::IncorrectAmountOfArguments();

				// Parse values
				unsigned type = InputParser::parse<unsigned>(parameters[1]);
				double openingBalance = InputParser::parse<double>(parameters[2]);

				// Check balance
				if (openingBalance < 0) throw Exceptions::IncorrectArgumentValue("All accounts require opening balance of 0 or more!");
				
				// Check for non-negative value or incorrect amount for ISA
				if (type == 3)
				{
					// ISA account requires minimum deposit of 1000
					if (openingBalance < 1000) throw Exceptions::IncorrectArgumentValue("ISA accounts require opening balance of 1000 or more!");

					// Check if ISA is already open
					Accounts::Savings* ISA = NULL;

					for (auto account : openedAccounts) {

						// Try to downcast
						if (ISA = dynamic_cast<Accounts::Savings*>(account)) {

							// Check if savings account is ISA
							if ((*ISA).isISA()) {
								ISA = (Accounts::Savings*)account;
								break;
							}
						}
						
						ISA = NULL;
					}

					if (ISA) throw Exceptions::IncorrectArgumentValue("ISA account already opened! Only 1 ISA account is available at time!");
				}
				
				// ID of next account
				long newID = openedAccounts.size() + 1;

				// Open account and save it to lastSelected
				switch (type)
				{
					case (1):
						openedAccounts.push_back(lastSelectedAccount = new Accounts::Current(newID, openingBalance));
						std::cout << "Current account created!" << std::endl;
						break;

					case (2):
						openedAccounts.push_back(lastSelectedAccount = new Accounts::Savings(newID, openingBalance));
						std::cout << "Savings account created!" << std::endl;
						break;

					case (3):
						openedAccounts.push_back(lastSelectedAccount = new Accounts::Savings(newID, openingBalance, true));
						std::cout << "ISA account created!" << std::endl;
						break;

					default:
						throw Exceptions::IncorrectArgumentValue("Only 1-3 values are available!");
				}
			}

			// View balance and recent transactions
			else if (command.compare("view") == 0)
			{
				if (parameters.size() > 2) throw Exceptions::IncorrectAmountOfArguments();

				// Check if any accounts are open
				if (openedAccounts.size() == 0) throw Exceptions::IncorrectArgumentValue("No accounts exists!");

				// display all accounts if no index is provided
				if (parameters.size() == 1)
				{
					for (auto acc : openedAccounts) 
					{
						// Print account info
						std::cout << acc->toString() << std::endl;

						// Print last 10 transactions to avoid a lot of text
						for (auto transaction : acc->getLastTransaction(10)) {
							std::cout << transaction->toString();
						}

						std::cout << std::endl;
					}
				}

				// Display an account according to an index (starting from 1)
				else 
				{
					int index = InputParser::parse<int>(parameters[1]);

					// Check if index is negative, negative bound and if it exists
					if (index <= 0 || index-1 > index || openedAccounts.size() < index) throw Exceptions::IncorrectArgumentValue("No account with such index exists!");

					// Save last select (don't care about overflow because of previous check)
					lastSelectedAccount = openedAccounts[index-1];

					// Print data about account
					std::cout << lastSelectedAccount->toString() << std::endl;

					// Print last 10 transactions to avoid a lot of text
					for (auto transaction : lastSelectedAccount->getLastTransaction(10)) {
						std::cout << transaction->toString();
					}
				}
			}

			else if (command.compare("withdraw") == 0 || command.compare("deposit") == 0)
			{
				if (parameters.size() != 2) throw Exceptions::IncorrectAmountOfArguments();
				
				double amount = InputParser::parse<double>(parameters[1]);

				// Check if amount to deposit / withdraw is 0
				if (amount <= 0) throw Exceptions::IncorrectArgumentValue("Deposit or Withdraw amount should be more than 0!");

				// Check if any accounts are open
				if (openedAccounts.size() == 0) throw Exceptions::IncorrectArgumentValue("No account(-s) exists!");

				// If no selected account, select last
				if (lastSelectedAccount == NULL) lastSelectedAccount = openedAccounts[openedAccounts.size() - 1];

				// Withdraw funds from most recently viewed account
				if (command.compare("withdraw") == 0) {

					double possibleOverdraft = 0;

					// Check if account is Current and overdraft is possible
					if (Accounts::Current* current = dynamic_cast<Accounts::Current*>(lastSelectedAccount)) possibleOverdraft = current->getOverdraft();
					
					// Check balance
					if (amount > lastSelectedAccount->getBalance() + possibleOverdraft) throw Exceptions::IncorrectArgumentValue("Insufficient balance to perform operation!");

					// Withdraw funds from an account
					lastSelectedAccount->withdraw(amount);
				}

				// Deposit funds into most recently viewed account
				else {
					lastSelectedAccount->deposit(amount);
				}
			}

			// Transfer funds between accounts
			else if (command.compare("transfer") == 0)
			{
				if (parameters.size() != 4) throw Exceptions::IncorrectAmountOfArguments();

				int src = InputParser::parse<int>(parameters[1]);
				int dest = InputParser::parse<int>(parameters[2]);
				double sum = InputParser::parse<double>(parameters[3]);
				
				// Check if src and dest exists
				if (src <= 0 || src - 1 > src || openedAccounts.size() < src) throw Exceptions::IncorrectArgumentValue("No account with such index exists!");
				if (dest <= 0 || dest - 1 > dest || openedAccounts.size() < dest) throw Exceptions::IncorrectArgumentValue("No account with such index exists!");

				if (src == dest) throw Exceptions::IncorrectArgumentValue("Source and destination adress should not be the same!");

				// Check amount
				if (sum <= 0) throw Exceptions::IncorrectArgumentValue("Tranfer amount should more than 0!");
				
				// Get absolute index of accounts
				src--;
				dest--;

				// Check if account is Current and overdraft is possible
				double possibleOverdraft = 0;
				if (Accounts::Current* current = dynamic_cast<Accounts::Current*>(openedAccounts[src])) possibleOverdraft = current->getOverdraft();

				// Check balance
				if (sum > openedAccounts[src]->getBalance() + possibleOverdraft) throw Exceptions::IncorrectArgumentValue("Insufficient balance to perform operation!");

				// Transfer funds between accounts
				openedAccounts[src]->transfer(openedAccounts[dest], sum);
			}

			// Project balance forward in time
			else if (command.compare("project") == 0)
			{
				if (parameters.size() != 2) throw Exceptions::IncorrectAmountOfArguments();

				double amountOfYears = InputParser::parse<double>(parameters[1]);

				// Check amount
				if (amountOfYears <= 0) throw Exceptions::IncorrectArgumentValue("Project time should be more than 0!");

				// Check if last selected is not savings account
				Accounts::Savings* saving = dynamic_cast<Accounts::Savings*>(lastSelectedAccount);

				if (saving == NULL) {
					// Select last savings or ISA created account
					for (std::vector<Accounts::Account*>::reverse_iterator it = openedAccounts.rbegin(); it != openedAccounts.rend(); it++) {
						if (saving = dynamic_cast<Accounts::Savings*>(*it)) {
							break;
						}
					}
				}

				// Check if no saving account created yet
				if (saving == NULL) throw Exceptions::IncorrectArgumentTypes("No Regular Saving or ISA account(-s) were found!");

				// Calculate and print
				std::cout << (*saving).computeInterest(amountOfYears) << std::endl;
			}

			else if (command.compare("search") == 0)
			{
				if (parameters.size() != 5) throw Exceptions::IncorrectAmountOfArguments();

				int type = InputParser::parse<int>(parameters[1]);
				double amount = InputParser::parse<double>(parameters[2]);
				long long time_stamp = InputParser::parse<std::time_t>(parameters[3] + parameters[4]);

				// Check if any accounts are open
				if (openedAccounts.size() == 0) throw Exceptions::IncorrectArgumentValue("No account(-s) exists!");

				// If no selected account, select last
				if (lastSelectedAccount == NULL) lastSelectedAccount = openedAccounts[openedAccounts.size() - 1];

				// Find particular transaction
				if (Transaction* transaction = lastSelectedAccount->searchTransaction(std::to_string(time_stamp) + std::to_string(type) + std::to_string(amount))) {
					
					// Print all the data related to transaction
					std::cout << (*transaction).toString();
				}
				else {
					throw Exceptions::IncorrectArgumentTypes("No provided transaction was found!");
				}
			}

			else if (command.compare("exit") == 0)
			{
				// do nothing
			}

			else
			{
				std::cout << '"' << command << '"' << " is not recognized as an internal or external command. Type 'options' / 'help' / '?' for list of options." << std::endl;
			}
		}
		catch (Exceptions::IncorrectAmountOfArguments) {
			continue;
		}
		catch (Exceptions::IncorrectArgumentTypes) {
			continue;
		}
		catch (Exceptions::IncorrectArgumentValue) {
			continue;
		}
	}

	std::cout << "Press any key to quit...";
	std::getchar();
}