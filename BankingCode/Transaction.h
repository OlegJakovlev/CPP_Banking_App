#pragma once

#ifndef Transaction_H
#define Transaction_H

#include <iostream>
#include <string>
#include <chrono>

class Transaction
{
public:
	enum class transactionType { open_deposit, deposit, withdraw, transfer };

	Transaction(const transactionType& newType, const double newValue, const std::string& description = "");
	Transaction(const Transaction&) = default; // 1/5: Copy Ctor
	Transaction(Transaction&&) noexcept = default; // 2/5: Move Ctor
	Transaction& operator=(const Transaction&) = default; // 3/5: Copy Assignment
	Transaction& operator=(Transaction&&) noexcept = default; // 4/5: Move Assignment
	~Transaction();

private:
	double value;
	transactionType type;
	std::string desc;
	tm date;

public:
	const double getAmount() const;
	const int getNumericType() const;
	const std::string getTimestamp() const;
	const tm getDateObject() const;
	const std::string toString() const;
};

#endif