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
		AccountFactory(const AccountFactory&) = delete; // 1/5: Copy Ctor
		AccountFactory(AccountFactory&&) noexcept = delete; // 2/5: Move Ctor
		AccountFactory& operator=(const AccountFactory&) = delete; // 3/5: Copy Assignment
		AccountFactory& operator=(AccountFactory&&) noexcept = delete; // 4/5: Move Assignment
		~AccountFactory();

	public:
		const int GetAmountOfAccounts() const;
		const std::vector <Accounts::Account*>& GetAccounts() const;
		Accounts::Account* const GetLastCreatedAccount() const;
		Accounts::Account* const GetAccountByIndex(const int& index) const;
		Accounts::Account* const CreateRegularAccount(const double& openingBalance, double newOverdraft = 500.00);
		Accounts::Account* const CreateSavingsAccount(const double& openingBalance);
		Accounts::Account* const CreateISAAccount(const double& openingBalance);
	};
}

#endif