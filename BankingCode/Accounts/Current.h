#pragma once

#ifndef Current_H
#define Current_H

#include "Account.h"

namespace Accounts {
	class Current : public Account
	{
	public:
		Current() = delete;
		Current(const long newID, const double newBalance, const double newOverdraft = 500);
		Current(const Current&) = delete; // 1/5: Copy Ctor
		Current& operator=(const Current&) = delete; // 2/5: Copy Assignment
		Current(Current&&) noexcept = delete; // 3/5: Move Ctor
		Current& operator=(Current&&) noexcept = delete; // 4/5: Move Assignment
		~Current();

	private:
		double initialOverdraft;
		double overdraft;

	public:
		// Property Gettter
		const double getOverdraft() const;

		// Visual representation
		const std::string toString() const override;

		// Money related operations
		void deposit(const double amount,
			const Transaction::transactionType& type = Transaction::transactionType::deposit,
			const std::string& successfulMessage = "Successfully deposited the money!",
			const std::string& transactionDescription = "") override;

		void withdraw(const double amount,
			const Transaction::transactionType& type = Transaction::transactionType::withdraw,
			const std::string& successfulMessage = "Successfully withdrawn the money!",
			const std::string& transactionDescription = "") override;

		
	};
}

#endif