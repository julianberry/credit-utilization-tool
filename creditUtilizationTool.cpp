#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

struct Card {
	float creditLimit;
	float currentBalance;
};


int main()
{
	float limitInput, totalCreditLimit = 0, utilPercent;
	float totalBalances = 0;

	vector<Card> cards;

	cout << "Please enter each card's credit limit and balance (press any non-number to stop): " << endl;

	//while loop to get user credit limit and balance input
	while (true)
	{
		cout << endl;
		cout << "Please enter credit limit for card " << (cards.size() + 1) << ":" << endl;

		if (!(cin >> limitInput)) {

			break;
		}
		Card c;
		c.creditLimit = limitInput;

		cout << "Balance for card " << (cards.size() + 1) << ":" << endl;
		cin >> c.currentBalance;

		cards.push_back(c);

		totalCreditLimit += c.creditLimit;
		totalBalances += c.currentBalance;
	}

	//clear fail
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	int cardCount = cards.size();

	//5% calc
	float fiveUtil = totalCreditLimit * 0.05;
	float fivePayment = totalBalances - fiveUtil;

	//10% Calc
	float tenUtil = totalCreditLimit * 0.10;
	float tenPayment = totalBalances - tenUtil;

	//25% Calc
	float twentyfiveUtil = totalCreditLimit * 0.25;
	float twentyfivePayment = totalBalances - twentyfiveUtil;

	//current util calc
	float currentUtil = (totalBalances / totalCreditLimit) * 100;

	//display summaries and check for negative values
	cout << endl;
	cout << fixed << setprecision(2);
	cout << "Total Balances: $" << totalBalances << endl;
	cout << "Total Credit Limit: $" << totalCreditLimit << endl;
	cout << "Current Utilization: " << currentUtil << "%" << endl;
	if (fivePayment <= 0) {
		cout << "Payment to be at 5%: N/A" << endl;
	}
	else {
	cout << "Payment to be at 5%: $" << fivePayment << endl;
	}
	if (tenPayment <= 0) {
		cout << "Payment to be at 10%: N/A" << endl;
	}
	else {
		cout << "Payment to be at 10%: $" << tenPayment << endl;
	}
	if (twentyfivePayment <= 0) {
		cout << "Payment to be at 25%: N/A" << endl;
	}
	else {
	cout << "Payment to be at 25%: $" << twentyfivePayment << endl;
	}

	//suggested payments calc
	vector<float> fivePayments(cardCount, 0.0);
	vector<float> tenPayments(cardCount, 0.0);
	vector<float> twentyfivePayments(cardCount, 0.0);

	auto calculatePayments = [&](float totalPaymentNeeded, vector<float>& payments) {
		vector<float> workingBalances(cardCount);
		for (size_t i = 0; i < cardCount; ++i) {
			workingBalances[i] = cards[i].currentBalance;
		}

		//payment to bring each card under 30% utilization if needed
		for (size_t i = 0; i < cardCount; ++i) {
			float maxAllowedBalance = cards[i].creditLimit * 0.3;
			if (workingBalances[i] > maxAllowedBalance) {
				float needed = workingBalances[i] - maxAllowedBalance;
				if (totalPaymentNeeded >= needed) {
					payments[i] += needed;
					workingBalances[i] -= needed;
					totalPaymentNeeded -= needed;
				}
				else {
					payments[i] += totalPaymentNeeded;
					workingBalances[i] -= totalPaymentNeeded;
					totalPaymentNeeded = 0;
					break;
				}
			}
		}

		//spread extra across cards
		if (totalPaymentNeeded > 0) {
			float remainingTotalBalance = 0;
			for (size_t i = 0; i < cardCount; ++i) {
				remainingTotalBalance += workingBalances[i];
			}

			for (size_t i = 0; i < cardCount; ++i) {
				if (remainingTotalBalance > 0) {
					float proportion = workingBalances[i] / remainingTotalBalance;
					float extraPayment = proportion * totalPaymentNeeded;
					payments[i] += extraPayment;
					workingBalances[i] -= extraPayment;
				}
			}
		}
		};

	//perform calculations for target util %
	calculatePayments(fivePayment, fivePayments);
	calculatePayments(tenPayment, tenPayments);
	calculatePayments(twentyfivePayment, twentyfivePayments);

	//display
	cout << endl;
	cout << fixed << setprecision(2);
	cout << "-----------------------------------------------------------------------------------------" << endl;
	cout << "| Card # | Credit Limit  | Current Balance | 5% Payment  | 10% Payment | 25% Payment   |" << endl;
	cout << "-----------------------------------------------------------------------------------------" << endl;

	for (size_t i = 0; i < cards.size(); i++) {
		cout << "| " << left << setw(7) << (i + 1)
			<< "| $" << right << setw(12) << cards[i].creditLimit
			<< " | $" << setw(14) << cards[i].currentBalance
			<< " | $" << setw(10) << fivePayments[i]
			<< " | $" << setw(10) << tenPayments[i]
			<< " | $" << setw(12) << twentyfivePayments[i]
			<< " |" << endl;
	}

	cout << "-----------------------------------------------------------------------------------------" << endl;

	return 0;
}//end of main