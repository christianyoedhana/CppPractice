#include "BankAccount.h"
#include <random>
#include <vector>
#include <algorithm>
#include <iterator>
#include <future>
#include <iostream>
using namespace std;
//Demonstrate asynchronous debit/credit activity of a shared account with several direct deposits and direct payments
//Not to mention atm withdral by family members
BankAccount::BalanceType generateValue(BankAccount::BalanceType min, BankAccount::BalanceType max) {
	std::random_device seed;
	static mt19937 random(seed());
	uniform_real_distribution<BankAccount::BalanceType> generator(min, max);
	return generator(random);
}

int main()
{
	enum {
		NUM_ACTIVITIES = 4
	};

	const BankAccount::BalanceType minTransaction = 0.0f;
	const BankAccount::BalanceType maxDebit = 10000.0f;
	//Yeah this family can spend more than they earn
	const BankAccount::BalanceType maxCredit = 15000.0f;
	//Simulates several debits and credits of a dollar bank account
	const BankAccount::BalanceType startingBalance = 1000.0f;
	BankAccount familyAccount(startingBalance);
	//Direct payments
	//Generate the direct payments amounts
	vector<BankAccount::BalanceType> depositAmounts;
	generate_n(back_inserter(depositAmounts), NUM_ACTIVITIES, [=]() { return generateValue(minTransaction, maxDebit); });
	//What if? OK, who ate my exception?
	//depositAmounts[0] *= -1.0;
	//Generate withdrawl amount
	vector<BankAccount::BalanceType> withdrawls;
	generate_n(back_inserter(withdrawls), NUM_ACTIVITIES, [=]() { return generateValue(minTransaction, maxCredit); });
	//Yaay data driven
	vector<future<BankAccount::BalanceType>> debitActivities;
	//Be careful using std::async here. You want to pass the reference to the BankAccount instance, otherwise each worker threads will be operating on its own copy of
	//BankAccount instances
	transform(withdrawls.begin(), withdrawls.end(), back_inserter(debitActivities),
		[&familyAccount](BankAccount::BalanceType amt) { return std::async(launch::async, &BankAccount::debit, &familyAccount, amt); });

	vector<future<void>> creditActivities;
	transform(depositAmounts.begin(), depositAmounts.end(), back_inserter(creditActivities),
		[&familyAccount](BankAccount::BalanceType amt) { return std::async(launch::async, &BankAccount::credit, &familyAccount, amt); });
	//Test result. Compare the balance with the theoretical balance
	cout << "Starting account balance " << startingBalance << endl;
	cout << "Deposits :";
	decltype(depositAmounts)::value_type totalDeposits{};
	for (const auto amt : depositAmounts) {
		cout << " " << amt;
		totalDeposits += amt;
	}
	cout << " Total Deposits : " << totalDeposits << endl;
	cout << "Withdrawls :";
	decltype(withdrawls)::value_type totalWithdrawls{};
	for (const auto amt : withdrawls) {
		cout << " " << amt;
		totalWithdrawls += amt;
	}
	cout << " Total Withdrawl : " << totalWithdrawls << endl;
	cout << "End total amount should be :" << startingBalance + totalDeposits - totalWithdrawls << endl;
	//Sync block here, or catch std::exception
	for (auto& work : debitActivities) {
		if (work.valid()) {
			work.wait();
		}
		else {
			cout << "A debit activity becomes invalid" << endl;
		}
	}
	
	for (auto& work : creditActivities) {
		if (work.valid()) {
			work.wait();
		}
		else {
			cout << "A credit activity becomes invalid" << endl;
		}
	}
	
	cout << "Demonstrated total account value is " << familyAccount.available() << endl;
	/*//Somebody ate my exception when I turn depositAmount[0] to a negative number
	exception_ptr except = current_exception();
	try {
		if (except)
		{
			std::rethrow_exception(except);
		}
	}
	catch (const std::exception& e) {
		std::cout << "Caught exception \"" << e.what() << "\"\n";
	}
	*/
	return 0;
}