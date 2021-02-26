#include "BankAccount.h"
#include "Monitor.cpp"
#include <exception>
#include <string>

using Concurrency::Monitor;

//double should have enough range to contain all of the riches in the world, in any currency
template class Monitor<double>;

BankAccount::BankAccount(BankAccount::BalanceType initialBalance) : m_balance{ initialBalance } {}

//Fancy word for withdraw.
BankAccount::BalanceType BankAccount::debit(BankAccount::BalanceType amount) {
	//This is a problem with std::future
	if (amount < 0)
	{
		throw std::out_of_range("Debit amount " + std::to_string(amount) + " is less than 0");
	}
	return m_balance([=](BankAccount::BalanceType& balanceValue) {
		balanceValue -= amount;
		return amount;
		});
}
//Fancy word for deposit. I failed accounting class because I keep confusing this with debit
void BankAccount::credit(BankAccount::BalanceType amount) {
	if (amount < 0)
	{
		throw std::out_of_range("Credit amount " + std::to_string(amount) + " is less than 0");
	}
	return m_balance([=](BankAccount::BalanceType& balanceValue) {balanceValue += amount;});
}

BankAccount::BalanceType BankAccount::available() const {
	return m_balance([](BankAccount::BalanceType balanceValue){ return balanceValue; });
}


