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

	Transaction() = delete;
	Transaction(const transactionType& newType, const double newValue, const std::string& description = "");
	Transaction(const Transaction&) = delete; // 1/5: Copy Ctor
	Transaction& operator=(const Transaction&) = delete; // 2/5: Copy Assignment
	Transaction(Transaction&&) noexcept = delete; // 3/5: Move Ctor
	Transaction& operator=(Transaction&&) noexcept = delete; // 4/5: Move Assignment
	~Transaction();

private:
	double value;
	transactionType type;
	std::string desc;
	tm date;

public:
	// Properties Getters
	const double getAmount() const;
	const int getNumericType() const;
	const std::string getTimestamp() const;
	const tm& getDateObject() const;

	// Visual representation
	const std::string toString() const;
};

#endif