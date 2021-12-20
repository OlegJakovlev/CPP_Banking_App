#pragma once

#ifndef AccountFactory_H
#define AccountFactory_H

#include <algorithm>

#include "../Accounts/Account.h"
#include "../Accounts/Current.h"
#include "../Accounts/Savings.h"

namespace AccountFactories {
	class AccountFactory
	{
	private:
		long newID = 1;
		std::vector <Accounts::Account*> openedAccounts;

	public:
		AccountFactory();
		~AccountFactory();

	public:
		int GetAmountOfAccounts();
		std::vector <Accounts::Account*> GetAccounts();
		Accounts::Account* GetLastCreatedAccount();
		Accounts::Account* GetAccountByIndex(int index);
		Accounts::Account* CreateRegularAccount(double openingBalance, double newOverdraft = 500);
		Accounts::Account* CreateSavingsAccount(double openingBalance);
		Accounts::Account* CreateISAAccount(double openingBalance);
	};
}

#endif