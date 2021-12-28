#include "Savings.h"

namespace Accounts {
	Savings::Savings(const long newID, const double openingBalance, bool isIsa) : Accounts::Account(newID, openingBalance) {
		accountName = "Savings";

		if (isIsa) {
			isa = isIsa;
			interestRate = 0.0115;
			accountName = "ISA";
		}
	}

	Savings::~Savings() {}

	const bool Savings::isISA() const {
		return isa;
	}

	const double Savings::computeInterest(const double years) const {
		return InterestEarning::computeInterest(balance, interestRate, years);
	}
}