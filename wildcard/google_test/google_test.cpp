#include "stdafx.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <map>

class wildcard
{
public:
	typedef std::string value_type;
	typedef std::vector<value_type> result_type;
	typedef char char_type;
	static const char_type question;
	static const char_type zero;
	static const char_type one;
	struct wildcard_digits
	{
		typedef std::pair<size_t, char_type> pair;
		typedef std::vector<pair> container_type;
		wildcard_digits(){}
		inline size_t size() const
		{
			return static_cast<size_t>( pow(2.0, static_cast<double>( _container.size() ) ) );
		}

		inline void merge(value_type& input) const
		{
			for (auto it : _container)
				input[it.first] = it.second;
		}

		wildcard_digits& operator ++ (int) // ex> 00 -> 01 -> 10 -> 11
		{
			bool carry = true;
			auto invert = [&carry](wildcard::char_type& prev) -> bool // true : carry set, false : carry off
			{
				if(prev == zero)
				{
					prev = one;
					carry = false;
					return carry;
				}
				prev = zero;
				carry = true;
				return carry;
			};

			for (auto it = _container.rbegin(); it != _container.rend(); ++it)
			{
				if(carry && !invert(it->second))
					break;
			}
			return *this;
		}
		container_type _container;
	};

	// N : input string length, M : wildcard(?) count
	static result_type produce(const value_type& input) // N + 2^M
	{
		result_type result;
		wildcard_digits wildcards;
		value_type output(input);
		for ( auto it = output.begin(); it != output.end(); ++it) // N
			if(is_wildcard(*it))
				wildcards._container.push_back( std::make_pair( std::distance(output.begin(), it), zero ) );

		for (size_t i = 0; i < wildcards.size(); ++i) // 2^M
		{
			wildcards.merge(output);
			result.push_back(output);
			wildcards++;
		}
		return result;
	}
private:
	static inline bool is_wildcard(const char_type token)
	{
		return token == question;
	}
};

const wildcard::char_type wildcard::question = '?';
const wildcard::char_type wildcard::zero = '0';
const wildcard::char_type wildcard::one = '1';

class wildcard_test : public testing::Test
{
protected:
	typedef std::multimap<wildcard::value_type, wildcard::value_type> solutions_type;
	wildcard_test()
	{}
	~wildcard_test()
	{}
	virtual void SetUp()
	{
		// test cases
		using namespace std;
		solutions.emplace(make_pair("?", "0"));
		solutions.emplace(make_pair("?", "1"));

		solutions.emplace(make_pair("??", "00"));
		solutions.emplace(make_pair("??", "01"));
		solutions.emplace(make_pair("??", "10"));
		solutions.emplace(make_pair("??", "11"));

		solutions.emplace(make_pair("01?", "010"));
		solutions.emplace(make_pair("01?", "011"));

		solutions.emplace(make_pair("01?0?", "01000"));
		solutions.emplace(make_pair("01?0?", "01001"));
		solutions.emplace(make_pair("01?0?", "01100"));
		solutions.emplace(make_pair("01?0?", "01101"));

		solutions.emplace(make_pair("0??0?1", "000001"));
		solutions.emplace(make_pair("0??0?1", "000011"));
		solutions.emplace(make_pair("0??0?1", "001001"));
		solutions.emplace(make_pair("0??0?1", "001011"));
		solutions.emplace(make_pair("0??0?1", "010001"));
		solutions.emplace(make_pair("0??0?1", "010011"));
		solutions.emplace(make_pair("0??0?1", "011001"));
		solutions.emplace(make_pair("0??0?1", "011011"));

		solutions.emplace(make_pair("??11", "0011"));
		solutions.emplace(make_pair("??11", "0111"));
		solutions.emplace(make_pair("??11", "1011"));
		solutions.emplace(make_pair("??11", "1111"));

	}
	virtual void TearDown()
	{
	}
	void run_test(wildcard::value_type&& input, wildcard::result_type&& expect)
	{
		EXPECT_EQ(expect.size(), solutions.count(input));
		for ( auto& input_it : expect)
		{
			bool found = false;
			const auto& range = solutions.equal_range(input);
			if(range.first != range.second)
			{
				for (auto range_it = range.first; range_it != range.second; ++range_it)
				{
					if(input_it == range_it->second)
						found = true;
				}
			}
			EXPECT_TRUE(found);
		}
	}
private:
	solutions_type solutions;
};

#define RUN_TEST(input) run_test(input, std::move(wildcard::produce(input)));


TEST_F(wildcard_test, positive_test)
{
	RUN_TEST("?");
	RUN_TEST("??");
	RUN_TEST("01?");
	RUN_TEST("01?0?");
	RUN_TEST("0??0?1");
	RUN_TEST("??11");
}

TEST_F(wildcard_test, negative_test)
{
	// TODO:
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 
