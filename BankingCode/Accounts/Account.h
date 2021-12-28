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
		std::map<std::string, const Transaction*> history;
		std::string accountName;

	protected:
		virtual void addTransaction(const Transaction::transactionType& type, const double& amount, std::string description = "");

	public:
		Account(const long& newID, const double& amount); // 0/5: Default Ctor
		Account(const Account&) = default; // 1/5: Copy Ctor
		Account(Account&&) noexcept = default; // 2/5: Move Ctor
		Account& operator=(const Account&) = default; // 3/5: Copy Assignment
		Account& operator=(Account&&) noexcept = default; // 4/5: Move Assignment
		virtual ~Account(); // 5/5: Dtor

	public:
		virtual double getBalance() const;
		virtual long getID() const;

		virtual void deposit(const double& amount);
		virtual void withdraw(const double& amount);
		virtual void transfer(Account* destination, const double& amount);

		virtual const std::string toString() const;

		virtual const Transaction* const searchTransaction(const std::string& key) const;
		virtual const std::vector<const Transaction*> getLastTransaction(const int& amount) const;
		virtual const std::vector<const Transaction*> getAllTransaction() const;
	};
}


#endif
