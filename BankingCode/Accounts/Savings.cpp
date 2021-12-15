#include "Savings.h"

namespace Accounts {
	Savings::Savings(long newID, double openingBalance, bool isIsa) : Accounts::Account(newID, openingBalance) {
		accountName = "Savings";

		if (isIsa) {
			isa = isIsa;
			interestRate = 0.0115;
			accountName = "ISA";
		}
	}

	Savings::~Savings() {}

	bool Savings::isISA() {
		return isa;
	}

	double Savings::computeInterest(double years) {
		return InterestEarning::computeInterest(balance, interestRate, years);
	}
}