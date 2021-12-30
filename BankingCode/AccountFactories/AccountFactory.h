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
	public:
		explicit AccountFactory() = default;
		explicit AccountFactory(const AccountFactory& copyFactory); // 1/5: Copy Ctor
		AccountFactory& operator=(const AccountFactory& copyFactory); // 2/5: Copy Assignment
		explicit AccountFactory(AccountFactory&& newFactory) noexcept; // 3/5: Move Ctor
		AccountFactory& operator=(AccountFactory&& newFactory) noexcept; // 4/5: Move Assignment
		~AccountFactory();
	
	private:
		long newID = 1;
		std::vector <Accounts::Account*> openedAccounts;

	public:
		const int GetAmountOfAccounts() const;
		const std::vector <Accounts::Account*>& GetAccounts() const;
		Accounts::Account* const GetLastCreatedAccount() const;
		Accounts::Account* const GetAccountByIndex(const int index) const;
		Accounts::Account* const CreateRegularAccount(const double openingBalance, const double newOverdraft = 500.00);
		Accounts::Account* const CreateSavingsAccount(const double openingBalance);
		Accounts::Account* const CreateISAAccount(const double openingBalance);
	};
}

#endif