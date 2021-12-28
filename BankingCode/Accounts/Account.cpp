#include "Account.h"

namespace Accounts {
	Account::Account(const long& newID, const double& amount) {
		id = newID;
		balance = amount;
		addTransaction(Transaction::transactionType::open_deposit, amount);
	}

	Account::~Account() {

	}

	void Account::deposit(const double& amount) {
		balance += amount;
		addTransaction(Transaction::transactionType::deposit, amount);
		std::cout << "Successfully deposited the money!" << std::endl;
	}

	void Account::withdraw(const double& amount) {
		balance -= amount;
		addTransaction(Transaction::transactionType::withdraw, amount);
		std::cout << "Successfully withdrawn the money!" << std::endl;
	}

	void Account::transfer(Account* destination, const double& amount) {
		balance -= amount;
		(*destination).balance += amount;
		
		// Create transaction for each of accounts with different descriptions
		addTransaction(Transaction::transactionType::transfer, amount, "Transfer to " + std::to_string((*destination).getID()));
		(*destination).addTransaction(Transaction::transactionType::transfer, amount, "Transfer from " + std::to_string(getID()));

		std::cout << "Money transfer successful!" << std::endl;
	}

	double Account::getBalance() const {
		return balance;
	}

	long Account::getID() const
	{
		return id;
	}

	const std::string Account::toString() const {
		return ("ID: " + std::to_string(id) 
			+ " | Type: " + accountName 
			+ " | Balance: $"
			+ std::to_string(balance).substr(0, std::to_string(balance).find(".") + 3));
	}

	const Transaction* const Account::searchTransaction(const std::string& key) const
	{
		try {
			return history.at(key);
		}
		catch (std::out_of_range) {
			return NULL;
		}
	}

	void Account::addTransaction(const Transaction::transactionType& type, const double& amount, std::string description) {
		// Concatenate the key
		std::string key = std::to_string(std::time(0)) +
			std::to_string((int)type) +
			std::to_string(amount);

		// Check if same transaction exists
		if (searchTransaction(key)) throw Exceptions::IncorrectArgumentValue("You are so fast! Same operation in 1 second are prohibited!");

		Transaction* const newTransaction = new Transaction(type, amount, description);

		// Red-Black Tree solution
		history[key] = newTransaction;
	}

	const std::vector<const Transaction*> Account::getLastTransaction(const int& amount) const
	{
		std::vector<const Transaction*> result;

		// Initialize start and end pointers
		std::map<std::string, const Transaction*>::const_reverse_iterator it = history.crbegin();
		std::map<std::string, const Transaction*>::const_reverse_iterator end_it  = it;
		
		// Iterate throught and get last memory adress possible / need
		std::advance(end_it, (amount > history.size()) ? history.size() : amount);

		// Iterate throught and save transactions
		for (; it != end_it; it++) {
			result.push_back(it->second);
		}

		return result;
	}

	const std::vector<const Transaction*> Account::getAllTransaction() const
	{
		std::vector<const Transaction*> result;

		// Initialize start and end pointers
		std::map<std::string, const Transaction*>::const_reverse_iterator it = history.crbegin();
		std::map<std::string, const Transaction*>::const_reverse_iterator end_it = history.crend();

		// Iterate throught and save transactions
		for (; it != end_it; it++) {
			result.push_back(it->second);
		}

		return result;
	}
}