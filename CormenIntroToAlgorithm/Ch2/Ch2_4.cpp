#include "Ch2_4.h"
#include <numeric>
#include <iterator>
#include <limits>
#include <iostream>

//Assume T contains a value
template <class T>
std::pair<typename T::size_type, typename T::size_type> maximumSubarrayBruteForce(T& data){
	using IndexType = typename T::size_type;
	using ValueType = typename T::value_type;

	ValueType greatestDiff{};
	std::pair<typename T::size_type, typename T::size_type> output;

	for (IndexType buy = 0; buy < data.size() - 1; ++buy) {
		for (IndexType sell = buy + 1; sell < data.size(); ++sell) {
			ValueType diff = data.at(sell) - data.at(buy);
			if (diff > greatestDiff) {
				greatestDiff = diff;
				output.first = buy;
				output.second = sell;
			}
		}
	}
	return output;
}

//Helper functions
/*As explained in Cormen's book, using divide and conquer, the max subarray can reside to the left, right, or straddling the split*/
//Calculate the difference between trading days.
template <class T>
T calculateDailyDiff(T& dailyPrice) {
	T output(dailyPrice);
	adjacent_difference(output.begin(), output.end(), output.begin());
	
	const T::value_type sentient{};
	output.at(0) = sentient;	//sentinel at 0, the price difference of day -1 to day 0 is 0, or sentinel value
	return output;
}

//The real work is actually handled here. Find the date ranges that straddles the middle
template<class T>
std::tuple<typename T::size_type, typename T::size_type, typename T::value_type>
maximumSubarrayCrossingMiddle(const T& diff, typename T::size_type startDate, typename T::size_type middle, typename T::size_type endDate) {
	using DateIndex = typename T::size_type;
	using Value = typename T::value_type;
	
	DateIndex maxBuy = startDate;
	DateIndex maxSell = endDate;
	
	//Take into account edge cases where the middle is either the same as startDate or endDate for 2 element comparison
	Value sumLeft = (middle > startDate)? std::numeric_limits<Value>{}.lowest() : diff[startDate];	//negative infinity = largest typed negative
	Value sum = 0;
	//Search left of middle
	for (DateIndex date = middle; date > startDate; --date) {
		sum += diff.at(date);
		if (sum > sumLeft) {
			sumLeft = sum;
			maxBuy = date;
		}
	}
	//Search right of middle
	Value sumRight = (middle + 1 < endDate)? std::numeric_limits<Value>{}.lowest() : diff[endDate];
	sum = 0;
	for (DateIndex date = middle + 1; date < endDate; ++date) {
		sum += diff.at(date);
		if (sum > sumRight) {
			sumRight = sum;
			maxSell = date;
		}
	}
	
	return std::make_tuple(maxBuy, maxSell, sumLeft + sumRight);
}


template<class T>
std::tuple<typename T::size_type, typename T::size_type, typename T::value_type>
maximumSubarrayTransformationHelper(const T& dailyDiff, typename T::size_type buyDate, typename T::size_type sellDate)
{
	using namespace std;
	using Index = typename T::size_type;
	using Value = typename T::value_type;
	struct MaxSubarray {
		Index m_buy;
		Index m_sell;
		Value m_gain;
	};

	//Each recursion will try to override this maxSubarray with its highset. this might work
	//static MaxSubarray maxSubarray;	//gain has to be the largest negative, so we can minimize loss, or just use local variables
	if (sellDate <= buyDate) return std::make_tuple(buyDate, buyDate, dailyDiff.at(buyDate));

	auto middle = (sellDate + buyDate) / 2;
	
	//Find solution left of middle
	MaxSubarray maxSubarray{};
	std::tie(maxSubarray.m_buy, maxSubarray.m_sell, maxSubarray.m_gain) = maximumSubarrayTransformationHelper(dailyDiff, buyDate, middle);

	//Find solution right of middle
	MaxSubarray right{};
	std::tie(right.m_buy, right.m_sell, right.m_gain) = maximumSubarrayTransformationHelper(dailyDiff, middle+1, sellDate);
	if (right.m_gain > maxSubarray.m_gain) {
		maxSubarray = right;
	}

	//Find solution straddling middle. The real work is done here
	MaxSubarray straddling{};
	std::tie(straddling.m_buy, straddling.m_sell, straddling.m_gain) = maximumSubarrayCrossingMiddle(dailyDiff, buyDate, middle, sellDate);
	if (straddling.m_gain > maxSubarray.m_gain) {
		maxSubarray = straddling;
	}

	return std::make_tuple( maxSubarray.m_buy, maxSubarray.m_sell, maxSubarray.m_gain );	//This should have been resolved in 2017
}

template <class T>
std::tuple<typename T::size_type, typename T::size_type, typename T::value_type> maximumSubarrayUsingTransformation(T& data) {
	using namespace std;
	T diff = calculateDailyDiff(data);
	auto result = maximumSubarrayTransformationHelper(diff, 1, diff.size() - 1);
	return { std::get<0>(result) - 1, std::get<1>(result) - 1, std::get<2>(result) };
}