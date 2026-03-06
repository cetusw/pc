#include "ActorWithBankAccount.h"

ActorWithBankAccount::ActorWithBankAccount(const Money cash, const AccountId accountId, Bank& bank)
	: Actor(cash)
	, m_accountId(accountId)
	, m_bank(bank)
{
}

Money ActorWithBankAccount::GetAccountBalance() const
{
	return m_bank.GetAccountBalance(m_accountId);
}

AccountId ActorWithBankAccount::GetAccountId() const { return m_accountId; }

bool ActorWithBankAccount::SendMoney(const AccountId dstAccountId, const Money amount) const
{
	return m_bank.TrySendMoney(m_accountId, dstAccountId, amount);
}

bool ActorWithBankAccount::WithdrawMoney(const Money amount)
{
	if (m_bank.TryWithdrawMoney(m_accountId, amount))
	{
		AddCash(amount);
		return true;
	}
	return false;
}
bool ActorWithBankAccount::DepositMoney(const Money amount)
{
	if (SpendCash(amount))
	{
		m_bank.DepositMoney(m_accountId, amount);
		return true;
	}
	return false;
}

void ActorWithBankAccount::OpenAccount()
{
	if (m_accountId)
	{
		throw BankOperationError("Cannot open account");
	}
	m_accountId = m_bank.OpenAccount();
} // TODO: сделать так, чтобы был возможен открыт только один аккаунт одновременно ++

Money ActorWithBankAccount::CloseAccount()
{
	const Money balance = m_bank.CloseAccount(m_accountId);
	AddCash(balance);
	m_accountId = NO_ACCOUNT;
	return balance;
}