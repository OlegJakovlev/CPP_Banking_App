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
		Current(long newID, double newBalance, double newOverdraft = 500);
		~Current() override;

	public:
		double getOverdraft();
		void withdraw(double amount) override;
		void deposit(double amount) override;
		std::string toString() override;
	};
}

#endif