#include "Account.h"

namespace Accounts {
	Account::Account(const long newID, const double amount) {
		id = newID;
		balance = amount;
		addTransaction(Transaction::transactionType::open_deposit, amount);
	}

	Account::~Account() {
		// Deallocate all pointers of transactions
		std::vector<const Transaction*>::const_iterator it = history.cbegin();

		while (it != history.cend()) {
			delete *it;
			it = history.erase(it);
		}

		history.clear();
		history.shrink_to_fit();
	}

	void Account::deposit(const double amount,
		const Transaction::transactionType& type,
		const std::string& successfulMessage,
		const std::string& transactionDescription
	) {
		balance += amount;
		addTransaction(type, amount, transactionDescription);
		if (successfulMessage != "") std::cout << successfulMessage << "\n";
	}

	void Account::withdraw(const double amount,
		const Transaction::transactionType& type,
		const std::string& successfulMessage,
		const std::string& transactionDescription
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

	const std::string& Account::getTypeName() const
	{
		return accountName;
	}

	const std::string Account::toString() const {
		return ("ID: " + std::to_string(id) 
			+ " | Type: " + accountName 
			+ " | Balance: $"
			+ std::to_string(balance).substr(0, std::to_string(balance).find(".") + 3));
	}

	void Account::searchTransactionByType(std::vector<const Transaction*>& transactions, const int type) const
	{
		std::vector<const Transaction*>::const_reverse_iterator transaction = history.crbegin();

		for (; transaction != history.crend(); transaction++) {
			if ((*transaction)->getNumericType() == type) {
				transactions.push_back(*transaction);
			}
		}
	}

	void Account::searchTransactionByAmount(std::vector<const Transaction*>& transactions, const int amount) const
	{
		std::vector<const Transaction*>::const_reverse_iterator transaction = history.crbegin();

		for (; transaction != history.crend(); transaction++) {
			if ((*transaction)->getAmount() == amount) {
				transactions.push_back(*transaction);
			}
		}
	}

	void Account::searchTransactionByDate(std::vector<const Transaction*>& transactions, const std::vector<std::string>& parsedSearchDate) const
	{
		std::vector<const Transaction*>::const_reverse_iterator transaction = history.crbegin();

		for (; transaction != history.crend(); transaction++) {
			bool equal = true;

			// Get transaction date
			tm transactionDate = (*transaction)->getDateObject();
			std::vector<std::string> splitTransactionDate;
			splitTransactionDate.push_back(std::to_string(transactionDate.tm_mday));
			splitTransactionDate.push_back(std::to_string(transactionDate.tm_mon + 1));
			splitTransactionDate.push_back(std::to_string(transactionDate.tm_year + 1900));

			// Compare dates 
			for (int i = 0; i < parsedSearchDate.size(); i++) {
				// If user input *, we ignore date difference
				if (parsedSearchDate[i] != "*" && parsedSearchDate[i] != splitTransactionDate[i]) {
					equal = false;
				}
			}

			if (equal) transactions.push_back(*transaction);
		}
	}

	void Account::searchTransactionByTime(std::vector<const Transaction*>& transactions, const std::vector<std::string>& parsedSearchTime) const
	{
		std::vector<const Transaction*>::const_reverse_iterator transaction = history.crbegin();

		for (; transaction != history.crend(); transaction++) {
			bool equal = true;

			// Get transaction date
			const tm transactionTime = (*transaction)->getDateObject();
			std::vector<std::string> splitTransactionTime;
			splitTransactionTime.push_back(std::to_string(transactionTime.tm_hour));
			splitTransactionTime.push_back(std::to_string(transactionTime.tm_min));
			splitTransactionTime.push_back(std::to_string(transactionTime.tm_sec));

			// Compare dates
			for (int i = 0; i < parsedSearchTime.size(); i++) {
				// If user input *, we ignore date difference
				if (parsedSearchTime[i] != "*" && parsedSearchTime[i] != splitTransactionTime[i]) {
					equal = false;
				}
			}

			if (equal) transactions.push_back(*transaction);
		}
	}

	void Account::addTransaction(const Transaction::transactionType& type, const double amount, const std::string& description) {
		history.push_back(new Transaction(type, amount, description));
	}

	const std::vector<const Transaction*> Account::getLastTransaction(const int amount) const
	{
		std::vector<const Transaction*> result;

		// Copy max / requested elements from the end
		copy_n(history.rbegin(), (amount > history.size()) ? history.size() : amount, back_inserter(result));

		return result;
	}

	const std::vector<const Transaction*> Account::getAllTransaction() const
	{
		return getLastTransaction(history.size());
	}
}