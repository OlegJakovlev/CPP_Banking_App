#pragma once

#ifndef InterestEarning_H
#define InterestEarning_H

#include <math.h>

class InterestEarning
{
public:
	virtual const double computeInterest(const double initialBalance, const double interestRate, const double years, const int interestApplied = 12) const;
};

#endif
