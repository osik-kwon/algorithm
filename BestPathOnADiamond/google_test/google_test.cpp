#include "stdafx.h"
#include <gtest/gtest.h>
#include <array>
#include <vector>

class diamond_test : public testing::Test
{
protected:
	diamond_test()
	{}
	~diamond_test()
	{}
	virtual void SetUp()
	{
	}
	virtual void TearDown()
	{
	}
private:
};

class diamond
{
public:
	typedef int size_type;
	typedef int value_type;
	typedef std::vector<value_type> col_type;
	typedef std::vector<col_type> row_type;
	typedef row_type input_type;
	diamond(){}
	value_type find_path(input_type&& input)
	{
		return find_path(input);
	}

	value_type find_path(const input_type& input)
	{
		reset(input);
		value_type result = find(input, 0, 0, 0);
		return result;
	}
private:
	static const value_type unmemorized = -1;
	void reset(const input_type& input)
	{
		max_col = input.size() / 2 ;
		memo.resize(0);
		memo = input;
		for (auto& r : memo)
			for (auto& c : r)
				c = unmemorized;
	}
	const value_type find(const input_type& input, size_type r, size_type c, value_type sum)
	{
		if( input.size() > r  && c >= 0 && input[r].size() > c)
		{
			if(memo[r][c] == unmemorized)
				memo[r][c] = sum + input[r][c];
			else
				memo[r][c] = std::max(memo[r][c], sum + input[r][c]);
		}
		else
			return sum;

		if(r < max_col)
			// down or right
			sum = std::max(find(input, r+1, c, memo[r][c]), find(input, r+1, c+1, memo[r][c]));
		else
			// down or left
			sum = std::max(find(input, r+1, c, memo[r][c]), find(input, r+1, c-1, memo[r][c]));

		return sum;
	}
	input_type memo;
	size_type max_col;
};

template <int n>
struct array_traits
{
	static const diamond::value_type col = n;
	static const diamond::value_type row = 2*n - 1;
	static const diamond::value_type capacity = col * row;
	static const diamond::value_type size = n * (n+1) / 2 + n * (n-1) / 2;
};

template <typename Tratis, typename Array>
diamond::input_type array_to_vector(const Array& array)
{
	diamond::input_type input;
	
	auto& it = array.begin();
	bool dir = true; // true : increase, false : decrease
	size_t col_max = 1;
	for (size_t r = 0; r < Tratis::row; ++r)
	{
		diamond::col_type col;
 		for (size_t c = 0; c < col_max; ++c)
 		{
			col.push_back(*it);
			it++;
 		}
		input.push_back(col);

		if(dir)
			col_max++;
		else
			col_max--;
		if(col_max == Tratis::col)
			dir = false;
	}
	return input;
}

TEST_F(diamond_test, simple_test)
{
	std::array<diamond::value_type, array_traits<5>::size> input1 =
	{
		6,/*0,0,0,0,*/
		1,2,/*0,0,0,*/
		6,7,4,/*0,0,*/
		9,4,1,7,/*0,*/
		6,7,5,9,4,
		4,4,3,2,/*0,*/
		1,2,3,/*0,0,*/
		6,1,/*0,0,0,*/
		7/*,0,0,0,0*/
	};

	std::array<diamond::value_type, array_traits<5>::size> input2 =
	{
		39,/*0,0,0,0,*/
		43,16,/*0,0,0,*/
		74,94,24,/*0,0,*/
		25,76,98,62,/*0,*/
		79,58,71,67,98,
		43,55,27,44,/*0,*/
		10,96,56,/*0,0,*/
		73,31,/*0,0,0,*/
		95/*,0,0,0,0*/
	};

	diamond tester;
	EXPECT_EQ(48, tester.find_path(std::move(array_to_vector< array_traits<5> >(input1) )));
	EXPECT_EQ(664, tester.find_path(std::move(array_to_vector< array_traits<5> >(input2) )));

}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 
