#define _CRT_SECURE_NO_WARNINGS
#include "Transaction.h"

Transaction::Transaction(const transactionType& newType, const double newValue, const std::string& description) {
	type = newType;
	desc = description;

	// Apply default description if there is no current
	if (description == "") {
		switch ((int) type) {
		case 0:
			desc = "Initial deposit";
			break;
		case 1:
			desc = "Deposit";
			break;
		case 2:
			desc = "Withdraw";
			break;
		case 3:
			desc = "Transfer";
			break;
		}
	}

	// Set amount of transaction
	value = newValue;

	// Get current time
	auto now = std::chrono::system_clock::now();
	time_t tt = std::chrono::system_clock::to_time_t(now);
	date = *localtime(&tt);
}

Transaction::~Transaction() { }

const std::string Transaction::toString() const {
	return "-- " + desc 
		+ ": $" 
		+ std::to_string(value).substr(0, std::to_string(value).find(".") + 3) 
		+ " on " 
		+ getTimestamp();
}

const std::string Transaction::getTimestamp() const {
	char buffer[30];

	// Pretty-parse date + time
	strftime(buffer, sizeof(buffer), "%d/%m/%y %T", &date);

	return buffer;
}

const double Transaction::getAmount() const
{
	return value;
}

const int Transaction::getNumericType() const {
	return (int) type;
}

const tm Transaction::getDateObject() const 
{
	return date;
}
