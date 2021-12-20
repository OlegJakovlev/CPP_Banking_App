#pragma once

#ifndef Savings_H
#define Savings_H

#include "Account.h"
#include "../InterestEarning.h"

namespace Accounts {
	class Savings : public Account, public InterestEarning
	{
	private:
		double interestRate = 0.0085;
		bool isa = false;
	
	public:
		Savings(long newID, double openingBalance, bool isIsa=false);
		~Savings() override;
		double computeInterest(double years);
		bool isISA();
	};
}

#endif