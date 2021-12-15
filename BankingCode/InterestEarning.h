#pragma once

#ifndef InterestEarning_H
#define InterestEarning_H

#include <math.h>

class InterestEarning
{
public:
	virtual double computeInterest(double initialBalance, double interestRate, double years, int interestApplied = 12);
};

#endif
