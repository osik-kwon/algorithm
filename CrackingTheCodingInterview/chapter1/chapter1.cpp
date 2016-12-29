#include "stdafx.h"
#include <gtest/gtest.h>
#include <string>

class reverse_string_test : public testing::Test
{
protected:
	reverse_string_test()
	{}
	~reverse_string_test()
	{}
	virtual void SetUp()
	{
	}
	virtual void TearDown()
	{
	}
private:
};

class reverse_string
{
public:
	reverse_string(){}
	void reverse(char* input)
	{
		if(!input)
			return;
		const char* eos = input;
		while(*eos++);
		const size_t size = eos - input - 1 ;
		const size_t half_size = size / 2;
		for (size_t i = 0; i < half_size; ++i)
			std::swap(input[i], input[size - i - 1]);
	}

	void reverse(std::string& input)
	{
		auto first = input.begin();
		auto last = input.end();
		while ( first != last && first != --last )
		{
			std::swap(*first, *last);
			++first;
		}
	}
private:
};

void test_char_pointer(std::string&& input, std::string&& expect)
{
	reverse_string tester;
	std::string buf = input;
	char* in = (char*)buf.c_str();
	tester.reverse(in);
	EXPECT_EQ(expect, in);
}

void test_string(std::string&& input, std::string&& expect)
{
	reverse_string tester;
	std::string buf = input;
	tester.reverse(buf);
	EXPECT_EQ(expect, buf);
}

TEST_F(reverse_string_test, char_pointer_case)
{
	test_char_pointer("","");
	test_char_pointer("a","a");
	test_char_pointer("ab","ba");
	test_char_pointer("abc","cba");
	test_char_pointer("abcd","dcba");
}

TEST_F(reverse_string_test, string_case)
{
	test_string("","");
	test_string("a","a");
	test_string("ab","ba");
	test_string("abc","cba");
	test_string("abcd","dcba");
}

class unique
{
public:
	typedef char char_type;
	typedef size_t* interator;
	static const size_t alphabet_size = 26;
	unique(){}
	bool check(std::string&& input)
	{
		std::fill(begin(), end(),0);
		for (auto& it : input)
			map[to_index(it)]++;

		interator it = std::find_if(begin(), end(), [](const size_t& count){ return count > 1 ? true : false; });
		if(it != end())
			return false;
		return true;
	}
private:
	interator begin(){ return &map[0]; }
	interator end(){ return &map[alphabet_size]; }
	size_t to_index(char_type& c)
	{
		return c - 'a';
	}
	size_t map[alphabet_size];
};

TEST(unique_test, simple_case)
{
	unique tester;
	EXPECT_EQ(true,tester.check("abcd"));
	EXPECT_EQ(false,tester.check("abcc"));
	EXPECT_EQ(false,tester.check("abac"));
	EXPECT_EQ(true,tester.check(""));
}

class deflater
{
public:
	typedef char char_type;
	typedef std::string value_type;
	deflater(){}
	inline value_type deflate(const value_type& input)
	{
		return deflate(std::move(input));
	}
	value_type deflate(value_type&& input)
	{
		if(input.length() == 0)
			return input;

		//huffman coding
		size_t bit = 0;
		value_type output;
		auto prev = input.begin();
		for (auto it = input.begin(); it != input.end(); ++it)
		{
			if(*prev != *it)
			{
				output += *prev;
				output += std::to_string(bit);
				bit = 1;
			}
			else
				bit++;
			prev = it;
		}
		output += *prev;
		output += std::to_string(bit);

		if (output.length() > input.length())
			return input;
		return output;
	}
private:
};

TEST(deflater, simple_case)
{
	deflater tester;
	EXPECT_EQ("a2b2",tester.deflate("aabb"));
	EXPECT_EQ("a2b1c8a3",tester.deflate("aabccccccccaaa"));
	EXPECT_EQ("a",tester.deflate("a"));
	EXPECT_EQ("abcd",tester.deflate("abcd"));
	EXPECT_EQ("",tester.deflate(""));
}

