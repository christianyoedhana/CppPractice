#include <vector>
#include <string>
#include <limits>
#include <iostream>
#include <algorithm>
//A table is a 2-dimensional vector.
//M is the matrix vector
//P is the matrix dimension list
//S is the partition table
using std::vector;
using std::string;

//This syntax of 'using' keyword is new to C++11. Before this I can only use typedef. The other 2 using scoping use-cases are C++98
//M_Table is the multiplication cost table. Cormen uses 'm' for the cost table
template<typename T = uint32_t> using M_Table = vector<vector<T>>;
//S_Table is the k partition index table. I prefer T=size_t
template<typename T = uint32_t> using S_Table = vector<vector<T>>;
//P_List is the p matrix dimension list of a series of matrix multiplication, with the common dimensions reduced into one entry
template<typename T = uint32_t> using P_List = vector<T>;

//MatrixChain is a code generator for optimally parenthesizing a series of matrix multiplication
//Use case:
/*
* P_List<> dimList;
* MatrixChain matrixChainCodegen(dimList);
* const vector<string> multiplicationSeries{"A","B","C","D","E","F"};
* string codegen{matrixChainCodegen(multiplicationSeries);	//no check if multiplicationSeries.length() != dimList.length() - 1;
*/
class MatrixChain {
	//m_, s_ are both 1-based table. So [0][0] is currently wasted. dim(m_) = {p_.size, p_.size}. So is s
	M_Table<> m_;
	S_Table<> s_;
	//p_ is zero-based, because I need to index p[i-1] where i = 1
	P_List<> p_;

public:
	MatrixChain() = delete;
	explicit MatrixChain(const P_List<>& p) : m_(p.size()), s_(p.size()), p_{ p } {
		std::fill(m_.begin(), m_.end(), decltype(m_)::value_type(p.size()));
		std::fill(s_.begin(), s_.end(), decltype(s_)::value_type(p.size()));
		matrixChainOrder();
	}
	string operator()(const vector<string>& multiplication) const {
		return parenthesize(multiplication, multiplication.size());
	};

private:
	string parenthesize(const vector<string>& multiplication, uint32_t j, uint32_t i = 1) const
	{
		if (i == j) {
			return multiplication.at(i-1);	//conversion to 0-based index, just for printing. The entire table is 1-based, including the entries
		}

		return string{ "(" + parenthesize(multiplication, s_[i][j], i) + " * " +
							parenthesize(multiplication, j , s_[i][j] + 1) + ")" };
	}

	void calculateCost(decltype(p_)::value_type i, decltype(p_)::value_type j) 
	{
		//That's our integral infinity
		m_[i][j] = std::numeric_limits<decltype(i)>().max();
		for (auto k = i; k < j; ++k) {
			auto cost = m_[i][k] + m_[k + 1u][j] + p_.at(i - 1) * p_.at(k) * p_.at(j);
			if (cost < m_[i][j]) {
				m_[i][j] = cost;
				s_[i][j] = k;
			}
		}
	}

	void matrixChainOrder()
	{
		using std::cout;
		using std::endl;
		//@todo how to extract the template parameter type from <vector<vector<T>>?
		for (uint32_t i = 1; i < p_.size(); ++i) {
			m_[i][i] = 0;
		}
		//This loop calculates indices diagonally to the right of the table diagonal
		for (uint32_t l = 2; l < p_.size(); ++l) {
			for (uint32_t i = 1; i < p_.size() - l + 1; ++i) {
				auto j = l + i - 1;
				calculateCost(i, j);
			}
		}
		//if n= p_size();
		//i = [1, n-1]
		//This loop definitely skips the diagonal. Just wandering why Cormen's solution actually compute i and j
		//for (uint32_t i = 1; i < p_.size() - 2; ++i) {
		//	for (uint32_t j = i + 1; j < p_.size() - 1; ++j) {
		//
		//	}
		//}
		//The loops must calculate the solution in a bottom-up fashion, starting from diagonal + 1 on each dimension
		//That's why naive loop above cannot be used, because this one calculates solution in row-major direction, which will require the 
		//leaf solution that has not been calculated yet. Now the recursive solution can use the naive row-major loop
		//but must ensure that the leaf calculation is done only once. otherwise the m_ table is useless
	}
};


int main() {
	using std::cout;
	using std::endl;
	using std::string;
	using std::vector;
	P_List<> p{ 30, 35, 15, 5, 10, 20, 25 };
	MatrixChain codegen(p);
	const vector<string> matrices{ "A", "B", "C", "D", "E", "F" };
	//static_assert((matrices.size() == p.size() - 1));
	cout << codegen(matrices) << endl;
	return 0;
}