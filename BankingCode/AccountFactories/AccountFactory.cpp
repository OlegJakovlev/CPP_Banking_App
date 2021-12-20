#include "AccountFactory.h"

namespace AccountFactories {
	AccountFactory::AccountFactory()
	{
	}

	AccountFactory::~AccountFactory()
	{
		delete &openedAccounts;
	}

	int AccountFactory::GetAmountOfAccounts()
	{
		return newID - 1;
	}

	std::vector<Accounts::Account*> AccountFactory::GetAccounts()
	{
		return openedAccounts;
	}

	Accounts::Account* AccountFactory::GetLastCreatedAccount()
	{
		return openedAccounts[GetAmountOfAccounts()];
	}

	Accounts::Account* AccountFactory::GetAccountByIndex(int index)
	{
		return openedAccounts[index];
	}

	Accounts::Account* AccountFactory::CreateRegularAccount(double openingBalance, double newOverdraft)
	{
		if (openingBalance < 0) throw Exceptions::IncorrectArgumentValue("All accounts require opening balance of 0 or more!");

		// Create new account and add to the list
		Accounts::Account* newAccount = new Accounts::Current(newID++, openingBalance, newOverdraft);
		openedAccounts.push_back(newAccount);
		
		return newAccount;
	}

	Accounts::Account* AccountFactory::CreateSavingsAccount(double openingBalance)
	{
		if (openingBalance < 0) throw Exceptions::IncorrectArgumentValue("All accounts require opening balance of 0 or more!");

		Accounts::Account* newAccount = new Accounts::Savings(newID++, openingBalance);
		openedAccounts.push_back(newAccount);

		return newAccount;
	}

	Accounts::Account* AccountFactory::CreateISAAccount(double openingBalance)
	{
		if (openingBalance < 1000) throw Exceptions::IncorrectArgumentValue("ISA accounts require opening balance of 1000 or more!");
		
		// Check if ISA is already open
		Accounts::Savings* ISA = NULL;

		std::for_each(openedAccounts.begin(), openedAccounts.end(), [&](Accounts::Account* account) {

			// Check if Savings account
			if (ISA = dynamic_cast<Accounts::Savings*>(account)) {

				// Check if account is ISA
				if ((*ISA).isISA()) {
					ISA = (Accounts::Savings*)account;
					return;
				}
			}

			ISA = NULL;
		});

		if (ISA) throw Exceptions::IncorrectArgumentValue("ISA account already opened! Only 1 ISA account is available at time!");

		Accounts::Account* newAccount = new Accounts::Savings(newID++, openingBalance, true);
		openedAccounts.push_back(newAccount);

		return newAccount;

	}
}
