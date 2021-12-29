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

	std::cout << "search parameter: find transaction by one of the parameters" << "\n\t"
		<< "{ Parameter -> Type (1), Money Amount (2), Date (3), Time (4) }" << "\n\n";

	std::cout << "exit: close this application" << "\n";
}

void getInput(std::vector <std::string>& parameters, std::string& userCommand, const char splitChar = ' ') {
	// Reset current parameters list
	parameters.clear();
	userCommand.clear();

	// Extract and store input
	std::getline(std::cin, userCommand);

	// Reinitialize C-like string and save it
	char* cstr = new char[userCommand.length() + 1];
	strcpy(cstr, userCommand.c_str());

	// Split input string into parts
	char* token;
	token = strtok(cstr, &splitChar);

	// Save each input parametr
	while (token != nullptr)
	{
		parameters.push_back(token);
		token = strtok(nullptr, &splitChar);
	}

	delete[] cstr;
	delete[] token;
}

int main()
{
	std::vector <std::string> parameters;
	std::string userCommand;

	AccountFactories::AccountFactory accountFactory;
	Accounts::Account* lastSelectedAccount = nullptr;

	std::cout << "~~~ Welcome to LincBank! ~~~" << "\n";
	printOptions();

	while (userCommand != "exit")
	{
		std::cout << "\n" << ">>> ";
		getInput(parameters, userCommand);

		std::string command = "";

		try 
		{
			// If no command - ignore
			if (parameters.empty()) continue;

			// Print new line for better user experience
			std::cout << "\n";

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
				const size_t dotPos = parameters[2].find(".");
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
					std::cout << "Current account created!" << "\n";
					break;

				case (2):
					lastSelectedAccount = accountFactory.CreateSavingsAccount(openingBalance);
					std::cout << "Savings account created!" << "\n";
					break;

				case (3):
					lastSelectedAccount = accountFactory.CreateISAAccount(openingBalance);
					std::cout << "ISA account created!" << "\n";
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
							std::cout << (*transaction).toString() << "\n";
						}

						if (acc != accountFactory.GetLastCreatedAccount()) std::cout << "\n";
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
						std::cout << transaction->toString() << "\n";
					}
				}
			}

			else if (command.compare("withdraw") == 0 || command.compare("deposit") == 0)
			{
				if (parameters.size() != 2) throw Exceptions::IncorrectAmountOfArguments();
				
				const double amount = InputParser::parse<double>(parameters[1]);

				// Check if user entered more than decimal precision
				const size_t dotPos = parameters[1].find(".");
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
				const size_t dotPos = parameters[3].find(".");
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
				if (parameters.size() != 2) throw Exceptions::IncorrectAmountOfArguments();

				// Check if any accounts are open
				if (accountFactory.GetAmountOfAccounts() == 0) throw Exceptions::IncorrectArgumentValue("No account(-s) exists!");

				const int type = InputParser::parse<int>(parameters[1]);

				// Print functionality info for each search type
				switch (type) {
				case (1):
					std::cout << "\n" << "Please provide an type of transaction." << "\n"
						<< "Types: " << "\n"
						<< " 1. Initial deposit" << "\n"
						<< " 2. Deposit" << "\n"
						<< " 3. Withdraw" << "\n"
						<< " 4. Transfer" << "\n\n";

					std::cout << "[search by type] >>> ";

					// Get input
					getInput(parameters, userCommand);

					// Check if only 1 parameter was passed
					if (parameters.size() != 1) throw Exceptions::IncorrectAmountOfArguments();
					break;

				case (2):
					std::cout << "\n" << "Please provide amount of money in transaction." << "\n\n";
					std::cout << "[search by money] >>> ";

					// Get input
					getInput(parameters, userCommand);

					// Check if only 1 parameter was passed
					if (parameters.size() != 1) throw Exceptions::IncorrectAmountOfArguments();
					break;

				case (3):
					std::cout << "\n" << "Please provide a date of transaction." << "\n"
						<< "\n" << "Examples:" << "\n"
						<< "3/12/2021 - Specific Date" << "\n\n"
						<< "Advanced Examples:" << "\n"
						<< "*/*/2021 - Transactions in 2021" << "\n"
						<< "*/12/* - Transactions in any year December" << "\n"
						<< "3/*/* - Transactions on 3rd of any month and year" << "\n"
						<< "*/12/2021 - Transactions in December of 2021" << "\n"
						<< "3/*/2021 - Transactions on 3rd of any month of 2021" << "\n"
						<< "3/12/* - Transactions on 3rd of December" << "\n\n";

					std::cout << "[search by date] >>> ";

					// Get input
					getInput(parameters, userCommand, '/');

					// Check if only 3 parameters were passed
					if (parameters.size() != 3) throw Exceptions::IncorrectAmountOfArguments();

					break;

				case (4):
					std::cout << "Please provide time of transaction in 24h type." << "\n"
						<< "\n" << "Examples: " << "\n"
						<< "13:12:59 - Specific Time" << "\n\n"
						<< "Advanced Examples:" << "\n"
						<< "*:*:59 - Transactions made in specific second of any minute" << "\n"
						<< "*:12:* - Transactions made in specific minute of any hour" << "\n"
						<< "*:12:59 - Transactions made in specific minute and second" << "\n"
						<< "13:*:* - Transactions made in specific hour" << "\n"
						<< "13:*:59 - Transactions made in specific hour and second" << "\n"
						<< "13:12:* - Transactions made in specific hour and minute" << "\n\n";

					std::cout << "[search by time] >>> ";

					// Get input
					getInput(parameters, userCommand, ':');

					// Check if only 3 parameters were passed
					if (parameters.size() != 3) throw Exceptions::IncorrectAmountOfArguments();

					break;

				default:
					throw Exceptions::IncorrectArgumentValue("Only 1-4 values are available!");
				}

				std::cout << "\n";

				bool anyTransactionsFound = false;
				std::vector<const Transaction*> foundTransactions;

				// Check transaction in each account
				for (Accounts::Account* account : accountFactory.GetAccounts()) {
					foundTransactions.clear();

					// Each type has own unique steps
					switch (type) {
					case (1):
						account->searchTransactionByType(foundTransactions, InputParser::parse<int>(parameters[0])-1);
						break;
					case (2):
						account->searchTransactionByAmount(foundTransactions, InputParser::parse<double>(parameters[0]));
						break;
					case (3):
						account->searchTransactionByDate(foundTransactions, parameters);
						break;
					case (4):
						account->searchTransactionByTime(foundTransactions, parameters);
						break;
					}
					
					if (foundTransactions.size() == 0) continue;

					// If any transactions are printed, no error message needed
					anyTransactionsFound = true;

					// Print data about account and selected transactions
					std::cout << account->toString() << "\n";

					for (auto transaction : foundTransactions) {
						std::cout << transaction->toString() << "\n";
					}

					std::cout << "\n";
				}

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
	}

	// Deallocate all the memory
	delete lastSelectedAccount;

	std::cout << "Press any key to quit...";
	std::getchar();
}