#define _CRT_SECURE_NO_WARNINGS
#include "Transaction.h"

Transaction::Transaction(const transactionType& type, const double newValue, const std::string newDescription) {
	if (newDescription == "") {
		switch ((int)type) {
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
	else {
		desc = newDescription;
	}

	value = newValue;
	timestamp = std::time(0);
}

Transaction::~Transaction() { }

const std::string Transaction::toString() const {
	return "-- " + desc 
		+ ": $" 
		+ std::to_string(value).substr(0, std::to_string(value).find(".") + 3) 
		+ " on " 
		+ std::asctime(std::localtime(&timestamp)); // TODO: Replace with safe function
}
