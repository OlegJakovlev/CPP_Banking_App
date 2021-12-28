#include "Current.h"

namespace Accounts {
	Current::Current(const long& newID, const double& newBalance, double newOverdraft) : Accounts::Account(newID, newBalance) {
		accountName = "Current";
		overdraft = newOverdraft;
		initialOverdraft = newOverdraft;
	}

	Current::~Current() {}

	const double Current::getOverdraft() const {
		return overdraft;
	}

	void Current::withdraw(const double& amount) {
		// Check if overdraft should be used
		double difference = amount - balance;

		// Create transaction record
		addTransaction(Transaction::transactionType::withdraw, amount);

		if (difference > 0) {
			overdraft -= difference;

			// Substract from balance and increase by factor of used overdraft
			balance -= amount - difference;
		}
		else {
			balance -= amount;
		}
		
		std::cout << "Successfully withdrawn the money!" << std::endl;
	}

	void Current::deposit(const double& amount) {
		// Check if overdraft was used
		double overdraftUsed = initialOverdraft - overdraft;

		// Create transaction record
		addTransaction(Transaction::transactionType::deposit, amount);

		// Check if overdraft was used
		if (overdraftUsed > 0) {

			// If amount is bigger than used overdraft, cover the overdraft first
			if (amount > overdraftUsed) {
				overdraft += overdraftUsed;
				balance += amount - overdraftUsed;
			}

			// If amount deposited is less than used, try to cover the overdraft
			else {
				overdraft += amount;
			}
		}
		else {
			balance += amount;
		}

		// Add to balance
		std::cout << "Successfully deposited the money!" << std::endl;
	}

	const std::string Current::toString() const
	{
		return Account::toString()
			+ " | Overdraft available: $" 
			+ std::to_string(overdraft).substr(0, std::to_string(overdraft).find(".") + 3);
	}
}