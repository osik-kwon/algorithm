#include "stdafx.h"
#include <gtest/gtest.h>

class chapter2_test : public testing::Test
{
protected:
	chapter2_test()
	{}
	~chapter2_test()
	{}
	virtual void SetUp()
	{
	}
	virtual void TearDown()
	{
	}
private:
};

template <typename T>
class slist
{
public:
	struct node
	{
		friend class slist;
		node() : next(nullptr)
		{}
		node(const T& val) : next(nullptr), value(val)
		{}
		const T& get() const { return value; }
		node* get_next() const { return next; }
	private:
		node* next;
		T value;
	};
	slist() : head(nullptr), tail(nullptr), count(0)
	{}
	~slist()
	{
		while (!empty())
			pop_back();
	}
	slist(const slist& rhs) : head(nullptr), tail(nullptr), count(0)
	{
		node* cur = rhs.head;
		for (size_t i = 0; i < rhs.count; ++i)
		{
			push_back(cur->value);
			cur = cur->next;
		}
	}

	slist& operator = (const slist& rhs)
	{
		while (!empty())
			pop_back();

		node* cur = rhs.head;
		for (size_t i = 0; i < rhs.count; ++i)
		{
			push_back(cur->value);
			cur = cur->next;
		}
		return *this;
	}

	inline bool empty() const { return count == 0 ; }
	inline size_t size() const { return count; }

	node* get(size_t id) const
	{
		node* cur = head;
		for (size_t i = 0; i < id; ++i)
		{
			if(cur != tail)
				cur = cur->next;
		}
		return cur;
	}

	node* back() const
	{
		if(count == 0)
			return head;
		node* cur = head;
		for (size_t i = 0; i < count - 1; ++i)
			cur = cur->next;
		return cur;
	}

	void push_back(const T& val)
	{
		if(head == tail)
		{
			head = new node(val);
			head->next = tail;
		}
		else
		{
			node* cur = head;
			while(cur->next != tail)
				cur = cur->next;

			cur->next = new node(val);
			cur->next->next = tail;
		}
		count++;
	}

	node* insert(node* next, const T& val)
	{
		node* cur = new node(val);
		if(head == next)
		{
			cur->next = head;
			head = cur;
		}
		else
		{
			node* prev = head;
			while(prev != tail)
			{
				if(prev->next == next)
					break;
				prev = prev->next;
			}

			if(prev != tail)
			{
				prev->next = new node(val);
				prev->next->next = next;
			}
			else
			{
				// TODO: runtime exception
				return tail;
			}
		}
		count++;
		return cur;
	};

	void pop_back()
	{
		if(head != tail)
		{
			node* prev_prev = head;
			node* prev = head;
			while(prev->next != tail)
			{
				prev_prev = prev;
				prev = prev_prev->next;
			}
			prev_prev->next = tail;
			delete prev;
			count--;
			if(count == 0)
				head = nullptr;
		}
	}

	void pop_front()
	{
		if(head != tail)
		{
			node* will_be_deleted = head;
			head = head->next;
			delete will_be_deleted;
			count--;
			if(count == 0)
				head = nullptr;
		}
	}

	node* erase(node* cur)
	{
		if(cur == head)
		{
			head = head->next;
			delete cur;
			count--;
			if(count == 0)
				head = nullptr;
			return cur;
		}

		node* prev = head;
		while(prev != tail)
		{
			if(prev->next == cur)
				break;
			prev = prev->next;
		}

		if(prev != tail)
		{
			prev->next = cur->next;
			delete cur;
			count--;
			if(count == 0)
				head = nullptr;
		}
		else
		{
			//TODO: runtime exception
		}
		return cur;
	};
	void make_circular(node* to, node* from)
	{
		from->next = to;
	}
private:
	node* head;
	node* tail;
	size_t count;
};

#include <list>
TEST_F(chapter2_test, 2_3)
{
	slist<int> list;
	list.push_back(1);
	list.push_back(2);
	list.insert(list.get(1), 3);

	auto back = list.back();

	slist<int> list2 = list;

	slist<int> list3;
	list3 = list2;

	list3.pop_front();
	list3.pop_front();
	list3.pop_front();

	list.erase(list.get(2));
	list.erase(list.get(1));
	list.erase(list.get(0));

	list.push_back(4);
	list.push_back(5);
	list.push_back(6);
	list.erase(list.get(0));
	list.pop_back();
	list.pop_back();
	list.push_back(7);
	list.pop_back();

	list.pop_back();
}

#include <stack>
template <typename T>
bool is_palindrome(slist<T>& list)
{
	const size_t n = list.size();
	if(n <= 1)
		return true;

	bool even = (n % 2) == 0;
	const size_t m = n/2;
	
	const size_t front_lower = 0;
	const size_t front_upper =  m - 1;
	const size_t back_lower = even ?  m : m + 1;
	const size_t back_upper = n - 1;

	// odd : 0 ~ m - 1 : m + 1 ~ n - 1
	// even : 0 ~ m - 1 : m ~ n - 1

	std::stack<T> front_stack;
	for (size_t i = front_lower ; i <= front_upper; ++i)
		front_stack.push(list.get(i)->get());		

	for (size_t i = back_lower ; i <= back_upper; ++i)
	{
		if(front_stack.top() != list.get(i)->get())
			return false;
		front_stack.pop();
	}
	return true;
}

