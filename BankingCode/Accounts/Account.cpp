#include "Account.h"

namespace Accounts {
	Account::Account(long newID, double amount) {
		id = newID;
		balance = amount;
		addTransaction(Transaction::transactionType::open_deposit, amount);
	}

	Account::~Account() {
		delete &historyUpdated;
	}

	void Account::deposit(double amount) {
		balance += amount;
		addTransaction(Transaction::transactionType::deposit, amount);
		std::cout << "Successfully deposited the money!" << std::endl;
	}

	void Account::withdraw(double amount) {
		balance -= amount;
		addTransaction(Transaction::transactionType::withdraw, amount);
		std::cout << "Successfully withdrawn the money!" << std::endl;
	}

	void Account::transfer(Account* destination, double amount) {
		// Substract balance
		balance -= amount;
		(*destination).balance += amount;
		
		addTransaction(Transaction::transactionType::transfer, amount, "Transfer to " + std::to_string((*destination).getID()));
		(*destination).addTransaction(Transaction::transactionType::transfer, amount, "Transfer from " + std::to_string(getID()));

		std::cout << "Money transfer successful!" << std::endl;
	}

	double Account::getBalance() {
		return balance;
	}

	long Account::getID()
	{
		return id;
	}

	std::string Account::toString() {
		return ("ID: " + std::to_string(id) + " | Type: " + accountName + " | Balance: " + std::to_string(balance));
	}

	Transaction* Account::searchTransaction(std::string key)
	{
		return historyUpdated[key];
	}

	void Account::addTransaction(Transaction::transactionType type, double amount, std::string description) {
		// Create new transaction
		Transaction* newTransaction = new Transaction(type, amount, description);

		// Hash table solution
		historyUpdated[
			std::to_string((int)type) +
				std::to_string(amount) +
				std::to_string(std::time(0))
		] = newTransaction;
	}

	std::vector<Transaction*> Account::getLastTransaction(int amount)
	{
		std::vector<Transaction*> result;

		// Create start and end pointers
		std::map<std::string, Transaction*>::reverse_iterator it = historyUpdated.rbegin();
		std::map<std::string, Transaction*>::reverse_iterator end_it  = it;
		
		// Iterate throught and get last memory adress possible / need
		std::advance(end_it, (amount > historyUpdated.size()) ? historyUpdated.size() : amount);

		// Iterate throught and save transactions
		for (; it != end_it; ++it) {
			result.push_back(it->second);
		}

		return result;
	}
}