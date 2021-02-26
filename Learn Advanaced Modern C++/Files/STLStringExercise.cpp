#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;
void STLStringExercise()
{
	const string brownFox{ "The Quick Brown Fox Jumps Over A Wall" };
	size_t loc = brownFox.find('l');
	if (loc != string::npos)
	{
		//I expect browmFox.size() - 3
		cout << "Found l at " << loc << " and my guess was " << brownFox.size() - 2 << endl;
	}
	loc = brownFox.rfind('l');
	if (loc != string::npos)
	{
		cout << "Found l at " << loc << " and my guess was " << brownFox.size() - 1 << endl;
	}

	string replaceable{ brownFox };
	const string toReplace{ "Quick" };
	const string replacement{ "Lazy" };
	replaceable.replace(replaceable.find(toReplace), toReplace.size(), replacement);
	cout << "The fox is now lazy. Here it is:" << replaceable << endl;
	const string fox{ "Brown Fox" };
	const string hound{ "Super Ugly Red Hound Dogs" };
	replaceable.replace(replaceable.find(fox), fox.size(), hound);
	cout << "The hounds have captured the fox : " << replaceable << endl;

}