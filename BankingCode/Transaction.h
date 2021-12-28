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
	Transaction(const transactionType& type, const double& newValue, std::string newDescription = "");
	Transaction(const Transaction&) = default; // 1/5: Copy Ctor
	Transaction(Transaction&&) noexcept = default; // 2/5: Move Ctor
	Transaction& operator=(const Transaction&) = default; // 3/5: Copy Assignment
	Transaction& operator=(Transaction&&) noexcept = default; // 4/5: Move Assignment
	~Transaction();

public:
	const std::string toString() const;
};

#endif