#include <array>
template <size_t N>
class square_matrix
{
public:
	typedef int value_type;
	typedef std::array<std::array<value_type, N>, N> matrix_type;
	typedef std::pair<size_t, size_t> shift_type; // origin, count
	typedef int angle_type;
	square_matrix(matrix_type&& mat) : matrix(std::move(mat))
	{}
	void rotate_90() // solution 1
	{
		const matrix_type copy = matrix;	
		const size_t upper = N-1;
		for (size_t row = 0; row < N; ++row)
			for (size_t col = 0; col < N; ++col)
				matrix[row][col] = copy[upper - col][row];		
	}

	struct iterator 
	{
		enum direction { right = 0, down, left, up, end_of_direction};
		explicit iterator(matrix_type& mat) : 
		matrix(mat), origin_row(0), origin_col(0), row(0), col(0), dir(right)
		{}
		explicit iterator(matrix_type& mat, value_type or, value_type oc, direction di) : 
			matrix(mat), origin_row(or), origin_col(oc), row(or), col(oc), dir(di)
		{}
		iterator(iterator&& arg) : matrix(std::move(arg.matrix)),
			origin_row(std::move(arg.origin_row)), origin_col(std::move(arg.origin_col)),
			row(std::move(arg.row)), col(std::move(arg.col)), dir(std::move(arg.dir))
		{}
		iterator(const iterator& arg) : matrix(arg.matrix),
			origin_row(arg.origin_row), origin_col(arg.origin_col), row(arg.row), col(arg.col), dir(arg.dir)
		{}
		iterator& operator ++ ()
		{
			if(dir == end_of_direction)
				return *this;
			if(dir == right)
			{
				if(col == N - 1 - origin_col)
					dir = down;
				else
					col++;
			}
			if(dir == down)
			{
				if(row == N - 1 - origin_row)
					dir = left;
				else
					row++;
			}
			if(dir == left)
			{
				if(col == origin_col)
					dir = up;
				else
					col--;
			}
			if(dir == up)
			{
				if(row == origin_row)
					dir = end_of_direction;
				else
					row--;
			}
			return *this;
		}

		inline value_type& operator * ()
		{
			return matrix[row][col];
		}

		inline const value_type& operator * () const
		{
			return matrix[row][col];
		}

		friend inline bool operator == (const iterator& left, const iterator& right);
		friend inline bool operator != (const iterator& left, const iterator& right);

		direction dir;
		value_type origin_row;
		value_type origin_col;
		size_t row;
		size_t col;
		matrix_type& matrix;
	};

	inline iterator begin(value_type orogin_r, value_type orogin_c)
	{
		return iterator(matrix, orogin_r, orogin_c, iterator::right);
	}

	inline iterator end(value_type orogin_r, value_type orogin_c)
	{
		return iterator(matrix, orogin_r, orogin_c, iterator::end_of_direction);
	}

	friend inline bool operator == (const iterator& left, const iterator& right)
	{
		return left.dir == right.dir && left.col == right.col && left.row == right.row
			&& left.origin_col == right.origin_col && left.origin_row == right.origin_row;
	}

	friend inline bool operator != (const iterator& left, const iterator& right)
	{
		return left.dir != right.dir || left.col != right.col || left.row != right.row
			|| left.origin_col != right.origin_col || left.origin_row != right.origin_row;
	}

	square_matrix& operator << (shift_type&& shift)
	{
		value_type buf = 0;
		value_type prev_buf = 0;
		iterator _end = end(shift.first, shift.first);
		for (size_t shift_count = 0; shift_count < shift.second; ++shift_count)
		{		
			for(iterator it = begin(shift.first, shift.first); it != _end; ++it)
			{
				buf = *it;
				*it = prev_buf;
				prev_buf = buf;
			}
		}
		return *this;
	}

