#pragma once

#ifndef Account_H
#define Account_H

#include <iostream>
#include <vector>
#include <map>

#include "../Transaction.h"
#include "../InputParser.h"

namespace Accounts {
	class Account
	{
	protected:
		long id;
		double balance;
		std::vector<const Transaction*> history;
		std::string accountName;

	protected:
		virtual void addTransaction(const Transaction::transactionType& type, const double amount, const std::string& description = "");

	public:
		Account(const long newID, const double amount); // 0/5: Default Ctor
		Account(const Account&) = default; // 1/5: Copy Ctor
		Account(Account&&) noexcept = default; // 2/5: Move Ctor
		Account& operator=(const Account&) = default; // 3/5: Copy Assignment
		Account& operator=(Account&&) noexcept = default; // 4/5: Move Assignment
		virtual ~Account(); // 5/5: Dtor

	public:
		virtual double getBalance() const;
		virtual long getID() const;

		virtual void deposit(const double amount,
			const Transaction::transactionType& type = Transaction::transactionType::deposit,
			const std::string successfulMessage = "Successfully deposited the money!",
			const std::string transactionDescription = "");

		virtual void withdraw(const double amount,
			const Transaction::transactionType& type = Transaction::transactionType::withdraw,
			const std::string successfulMessage = "Successfully withdrawn the money!",
			const std::string transactionDescription = "");

		virtual void transfer(Account* destination, const double amount);

		virtual const std::string toString() const;

		virtual void searchTransactionByType(std::vector<const Transaction*>& transactions, const int type) const;
		virtual void searchTransactionByAmount(std::vector<const Transaction*>& transactions, const int amount) const;
		virtual void searchTransactionByDate(std::vector<const Transaction*>& transactions, const std::vector<std::string>& parsedSearchDate) const;
		virtual void searchTransactionByTime(std::vector<const Transaction*>& transactions, const std::vector<std::string>& parsedSearchTime) const;

		virtual const std::vector<const Transaction*> getLastTransaction(const int amount) const;
		virtual const std::vector<const Transaction*> getAllTransaction() const;
	};
}


#endif
