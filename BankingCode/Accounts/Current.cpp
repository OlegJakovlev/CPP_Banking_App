#include "Current.h"

namespace Accounts {
	Current::Current(long newID, double newBalance, double newOverdraft) : Accounts::Account(newID, newBalance) {
		accountName = "Current";
		overdraft = newOverdraft;
		initialOverdraft = newOverdraft;
	}

	Current::~Current() {}

	double Current::getOverdraft() {
		return overdraft;
	}

	void Current::withdraw(double amount) {
		// Check if overdraft should be used
		double difference = amount - balance;

		// Create transaction record
		addTransaction(Transaction::transactionType::withdraw, amount);

		if (difference > 0) {
			overdraft -= difference;

			// Reduce amount by factor of used overdraft
			amount -= difference;
		}
		
		// Substract from balance
		balance -= amount;
		std::cout << "Successfully withdrawn the money!" << std::endl;
	}

	void Current::deposit(double amount) {
		// Check if overdraft was used
		double overdraftLeft = initialOverdraft - overdraft;

		// Create transaction record
		addTransaction(Transaction::transactionType::deposit, amount);

		// Reduce amount by factor of returned overdraft
		if (amount >= overdraftLeft) {
			overdraft += overdraftLeft;
			amount -= overdraftLeft;
		}

		// Add to balance
		balance += amount;
		std::cout << "Successfully deposited the money!" << std::endl;
	}

	std::string Current::toString()
	{
		return Account::toString() + " | Overdraft available: $" + std::to_string(overdraft);
	}
}