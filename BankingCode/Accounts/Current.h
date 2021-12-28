#pragma once

#ifndef Current_H
#define Current_H

#include "Account.h"

namespace Accounts {
	class Current : public Account
	{
	private:
		double initialOverdraft;
		double overdraft;

	public:
		Current(const long& newID, const double& newBalance, double newOverdraft = 500);
		Current(const Current&) = default; // 1/5: Copy Ctor
		Current(Current&&) noexcept = default; // 2/5: Move Ctor
		Current& operator=(const Current&) = default; // 3/5: Copy Assignment
		Current& operator=(Current&&) noexcept = default; // 4/5: Move Assignment
		~Current() override;

	public:
		const double getOverdraft() const;
		void withdraw(const double& amount) override;
		void deposit(const double& amount) override;
		const std::string toString() const override;
	};
}

#endif