	void rotate(angle_type angle) // solution 2
	{
		// TODO: negative degree optimization - reverse iterator
		angle = angle >= 0 ? angle : angle + 360;
		const angle_type factor = (angle / 90) % 4;
		size_t rotate_size = N / 2;
		size_t shift_size = factor * (N - 1);
		for (size_t rotate_count = 0; rotate_count < rotate_size; ++rotate_count, shift_size -= 2)
			*this<<std::move(shift_type(rotate_count, shift_size));
	}
	inline const matrix_type& get() const { return matrix; }
private:
	matrix_type matrix;
};


#define case1x1(name, a) std::array<std::array<int, 1>, 1> ##name = \
{{ \
{a} \
}};

#define case2x2(name, a,b,c,d) std::array<std::array<int, 2>, 2> ##name = \
{{ \
{a, b}, \
{c, d} \
}};

#define case3x3(name, a,b,c,d,e,f,g,h,i) std::array<std::array<int, 3>, 3> ##name = \
{{ \
{a, b, c}, \
{d, e, f}, \
{g, h, i} \
}};

#define case4x4(name, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) std::array<std::array<int, 4>, 4> ##name = \
{{ \
{a, b, c, d }, \
{e, f, g, h}, \
{i, j, k, l}, \
{m, n, o, p} \
}};

class square_matrix_test : public testing::Test
{
protected:
	typedef square_matrix<1>::value_type value_type;
	square_matrix_test()
	{}
	~square_matrix_test()
	{}
	virtual void SetUp()
	{
	}
	virtual void TearDown()
	{
	}

	void test_1x1(value_type a1, value_type e1)
	{
		case1x1(input, a1);
		case1x1(expect, e1);
		square_matrix<1> tester(std::move(input));
		tester.rotate(90);
		EXPECT_EQ(expect, tester.get());
	}

	void test_2x2(value_type a1, value_type a2, value_type a3, value_type a4,
		value_type e1, value_type e2, value_type e3, value_type e4)
	{
		case2x2(input, a1, a2, a3, a4);
		case2x2(expect, e1, e2, e3, e4);
		square_matrix<2> tester(std::move(input));
		tester.rotate(90);
		EXPECT_EQ(expect, tester.get());
	}

	void test_3x3(
		value_type a1, value_type a2, value_type a3,
		value_type a4, value_type a5, value_type a6, 
		value_type a7, value_type a8, value_type a9, 
		value_type e1, value_type e2, value_type e3,
		value_type e4, value_type e5, value_type e6, 
		value_type e7, value_type e8, value_type e9
		)
	{
		case3x3(input, a1, a2, a3, a4, a5, a6, a7, a8, a9);
		case3x3(expect, e1, e2, e3, e4, e5, e6, e7, e8, e9);
		square_matrix<3> tester(std::move(input));
		tester.rotate(90);
		EXPECT_EQ(expect, tester.get());
	}

