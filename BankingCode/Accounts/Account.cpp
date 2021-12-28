#include "Account.h"

namespace Accounts {
	Account::Account(const long newID, const double amount) {
		id = newID;
		balance = amount;
		addTransaction(Transaction::transactionType::open_deposit, amount);
	}

	Account::~Account() {
		// Deallocate all pointers of transactions
		std::map<std::string, const Transaction*>::const_iterator it = history.cbegin();

		while (it != history.cend()) {
			delete it->second;
			it = history.erase(it);
		}
	}

	void Account::deposit(const double amount,
		const Transaction::transactionType& type,
		const std::string successfulMessage,
		const std::string transactionDescription
	) {
		balance += amount;
		addTransaction(type, amount, transactionDescription);
		if (successfulMessage != "") std::cout << successfulMessage << "\n";
	}

	void Account::withdraw(const double amount,
		const Transaction::transactionType& type,
		const std::string successfulMessage,
		const std::string transactionDescription
	) {
		balance -= amount;
		addTransaction(type, amount, transactionDescription);
		if (successfulMessage != "") std::cout << successfulMessage << "\n";
	}

	void Account::transfer(Account* destination, const double amount) {

		// Create transfer transaction with custom description by using withdraw and deposit functions
		withdraw(amount, Transaction::transactionType::transfer, "", "Transfer to " + std::to_string((*destination).getID()));
		(*destination).deposit(amount, Transaction::transactionType::transfer, "", "Transfer from " + std::to_string(getID()));

		std::cout << "Money transfer successful!" << "\n";
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

	const std::vector<const Transaction*> const Account::searchTransaction(const std::string key) const
	{
		std::vector<const Transaction*> result;

		for (std::multimap<std::string, const Transaction*>::const_iterator it = history.find(key); it != history.cend(); it++) {
			result.push_back((*it).second);
		}

		return result;
	}

	void Account::addTransaction(const Transaction::transactionType& type, const double amount, const std::string description) {
		// Concatenate the key
		std::string key = std::to_string(std::time(0)) +
			std::to_string((int)type) +
			std::to_string(amount);

		Transaction* const newTransaction = new Transaction(type, amount, description);

		history.insert({ key, newTransaction });
	}

	const std::vector<const Transaction*> Account::getLastTransaction(const int amount) const
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