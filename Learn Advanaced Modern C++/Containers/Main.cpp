#include <unordered_map>
#include <map>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	unordered_multimap<string, int> scores;
	multimap<string, int> sortedScores;
	
	scores.insert({ "Zabracadabra", 0 });
	scores.insert({ "Close", 0 });
	scores.insert({ "Graham", 78 });
	scores.insert({ "Grace", 66 });
	scores.insert({ "Graham", 66 });
	scores.insert({ "Graham", 72 });
	scores.insert({ "Hareesh", 77 });

	sortedScores.insert({ "Zabracadabra", 0 });
	sortedScores.insert({ "Close", 0 });
	sortedScores.insert({ "Graham", 78 });
	sortedScores.insert({ "Grace", 66 });
	sortedScores.insert({ "Graham", 66 });
	sortedScores.insert({ "Graham", 72 });
	sortedScores.insert({ "Hareesh", 77 });
	for (auto& it : scores)
		cout << it.first << " has a score of " << it.second << endl;
	cout << "Now for the sorted version" << endl;
	for_each(sortedScores.cbegin(), sortedScores.cend(), [](const auto& it) { cout << it.first << " has a score of " << it.second << endl; });
	return 0;
}