	void test_4x4(
		value_type a1, value_type a2, value_type a3, value_type a4,
		value_type a5, value_type a6, value_type a7, value_type a8,
		value_type a9, value_type a10, value_type a11, value_type a12,
		value_type a13, value_type a14, value_type a15, value_type a16,
		value_type e1, value_type e2, value_type e3, value_type e4,
		value_type e5, value_type e6, value_type e7, value_type e8,
		value_type e9, value_type e10, value_type e11, value_type e12,
		value_type e13, value_type e14, value_type e15, value_type e16
		)
	{
		case4x4(input, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
		case4x4(expect, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16);
		square_matrix<4> tester(std::move(input));
		tester.rotate(90);
		EXPECT_EQ(expect, tester.get());
	}
private:
};

TEST_F(square_matrix_test, 1x1)
{
	test_1x1(
		// input
		1,
		// expect
		1
		);
	test_1x1(
		// input
		0,
		// expect
		0
		);
}

TEST_F(square_matrix_test, 2x2)
{
	test_2x2(
		// input
		1,2,
		3,4,
		// expect
		3,1,
		4,2
		);
	
	test_2x2(
		// input
		1,0,
		0,1,
		// expect
		0,1,
		1,0
		);
	
}

TEST_F(square_matrix_test, 3x3)
{
	test_3x3(
		// input
		1,2,3,
		4,5,6,
		7,8,9,
		// expect
		7,4,1,
		8,5,2,
		9,6,3
		);
	test_3x3(
		// input
		1,0,0,
		0,1,0,
		0,0,1,
		// expect
		0,0,1,
		0,1,0,
		1,0,0
		);
	test_3x3(
		// input
		1,1,1,
		0,0,0,
		-1,-1,-1,
		// expect
		-1,0,1,
		-1,0,1,
		-1,0,1
		);
}

TEST_F(square_matrix_test, 4x4)
{
	test_4x4(
		// input
		1,2,3,4,
		5,6,7,8,
		9,10,11,12,
		13,14,15,16,
		// expect
		13,9,5,1,
		14,10,6,2,
		15,11,7,3,
		16,12,8,4
		);
}

class string_rotation_test : public testing::Test
{
protected:
	string_rotation_test()
	{}
	~string_rotation_test()
	{}
	virtual void SetUp()
	{
	}
	virtual void TearDown()
	{
	}
private:
};

class string_rotation
{
public:
	typedef std::string value_type;
	string_rotation(){}
	inline bool check(value_type&& original, value_type&& rotated)
	{
		return (rotated + rotated).find(original) != std::string::npos;
	}

	inline bool check(const value_type& original, const value_type& rotated)
	{
		return check(std::move(original), std::move(rotated));
	}
private:
};

TEST_F(string_rotation_test, positive_test)
{
	string_rotation tester;
	EXPECT_EQ(true, tester.check("", ""));
	EXPECT_EQ(true, tester.check("abacab", "abacab"));
	EXPECT_EQ(true, tester.check("abacab", "babaca"));
	EXPECT_EQ(true, tester.check("abacab", "ababac"));
	EXPECT_EQ(true, tester.check("abacab", "cababa"));
	EXPECT_EQ(true, tester.check("abacab", "acabab"));
	EXPECT_EQ(true, tester.check("abacab", "bacaba"));
	EXPECT_EQ(true, tester.check("waterbottle", "erbottlewat"));
}

TEST_F(string_rotation_test, negative_test)
{
	string_rotation tester;
	EXPECT_EQ(false, tester.check("abacab", "abxcab"));
	EXPECT_EQ(false, tester.check("abacab", "aba"));
	EXPECT_EQ(false, tester.check("abacab", "abcaba"));
	EXPECT_EQ(false, tester.check("abacab", "aaabbc"));
}

class URLify
{
public:
	typedef std::string value_type; 
	void apply(value_type& input, size_t input_size)
	{
		if(input_size == 0)
			return;
		auto plain = input.rbegin() + (input.size() - input_size);
		auto encoded = input.end();
		for (; plain != input.rend(); ++plain)
		{
			--encoded;
			if(*plain == ' ')
			{
				*encoded = '0';
				--encoded;
				*encoded = '2';
				--encoded;
				*encoded = '%';
				
			}
			else
				*encoded = *plain;
		}
	}
private:
};

class URLify_test : public testing::Test
{
protected:
	URLify_test()
	{}
	~URLify_test()
	{}
	virtual void SetUp()
	{
	}
	virtual void TearDown()
	{
	}
	void test(URLify::value_type&& input, size_t input_size,  URLify::value_type&& expect)
	{
		URLify::value_type output = input;
		URLify tester;
		tester.apply(output, input_size);
		EXPECT_EQ(expect, output);
	}
private:
};

TEST_F(URLify_test, simple)
{
	test("My John Smith    ", 13, "My%20John%20Smith");
	test("   ", 1, "%20");
	test("      ", 2, "%20%20");
}

class chapter1_test : public testing::Test
{
protected:
	chapter1_test()
	{}
	~chapter1_test()
	{}
	virtual void SetUp()
	{
	}
	virtual void TearDown()
	{
	}
private:
};

//Write an algorithm such that if an element in an MxN matrix is 0, its entire row
//and column are set to 0.

template <size_t m, size_t n, typename T = int>
class matrix
{
public:
	typedef T value_type;
	typedef std::array<value_type, n> row_type;
	typedef std::array<row_type, m> container;
	matrix()
	{
		std::fill(mat.begin(), mat.end(), 0);
	}