TEST_F(chapter2_test, 2_7)
{
	slist<int> palindrome;
	palindrome.push_back(1);
	palindrome.push_back(2);
	palindrome.push_back(1);
	EXPECT_EQ(true, is_palindrome<int>(palindrome));

	slist<int> palindrome2;
	palindrome2.push_back(1);
	EXPECT_EQ(true, is_palindrome<int>(palindrome2));

	slist<int> palindrome3;
	palindrome3.push_back(1);
	palindrome3.push_back(1);
	EXPECT_EQ(true, is_palindrome<int>(palindrome3));

	slist<int> none_palindrome;
	none_palindrome.push_back(1);
	none_palindrome.push_back(2);
	none_palindrome.push_back(3);
	EXPECT_EQ(false, is_palindrome<int>(none_palindrome));

	slist<int> none_palindrome2;
	none_palindrome2.push_back(1);
	none_palindrome2.push_back(2);
	EXPECT_EQ(false, is_palindrome<int>(none_palindrome2));
}

#include <math.h>
int to_integer(const slist<int>& list)
{
	int sum = 0;
	for (size_t i = 0; i < list.size(); ++i)
		sum += list.get(i)->get() * static_cast<int>(pow(10, i));
	return sum;
}

slist<int> sum(const slist<int>& left, const slist<int>& right)
{
	int sum = to_integer(left) + to_integer(right);
	slist<int> output;
	while (sum > 0)
	{
		output.push_back(sum % 10);
		sum /= 10;
	}	
	return output;
}

TEST_F(chapter2_test, 2_5)
{
	slist<int> left;
	left.push_back(7);
	left.push_back(1);
	left.push_back(6);
	int il = to_integer(left);

	slist<int> right;
	right.push_back(5);
	right.push_back(9);
	right.push_back(2);
	int ir = to_integer(right);

	slist<int> result = sum(left, right);
	int sum = to_integer(result);
	EXPECT_EQ(912, sum);
}

// Write code to remove duplicates from an unsorted linked list.
// FOLLOW UP
// How would you solve this problem if a temporary buffer is not allowed?

template <typename T>
bool remove_duplicates(std::list<T>& list)
{
	bool removed = false;
	size_t size = list.size();
	if(size < 2)
		return removed;
	auto head = list.begin();
	auto tail = list.end();
	T target;
	while (head != tail)
	{
		target = *head;
		++head;
		for (auto cur = head; cur != tail; ++cur)
		{
			if(target == *cur)
			{
				removed = true;
				if(cur == head)
				{
					head = cur = list.erase(cur);
					if(head == tail)
						break;
				}
				else
					cur = list.erase(cur);
			}
		}
	}
	return removed;
}

TEST_F(chapter2_test, 2_1)
{
	std::list<int> input;
	input.push_back(1);
	input.push_back(2);
	input.push_back(1);
	input.push_back(2);
	EXPECT_EQ(true, remove_duplicates(input));
	auto it = input.begin();
	EXPECT_EQ(2, input.size());
	EXPECT_EQ(1, *it++);
	EXPECT_EQ(2, *it++);

	input.resize(0);
	input.push_back(1);
	EXPECT_EQ(false, remove_duplicates(input));
	it = input.begin();
	EXPECT_EQ(1, input.size());
	EXPECT_EQ(1, *it++);

	input.resize(0);
	input.push_back(1);
	input.push_back(2);
	input.push_back(3);
	EXPECT_EQ(false, remove_duplicates(input));
	it = input.begin();
	EXPECT_EQ(3, input.size());
	EXPECT_EQ(1, *it++);
	EXPECT_EQ(2, *it++);
	EXPECT_EQ(3, *it++);

	input.resize(0);
	input.push_back(1);
	input.push_back(2);
	input.push_back(1);
	input.push_back(4);
	input.push_back(2);
	input.push_back(3);
	input.push_back(3);
	EXPECT_EQ(true, remove_duplicates(input));
	it = input.begin();
	EXPECT_EQ(4, input.size());
	EXPECT_EQ(1, *it++);
	EXPECT_EQ(2, *it++);
	EXPECT_EQ(4, *it++);
	EXPECT_EQ(3, *it++);
}

