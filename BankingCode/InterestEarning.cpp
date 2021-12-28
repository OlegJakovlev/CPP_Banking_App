#include "InterestEarning.h"

const double InterestEarning::computeInterest(const double initialBalance, const double interestRate, const double years, const int interestApplied) const {
	return initialBalance * pow(1 + interestRate / interestApplied, interestApplied * years);
}