	matrix(const container& val) : mat(val)
	{}
	void clear_cross(size_t r, size_t c)
	{
		if(r >= m || c >= n)
			return; // TODO: exception

		for (auto& it : mat[r]) // clear row
			it = 0;

		for (size_t i = 0; i < m; ++i) // clear column
			mat[i][c] = 0;
	}
	row_type& operator [] (size_t r)
	{
		return mat[r];
	}
private:
	container mat;
};

TEST_F(chapter1_test, 1_7)
{
	matrix<3,2>::container input =
	{
		1, 2,
		3, 4,
		5, 6
	};
	matrix<3,2> mat(input);
	mat.clear_cross(1,1);
	EXPECT_EQ(1, mat[0][0]);
	EXPECT_EQ(0, mat[0][1]);
	EXPECT_EQ(0, mat[1][0]);
	EXPECT_EQ(0, mat[1][1]);
	EXPECT_EQ(5, mat[2][0]);
	EXPECT_EQ(0, mat[2][1]);
	mat.clear_cross(0,0);
	EXPECT_EQ(0, mat[0][0]);
	EXPECT_EQ(0, mat[0][1]);
	EXPECT_EQ(0, mat[1][0]);
	EXPECT_EQ(0, mat[1][1]);
	EXPECT_EQ(0, mat[2][0]);
	EXPECT_EQ(0, mat[2][1]);

	matrix<4,3>::container input2 =
	{
		1,	2,	3,
		4,	5,	6,
		7,	8,	9,
		10,	11,	12
	};
	matrix<4,3> mat2(input2);
	mat2.clear_cross(1,1);
	EXPECT_EQ(1, mat2[0][0]);
	EXPECT_EQ(0, mat2[0][1]);
	EXPECT_EQ(3, mat2[0][2]);
	EXPECT_EQ(0, mat2[1][0]);
	EXPECT_EQ(0, mat2[1][1]);
	EXPECT_EQ(0, mat2[1][2]);
	EXPECT_EQ(7, mat2[2][0]);
	EXPECT_EQ(0, mat2[2][1]);
	EXPECT_EQ(9, mat2[2][2]);
	EXPECT_EQ(10, mat2[3][0]);
	EXPECT_EQ(0, mat2[3][1]);
	EXPECT_EQ(12, mat2[3][2]);

}

#include <algorithm>
// Given two strings, write a method to decide if one is a permutation of the other.
bool b_is_permutation_of_a(const std::string& a, const std::string& b)
{
	if(a.size() != b.size())
		return false;

	std::string sorted_a = a; // n
	std::sort(sorted_a.begin(), sorted_a.end()); // n log n
	
	std::string sorted_b = b; // n
	std::sort(sorted_b.begin(), sorted_b.end()); // n log n

	return sorted_a == sorted_b; // n
}

TEST_F(chapter1_test, 1_3)
{
	EXPECT_EQ(true, b_is_permutation_of_a("abcd","acbd"));
	EXPECT_EQ(true, b_is_permutation_of_a("abcd","dabc"));
	EXPECT_EQ(true, b_is_permutation_of_a("aaaa","aaaa"));
	EXPECT_EQ(true, b_is_permutation_of_a("",""));
	EXPECT_EQ(false, b_is_permutation_of_a("abcd","bcba"));
	EXPECT_EQ(false, b_is_permutation_of_a("a ","a"));
	EXPECT_EQ(false, b_is_permutation_of_a("abcd","aabc"));
	EXPECT_EQ(false, b_is_permutation_of_a("aaaa","aaa"));
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 
