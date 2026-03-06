#include "Bank.h"

#include <iostream>

Bank::Bank(const Money cash)
    : m_cash(cash)
{
    AssertAmount(cash);
}

void Bank::SendMoney(const AccountId srcAccountId, const AccountId dstAccountId, const Money amount)
{
    if (!TrySendMoney(srcAccountId, dstAccountId, amount))
    {
        throw BankOperationError("Insufficient funds in source account");
    }
}

bool Bank::TrySendMoney(const AccountId srcAccountId, const AccountId dstAccountId, const Money amount)
{
    AssertAmount(amount);
    Money& srcAccountBalance = GetAccountBalance(srcAccountId); // TODO: сделать GetAccountMoney, чтобы тот возвращал Money ++
    Money& dstAccountBalance = GetAccountBalance(dstAccountId);

    if (srcAccountBalance < amount)
    {
        return false;
    }

    srcAccountBalance -= amount;
    dstAccountBalance += amount;
    return true;
}

void Bank::WithdrawMoney(const AccountId accountId, const Money amount)
{
    if (!TryWithdrawMoney(accountId, amount))
    {
        throw BankOperationError("Insufficient funds in account");
    }
}

bool Bank::TryWithdrawMoney(const AccountId accountId, const Money amount)
{
    AssertAmount(amount);
	Money& accountBalance = GetAccountBalance(accountId);

    if (accountBalance < amount)
    {
        return false;
    }

    accountBalance -= amount;
    m_cash += amount;
    return true;
}

void Bank::DepositMoney(const AccountId accountId, const Money amount)
{
    AssertAmount(amount);
	Money& accountBalance = GetAccountBalance(accountId);

    if (m_cash < amount)
    {
        throw BankOperationError("Insufficient cash in bank");
    }

    m_cash -= amount;
    accountBalance += amount;
}

AccountId Bank::OpenAccount()
{
    auto newId = GetNewAccountId();
    m_accounts.emplace(newId, 0);

    return newId;
}

Money Bank::CloseAccount(const AccountId accountId)
{
	const Money& accountBalance = GetAccountBalance(accountId);
    m_accounts.erase(accountId);
    m_cash += accountBalance;

    return accountBalance;
}

void Bank::AssertAmount(const Money amount)
{
    if (amount < 0)
    {
        throw std::out_of_range("Amount can't be negative");
    }
}

Money Bank::GetCash() const
{
    return m_cash;
}

Money& Bank::GetAccountBalance(const AccountId accountId)
{
	const auto account = m_accounts.find(accountId);
	if (account == m_accounts.end())
	{
		throw BankOperationError("Account not found");
	}
	return account->second;
}

AccountId Bank::GetNewAccountId()
{
    return m_nextAccountId++;
}
