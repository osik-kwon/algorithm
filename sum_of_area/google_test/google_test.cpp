#include "stdafx.h"
#include <gtest/gtest.h>
#include <string>
#include <array>
#include <vector>
#include <limits>


class sum_of_area_test : public testing::Test
{
protected:
	sum_of_area_test()
	{}
	~sum_of_area_test()
	{}
	virtual void SetUp()
	{
	}
	virtual void TearDown()
	{
	}
private:
};

class sum_of_area
{
public:
	// external type
	typedef int value_type;
	typedef std::array<value_type, 8> input_type;
	
	// inner type
	typedef std::pair<value_type, value_type> y_point;
	typedef std::pair<value_type, y_point> windows_x_value;
	typedef std::array<windows_x_value, 4> windows_x_type;
	class window_y_type
	{
	public:
		typedef std::vector<y_point> container;
		window_y_type(){}
		value_type update(const y_point& y)
		{			
			// find min, max in cont
			value_type min = std::numeric_limits<value_type>::max();
			value_type max = std::numeric_limits<value_type>::min();
			for (auto& it : cont)
			{
				if(min > it.first)
					min = it.first;
				if(max < it.second)
					max = it.second;
			}

			value_type height = cont.size() > 0 ? (max - min) : 0;

			// lazy update
			auto value = std::find(cont.begin(),cont.end(), y);
			if(value != cont.end()) // found
				cont.erase(value);
			else
				cont.push_back(y);

			return height;
		}
		void reset() { cont.resize(0); }
	private:
		container cont;
	};

	sum_of_area(){}
	value_type begin_event(input_type&& points)
	{
		reset();
		initialize(std::move(points));

		value_type sum = 0;
		value_type width = 0;
		value_type height = 0;
		value_type prev_x = 0;

		for (size_t i = 0; i < x_type.size(); ++i)
		{
			auto& it = x_type[i];
			if(i == 0)
			{
				prev_x = it.first;
				y_type.update(std::make_pair(it.second.first, it.second.second)); // y-min, y-max
				continue;
			}
			width = it.first - prev_x; // x
			prev_x = it.first;

			height = y_type.update(std::make_pair(it.second.first, it.second.second)); // y-min, y-max
			if(width > 0 && height > 0)
				sum += width * height;		
		}
		return sum;
	}
private:
	void reset()
	{
		y_type.reset();
	}
	void initialize(input_type&& points)
	{
		const size_t coord_pair_size = points.size() / 4;
		for ( int i = 0; i < coord_pair_size; ++i)
		{
			size_t offset = i * 4;
			value_type x0 = points[0 + offset];
			value_type y0_min = points[1 + offset];
			value_type y0_max = points[3 + offset];
			if(y0_min > y0_max)
				std::swap(y0_min, y0_max);

			value_type x1 = points[2 + offset];
			value_type y1_min = points[1 + offset];
			value_type y1_max = points[3 + offset];
			if(y1_min > y1_max)
				std::swap(y1_min, y1_max);

			size_t window_offset = i * 2;
			x_type[0 + window_offset] = std::make_pair(x0, std::make_pair(y0_min, y0_max));
			x_type[1 + window_offset] = std::make_pair(x1, std::make_pair(y1_min, y1_max));
		}

		std::sort(x_type.begin(), x_type.end(), [](const windows_x_value& left, const windows_x_value& right)
		{
			return left.first < right.first;
		});	
	}
	windows_x_type x_type;
	window_y_type y_type;
};

sum_of_area::input_type make_array(
	sum_of_area::value_type a, sum_of_area::value_type b,
	sum_of_area::value_type c, sum_of_area::value_type d,
	sum_of_area::value_type e, sum_of_area::value_type f,
	sum_of_area::value_type g, sum_of_area::value_type h
	)
{
	sum_of_area::input_type array = {a,b,c,d,e,f,g,h};
	return array;
}

TEST_F(sum_of_area_test, intersect_case)
{
	sum_of_area tester;
	EXPECT_EQ(42, tester.begin_event( make_array(-4,1,2,6,0,-1,4,3) ));
	EXPECT_EQ(28, tester.begin_event( make_array(-4,4,0,0,-2,2,2,-2) ));
	EXPECT_EQ(22, tester.begin_event( make_array(-4,0,0,-4,-3,2,-1,-5) ));
}

TEST_F(sum_of_area_test, include_case)
{
	sum_of_area tester;
	EXPECT_EQ(25, tester.begin_event( make_array(0,5,5,0,2,4,4,2) ));
	EXPECT_EQ(64, tester.begin_event( make_array(-4,4,4,-4,-2,2,2,-2) ));
	EXPECT_EQ(16, tester.begin_event( make_array(-4,0,0,-4,-2,0,0,-2) ));
}

TEST_F(sum_of_area_test, exclude_case)
{
	sum_of_area tester;
	EXPECT_EQ(9, tester.begin_event( make_array(0,5,5,4,2,4,4,2) ));
	EXPECT_EQ(10, tester.begin_event( make_array(1,3,5,2,-5,-2,-3,-5) ));
	EXPECT_EQ(8, tester.begin_event( make_array(0,2,2,0,0,0,2,-2) ));
}

TEST_F(sum_of_area_test, swap_order_case)
{
	sum_of_area tester;
	EXPECT_EQ(42, tester.begin_event( make_array(2,6,-4,1,4,3,0,-1) ));
	EXPECT_EQ(28, tester.begin_event( make_array(0,0,-4,4,2,-2,-2,2) ));
	EXPECT_EQ(22, tester.begin_event( make_array(0,-4,-4,0,-1,-5,-3,2) ));

	EXPECT_EQ(25, tester.begin_event( make_array(5,0,0,5,4,2,2,4) ));
	EXPECT_EQ(64, tester.begin_event( make_array(4,-4,-4,4,2,-2,-2,2) ));
	EXPECT_EQ(16, tester.begin_event( make_array(0,-4,-4,0,0,-2,-2,0) ));

	EXPECT_EQ(9, tester.begin_event( make_array(5,4,0,5,4,2,2,4) ));
	EXPECT_EQ(10, tester.begin_event( make_array(5,2,1,3,-3,-5,-5,-2) ));
	EXPECT_EQ(8, tester.begin_event( make_array(2,0,0,2,2,-2,0,0) ));
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 
