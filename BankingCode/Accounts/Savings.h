#pragma once

#ifndef Savings_H
#define Savings_H

#include "Account.h"
#include "../InterestEarning.h"

namespace Accounts {
	class Savings : public Account, public InterestEarning
	{
	public:
		Savings() = delete;
		Savings(const long newID, const double openingBalance, bool isIsa=false);
		Savings(const Savings&) = delete; // 1/5: Copy Ctor
		Savings& operator=(const Savings&) = delete; // 2/5: Copy Assignment
		Savings(Savings&&) noexcept = delete; // 3/5: Move Ctor
		Savings& operator=(Savings&&) noexcept = delete; // 4/5: Move Assignment
		~Savings();

	private:
		double interestRate = 0.0085;
		bool isa = false;

	public:
		// Property getters
		const bool isISA() const;

		// Interest related operations
		const double computeInterest(const double years) const;	
	};
}

#endif