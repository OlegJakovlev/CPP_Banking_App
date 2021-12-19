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
		std::map<std::string, Transaction*> history; // Dictionary [key]=value (Red-black tree)
		std::string accountName;

	protected:
		virtual void addTransaction(Transaction::transactionType type, double amount, std::string description = "");

	public:
		Account(long newID, double amount);
		virtual ~Account();

	public:
		virtual double getBalance();
		virtual long getID();

		virtual void deposit(double amount);
		virtual void withdraw(double amount);
		virtual void transfer(Account* destination, double amount);

		virtual std::string toString();

		virtual Transaction* searchTransaction(std::string key);
		virtual std::vector<Transaction*> getLastTransaction(int amount);
	};
}


#endif
