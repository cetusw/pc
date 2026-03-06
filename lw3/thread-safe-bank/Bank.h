#ifndef BANK_H
#define BANK_H
#include <stdexcept>
#include <unordered_map>

using AccountId = unsigned long long;
using Money = long long;

constexpr AccountId NO_ACCOUNT = 0;

class BankOperationError final : public std::runtime_error
{
public:
	using runtime_error::runtime_error;
};

class Bank
{
public:
	explicit Bank(Money cash);

	Bank(const Bank&) = delete;

	Bank& operator=(const Bank&) = delete;

	void SendMoney(AccountId srcAccountId, AccountId dstAccountId, Money amount);

	[[nodiscard]] bool TrySendMoney(AccountId srcAccountId, AccountId dstAccountId, Money amount);

	[[nodiscard]] Money GetCash() const;

	Money& GetAccountBalance(AccountId accountId);

	void WithdrawMoney(AccountId accountId, Money amount);

	[[nodiscard]] bool TryWithdrawMoney(AccountId accountId, Money amount);

	void DepositMoney(AccountId accountId, Money amount);

	[[nodiscard]] AccountId OpenAccount();

	[[nodiscard]] Money CloseAccount(AccountId accountId);

private:
	static void AssertAmount(Money amount);

	[[nodiscard]] AccountId GetNewAccountId();

	Money m_cash;
	std::unordered_map<AccountId, Money> m_accounts;
	AccountId m_nextAccountId = 1;
};

#endif // BANK_H
