#include "InterestEarning.h"

double InterestEarning::computeInterest(double initialBalance, double interestRate, double years, int interestApplied) {
	return initialBalance * pow(1 + interestRate / interestApplied, interestApplied * years);
}