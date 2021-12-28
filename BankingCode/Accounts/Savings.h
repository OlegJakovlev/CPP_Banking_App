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
		Savings(const long newID, const double openingBalance, bool isIsa=false);
		Savings(const Savings&) = default; // 1/5: Copy Ctor
		Savings(Savings&&) noexcept = default; // 2/5: Move Ctor
		Savings& operator=(const Savings&) = default; // 3/5: Copy Assignment
		Savings& operator=(Savings&&) noexcept = default; // 4/5: Move Assignment
		~Savings();

	public:
		const double computeInterest(const double years) const;
		const bool isISA() const;
	};
}

#endif