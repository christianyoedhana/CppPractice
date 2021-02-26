#include <set>
#include <string>
#include <tuple>

using namespace std;

void iJustWantsThisToCompile() {
	set<string> myset;
	//C++98
	pair<set<string>::iterator, bool> result1 = myset.insert("Hello");
	//c++11. Q: There are 8 overloads with different return value. What is the selection criteria of auto?
	auto result2 = myset.insert("C++");
	//C++11. Now this blows my mind away
	set<string>::iterator iter;
	bool success = false;
	//Ooof, auto does not deduce references. Now where is that Scott Meyer auto lecture?
	//Hmm lexicographical representation of a structure. Hmmm see cppreference.com's example
	auto wow = tie(iter, success) = myset.insert("Wow");
	string name = "Cal";
}