// Implement an algorithm to find the kth to last element of a singly linked list.
template <typename T>
typename std::list<T>::const_iterator find_kth_to_last(const std::list<T>& list, size_t k)
{
	// use only ++ operator
	const size_t n = list.size();
	if(n == 0 || n < k)
		return list.cbegin();

	const size_t count = n - k;

	auto it = list.cbegin();
	for (size_t i = 0; i < count; ++i, ++it);
	return it;
}
TEST_F(chapter2_test, 2_2)
{
	std::list<int> input;
	input.push_back(1);
	input.push_back(2);
	input.push_back(3);
	input.push_back(4);
	auto it = input.cend();
	--it;
	--it;
	EXPECT_EQ(it, find_kth_to_last(input, 2));
	it = input.cend();
	--it;
	EXPECT_EQ(it, find_kth_to_last(input, 1));
	it = input.cend();
	--it;
	--it;
	--it;
	EXPECT_EQ(it, find_kth_to_last(input, 3));
	it = input.cend();
	--it;
	--it;
	--it;
	--it;
	EXPECT_EQ(it, find_kth_to_last(input, 4));
	// TODO: k == 5 -> ?
	input.resize(0);
	input.push_back(1);
	it = input.cend();
	--it;
	EXPECT_EQ(it, find_kth_to_last(input, 1));
}

// Write code to partition a linked list around a value x, such that all nodes less than
// x come before all nodes greater than or equal to x.
template <typename T>
typename std::list<T>::const_iterator partition_by_x(std::list<T>& list, const T& value)
{
	auto x = std::find_if(list.cbegin(), list.cend(), [&value](const T& arg){ return arg == value; });
	if(x == list.end())
		return list.begin(); // TODO: exception

	auto less = std::less<T>();
	auto greater_equal = std::greater_equal<T>();

	auto tail = list.end();
	auto front = list.begin();
	auto back = x; ++back;

	while (back != tail)
	{
		if(less(*back, *x))
		{
			list.insert(x, *back);
			back = list.erase(back);
			continue;
		}
		if(back != tail)
			back++;
	}

	while (front != x)
	{
		if(greater_equal(*front, *x))
		{
			list.push_back(*front);
			front = list.erase(front);
			continue;
		}
		if(front != tail)
			front++;
	}
	return list.cbegin();
}

#include <xfunctional>
TEST_F(chapter2_test, 2_4)
{
	auto less = std::less<int>();
	auto greater_equal = std::greater_equal<int>();

	std::list<int> input;
	input.push_back(10);
	input.push_back(2);
	input.push_back(5);
	input.push_back(1);
	input.push_back(6);
	auto output = partition_by_x(input, 5);
	
	EXPECT_EQ(true, less(*output, 5)); output++;
	EXPECT_EQ(true, less(*output, 5)); output++;
	EXPECT_EQ(true, *output == 5); output++;
	EXPECT_EQ(true, greater_equal(*output, 5)); output++;
	EXPECT_EQ(true, greater_equal(*output, 5)); output++;

	input.resize(0);
	input.push_back(5);
	input.push_back(1);
	input.push_back(2);
	input.push_back(3);
	output = partition_by_x(input, 5);

	EXPECT_EQ(true, less(*output, 5)); output++;
	EXPECT_EQ(true, less(*output, 5)); output++;
	EXPECT_EQ(true, less(*output, 5)); output++;
	EXPECT_EQ(true, *output == 5); output++;

	input.resize(0);
	input.push_back(10);
	input.push_back(9);
	input.push_back(8);
	input.push_back(5);
	output = partition_by_x(input, 5);

	EXPECT_EQ(true, *output == 5); output++;
	EXPECT_EQ(true, greater_equal(*output, 5)); output++;
	EXPECT_EQ(true, greater_equal(*output, 5)); output++;
	EXPECT_EQ(true, greater_equal(*output, 5)); output++;
	
	input.resize(0);
	input.push_back(10);
	input.push_back(5);
	input.push_back(2);
	input.push_back(5);
	input.push_back(6);
	output = partition_by_x(input, 5);

	EXPECT_EQ(true, less(*output, 5)); output++;
	EXPECT_EQ(true, *output == 5); output++;
	EXPECT_EQ(true, greater_equal(*output, 5)); output++;
	EXPECT_EQ(true, greater_equal(*output, 5)); output++;
	EXPECT_EQ(true, greater_equal(*output, 5)); output++;

	input.resize(0);
	input.push_back(5);
	output = partition_by_x(input, 5);
	EXPECT_EQ(true, *output == 5); output++;

	input.resize(0);
	input.push_back(1);
	output = partition_by_x(input, 5);
	EXPECT_EQ(true, *output == 1); output++;

	input.resize(0);
	output = partition_by_x(input, 5);
	EXPECT_EQ(input.end(), output);
}

//Given a circular linked list, implement an algorithm which returns the node at
//	the beginning of the loop.
//	DEFINITION
//	Circular linked list: A (corrupt) linked list in which a node's next pointer points
//	to an earlier node, so as to make a loop in the linked list.
//	EXAMPLE
//Input: A - > B - > C - > D - > E - > C [the same C as earlier]
//Output: C

typedef slist<int>::node node;
node* find_beginning_of_the_loop(const slist<int>& list)
{
	size_t size = list.size();
	return list.get(size);
}

TEST_F(chapter2_test, 2_6)
{
	slist<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.make_circular(list.get(1), list.get(2)); // 1 -> 2-> 3-> 2
	
	EXPECT_EQ(2,find_beginning_of_the_loop(list)->get());
}


int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 
