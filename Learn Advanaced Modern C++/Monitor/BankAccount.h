#pragma once
#include "Monitor.h"
//Classic example: access to a person's bank account must be synchronized. Failure to do so results in robbery.
//Bank account represent a person's simple back account. Manages the debiting and crediting activity of a person's account
//Must write a custom copy constructor to copy just m_balance's shared value.
//Copy operator will be copying the content of m_balance to a new instance of BankAccount with an unlocked mutex
class BankAccount {
private:
	mutable Concurrency::Monitor<double> m_balance;
	
public:
	using BalanceType = typename decltype(m_balance)::SharedData;
	explicit BankAccount(BalanceType initialBalance = BalanceType{});
	//Fancy word for withdraw. But how do I enforce a precondition that amount >= 0?
	BalanceType debit(BalanceType amount);
	//Fancy word for deposit. I failed accounting class because I keep confusing this with debit
	//how do I enforce precondition that amount >= 0?
	void credit(BalanceType amount);
	BalanceType available() const;
};
