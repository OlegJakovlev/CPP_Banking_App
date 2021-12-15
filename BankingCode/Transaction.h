#pragma once

#ifndef Transaction_H
#define Transaction_H

#include <iostream>
#include <string>
#include <ctime>

class Transaction
{
private:
	std::string desc;
	std::time_t timestamp;
	double value;

public:
	enum class transactionType { open_deposit, deposit, withdraw, transfer };
	Transaction(transactionType type, double newValue, std::string newDescription = "");
	~Transaction();

public:
	
	bool operator==(const Transaction* transactionToCompareWith) const;
	std::string toString();
};

#endif