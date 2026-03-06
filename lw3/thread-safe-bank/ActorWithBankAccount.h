#ifndef ACTORWITHBANKACCOUNT_H
#define ACTORWITHBANKACCOUNT_H

#include "Actor.h"

class ActorWithBankAccount : public Actor
{
public:
	ActorWithBankAccount(Money cash, AccountId accountId, Bank& bank);

	[[nodiscard]] Money GetAccountBalance() const;

	[[nodiscard]] AccountId GetAccountId() const;

	[[nodiscard]] bool SendMoney(AccountId dstAccountId, Money amount) const;

	[[nodiscard]] bool WithdrawMoney(Money amount);

	bool DepositMoney(Money amount);

	void OpenAccount();

	[[nodiscard]] Money CloseAccount();

private:
	AccountId m_accountId;
	Bank& m_bank;
};

#endif // ACTORWITHBANKACCOUNT_H
