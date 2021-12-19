#define _CRT_SECURE_NO_WARNINGS
#include "Transaction.h"

Transaction::Transaction(transactionType type, double newValue, std::string newDescription) {
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

bool Transaction::operator==(const Transaction* transactionToCompareWith) const {
	return (transactionToCompareWith->timestamp == timestamp 
		&& transactionToCompareWith->desc == desc 
		&& transactionToCompareWith->value == value
	);
}

bool Transaction::operator>(const Transaction* transactionToCompareWith) const {
	return (transactionToCompareWith->timestamp > timestamp
		&& transactionToCompareWith->desc > desc
		&& transactionToCompareWith->value > value
		);
}

bool Transaction::operator<(const Transaction* transactionToCompareWith) const {
	return transactionToCompareWith > this;
}

std::string Transaction::toString() {
	return "-- " + desc + ": $" + std::to_string(value) + " on " + std::asctime(std::localtime(&timestamp)); // TODO: Replace with safe function
}
