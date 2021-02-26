#pragma once
#include <vector>

void algorithmWorkshop();

class AlgorithmWorkshop
{
public:
	AlgorithmWorkshop();
	void exercise3_4();
	void exercise5();
	void exercise6();
	void exercise7();
	void exercise8();
	void exercise9();
	void exercise10();
	void exercise11();
	void exercise12_13();
	void exercise22();

private:
	std::vector<uint32_t> m_randomNumbers;
	uint32_t m_max = 0;
	std::vector<uint32_t> m_sortedAscending;
	std::vector<uint32_t> m_sortedDescending;
	std::vector<uint32_t> m_shuffled;
};