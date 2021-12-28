#define DEBUG true
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "AccountFactories/AccountFactory.h"
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
	std::cout << "options / help / ?: view these options again" << "\n\n";
	
	std::cout << "open type initial_deposit: open an account " << "\n\t"
		<< "{ Type -> Current (1), Savings (2) or ISA (3) }" << "\n\n";

	std::cout << "view [index]: view balance and recent transactions" 
		<< "\n\t" << "{ *Index - optional, starting from 1 }" << "\n\n";
	
	std::cout << "withdraw sum: withdraw funds from most recently viewed account" << "\n\n";

	std::cout << "deposit sum: deposit funds into most recently viewed account" << "\n\n";
	
	std::cout << "transfer source_index destination_index sum: transfer funds between accounts" << "\n\n";
	
	std::cout << "project years: project balance forward in time" << "\n\n";

	std::cout << "search type amount timestamp: find transaction by following parametrs" << "\n\t" 
		<< "{ Type -> Initial Deposit (0), Deposit (1), Withdraw (2), Transfer (3) }" << "\n\t"
		<< "{ Timestamp -> HH:MM:SS DD/MM/YYYY }" << "\n\n";
	
	std::cout << "exit: close this application" << "\n";
}

int main()
{
	std::vector <std::string> parameters;
	std::string userCommand;

	AccountFactories::AccountFactory accountFactory;
	Accounts::Account* lastSelectedAccount = nullptr;

	std::cout << "~~~ Welcome to LincBank! ~~~\n";
	printOptions();

	while (userCommand != "exit")
	{
		// clear ready for next command
		parameters.clear(); 
		std::cout << "\n>>> ";

		// Extract and store input
		std::getline(std::cin, userCommand);

		// Initialize C-like string and save it
		char* cstr = new char[userCommand.length() + 1];
		strcpy(cstr, userCommand.c_str());

		// Split input string into parts
		char* token;
		token = strtok(cstr, " ");

		// Save each input parametr
		while (token != nullptr)
		{
			parameters.push_back(token);
			token = strtok(nullptr, " ");
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
				const unsigned type = InputParser::parse<unsigned>(parameters[1]);
				double initialOpeningBalance = InputParser::parse<double>(parameters[2]);

				// Check if user entered more than decimal precision
				const int dotPos = parameters[2].find(".");
				if (dotPos != std::string::npos) {
					if (parameters[2].size() - dotPos > 3) {
						throw Exceptions::IncorrectArgumentValue("Please provide correct opening balance!");
					}
				}

				// Check for -0 input
				if ((initialOpeningBalance + 0.0) == 0.00) initialOpeningBalance = 0;

				// Make opening balance const for security purposes
				const double openingBalance = initialOpeningBalance;

				// Open account and save it to lastSelected
				switch (type)
				{
				case (1):
					lastSelectedAccount = accountFactory.CreateRegularAccount(openingBalance);
					std::cout << "Current account created!\n";
					break;

				case (2):
					lastSelectedAccount = accountFactory.CreateSavingsAccount(openingBalance);
					std::cout << "Savings account created!\n";
					break;

				case (3):
					lastSelectedAccount = accountFactory.CreateISAAccount(openingBalance);
					std::cout << "ISA account created!\n";
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
				if (accountFactory.GetAmountOfAccounts() == 0) throw Exceptions::IncorrectArgumentValue("No accounts exists!");

				// display all accounts if no index is provided
				if (parameters.size() == 1)
				{
					for (const auto acc : accountFactory.GetAccounts())
					{
						// Print account info
						std::cout << acc->toString() << "\n";

						// Print last 10 transactions to avoid a lot of text
						for (const Transaction* transaction : acc->getLastTransaction(10)) {
							std::cout << (*transaction).toString();
						}

						std::cout << "\n";
					}
				}

				// Display an account according to an index (starting from 1)
				else 
				{
					const int index = InputParser::parse<int>(parameters[1]);
					Accounts::Account* tmpAccount = accountFactory.GetAccountByIndex(index - 1);

					// Check if exists
					if (tmpAccount == nullptr) throw Exceptions::IncorrectArgumentValue("No account with such index exists!");

					// Update last selected account
					lastSelectedAccount = tmpAccount;

					// Print data about account
					std::cout << lastSelectedAccount->toString() << "\n";

					// Print all transactions
					for (const auto transaction : lastSelectedAccount->getAllTransaction()) {
						std::cout << transaction->toString();
					}
				}
			}

			else if (command.compare("withdraw") == 0 || command.compare("deposit") == 0)
			{
				if (parameters.size() != 2) throw Exceptions::IncorrectAmountOfArguments();
				
				const double amount = InputParser::parse<double>(parameters[1]);

				// Check if user entered more than decimal precision
				const int dotPos = parameters[1].find(".");
				if (dotPos != std::string::npos) {
					if (parameters[1].size() - dotPos > 3) {
						throw Exceptions::IncorrectArgumentValue("Please provide correct withdraw/deposit amount!");
					}
				}

				// Check if amount to deposit / withdraw is 0
				if (amount <= 0) throw Exceptions::IncorrectArgumentValue("Deposit or Withdraw amount should be more than 0!");

				// Check if any accounts are open
				if (accountFactory.GetAmountOfAccounts() == 0) throw Exceptions::IncorrectArgumentValue("No account(-s) exists!");

				// If no selected account, select last
				if (lastSelectedAccount == nullptr) lastSelectedAccount = accountFactory.GetLastCreatedAccount();

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

				const int src = InputParser::parse<int>(parameters[1]);
				const int dest = InputParser::parse<int>(parameters[2]);
				const double sum = InputParser::parse<double>(parameters[3]);

				// Check if user entered more than decimal precision
				const int dotPos = parameters[3].find(".");
				if (dotPos != std::string::npos) {
					if (parameters[3].size() - dotPos > 3) {
						throw Exceptions::IncorrectArgumentValue("Please provide correct transfering amount!");
					}
				}
				
				// Get accounts
				Accounts::Account* sourceAccount = accountFactory.GetAccountByIndex(src-1);
				Accounts::Account* destinationAccount = accountFactory.GetAccountByIndex(dest-1);

				// Check if exists
				if (sourceAccount == nullptr || destinationAccount == nullptr) throw Exceptions::IncorrectArgumentValue("No account with such index exists!");

				// Check if same account is selected
				if (src == dest) throw Exceptions::IncorrectArgumentValue("Source and destination adress should not be the same!");

				// Check amount
				if (sum <= 0) throw Exceptions::IncorrectArgumentValue("Tranfer amount should more than 0!");

				// Check if account is Current and overdraft is possible
				double possibleOverdraft = 0;
				if (Accounts::Current* current = dynamic_cast<Accounts::Current*>(sourceAccount)) possibleOverdraft = current->getOverdraft();

				// Check balance
				if (sum > sourceAccount->getBalance() + possibleOverdraft) throw Exceptions::IncorrectArgumentValue("Insufficient balance to perform operation!");

				// Transfer funds between accounts
				sourceAccount->transfer(destinationAccount, sum);
			}

			// Project balance forward in time
			else if (command.compare("project") == 0)
			{
				if (parameters.size() != 2) throw Exceptions::IncorrectAmountOfArguments();

				const double amountOfYears = InputParser::parse<double>(parameters[1]);

				// Check amount
				if (amountOfYears <= 0) throw Exceptions::IncorrectArgumentValue("Project time should be more than 0!");

				// Check if last selected is not savings account
				Accounts::Savings* saving = dynamic_cast<Accounts::Savings*>(lastSelectedAccount);

				if (saving == nullptr) {

					// Get local copy of unmodifiable created accounts
					std::vector<Accounts::Account*> openedAccounts = accountFactory.GetAccounts();

					// Select last savings or ISA created account
					for (std::vector<Accounts::Account*>::reverse_iterator it = openedAccounts.rbegin(); it != openedAccounts.rend(); it++) {
						if (saving = dynamic_cast<Accounts::Savings*>(*it)) {
							break;
						}
					}

					// Deallocate memory of copy
					openedAccounts = std::vector<Accounts::Account*>();
				}

				// Check if no saving account created yet
				if (saving == nullptr) throw Exceptions::IncorrectArgumentTypes("No Regular Saving or ISA account(-s) were found!");

				// Calculate and print
				std::cout << (*saving).computeInterest(amountOfYears) << "\n";
			}

			else if (command.compare("search") == 0)
			{
				if (parameters.size() != 5) throw Exceptions::IncorrectAmountOfArguments();

				const int type = InputParser::parse<int>(parameters[1]);
				const double amount = InputParser::parse<double>(parameters[2]);
				const long long time_stamp = InputParser::parse<std::time_t>(parameters[3] + parameters[4]);

				// Check if user entered more than decimal precision
				const int dotPos = parameters[2].find(".");
				if (dotPos != std::string::npos) {
					if (parameters[2].size() - dotPos > 3) {
						throw Exceptions::IncorrectArgumentValue("Please provide correct opening balance!");
					}
				}

				// Check if any accounts are open
				if (accountFactory.GetAmountOfAccounts() == 0) throw Exceptions::IncorrectArgumentValue("No account(-s) exists!");

				// If no selected account, select last
				if (lastSelectedAccount == nullptr) lastSelectedAccount = accountFactory.GetLastCreatedAccount();

				bool anyTransactionsFound = false;
				std::vector<const Transaction*> transactions;

				// Try to find particular transactions for all accounts
				for (const Accounts::Account* account : accountFactory.GetAccounts()) {
					
					// Get transactions for each account
					transactions = account->searchTransaction(std::to_string(time_stamp) + std::to_string(type) + std::to_string(amount));

					// Nothing was found
					if (transactions.size() == 0) continue;
					anyTransactionsFound = true;

					// Print account data and appoporiate transactions
					std::cout << account->toString() << "\n";

					for (auto transaction : transactions) {
						// Print all the data related to transaction
						std::cout << (*transaction).toString();
					}

					std::cout << "\n";
				}
				
				// If nothing found throw exception
				if (!anyTransactionsFound) throw Exceptions::IncorrectArgumentTypes("No provided transaction was found!");
			}

			else if (command.compare("exit") == 0)
			{
				// do nothing
			}

			else
			{
				std::cout << '"' << command << '"' << " is not recognized as an internal or external command. Type 'options' / 'help' / '?' for list of options." << "\n";
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

		delete cstr;
		delete[] token;
	}

	// Deallocate all the memory
	delete lastSelectedAccount;

	std::cout << "Press any key to quit...";
	std::getchar();
}