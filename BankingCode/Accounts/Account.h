#pragma once

#ifndef Account_H
#define Account_H

#include <iostream>
#include <vector>

#include "../Transaction.h"
#include "../InputParser.h"

namespace Accounts {
	class Account
	{
	public:
		Account() = delete;
		Account(const long newID, const double amount);
		Account(const Account&) = delete; // 1/5: Copy Ctor (deep-copy)
		Account& operator=(const Account&) = delete; // 2/5: Copy Assignment
		Account(Account&&) noexcept = delete; // 3/5: Move Ctor (shallow-copy)
		Account& operator=(Account&&) noexcept = delete; // 4/5: Move Assignment
		virtual ~Account();
	
	protected:
		long id;
		double balance;
		std::vector<const Transaction*> history;
		std::string accountName;

	protected:
		virtual void addTransaction(const Transaction::transactionType& type, const double amount, const std::string& description = "");

	public:
		// Property getters
		virtual double getBalance() const;
		virtual long getID() const;

		// Visual representation
		virtual const std::string toString() const;

		// Money related operations
		virtual void deposit(const double amount,
			const Transaction::transactionType& type = Transaction::transactionType::deposit,
			const std::string& successfulMessage = "Successfully deposited the money!",
			const std::string& transactionDescription = "");

		virtual void withdraw(const double amount,
			const Transaction::transactionType& type = Transaction::transactionType::withdraw,
			const std::string& successfulMessage = "Successfully withdrawn the money!",
			const std::string& transactionDescription = "");

		virtual void transfer(Account* destination, const double amount);

		// Transactions related functions
		virtual void searchTransactionByType(std::vector<const Transaction*>& transactions, const int type) const;
		virtual void searchTransactionByAmount(std::vector<const Transaction*>& transactions, const int amount) const;
		virtual void searchTransactionByDate(std::vector<const Transaction*>& transactions, const std::vector<std::string>& parsedSearchDate) const;
		virtual void searchTransactionByTime(std::vector<const Transaction*>& transactions, const std::vector<std::string>& parsedSearchTime) const;
		virtual const std::vector<const Transaction*> getLastTransaction(const int amount) const;
		virtual const std::vector<const Transaction*> getAllTransaction() const;
	};
}


#endif
