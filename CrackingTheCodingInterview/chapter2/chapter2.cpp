#include "stdafx.h"
#include <gtest/gtest.h>
#include <string>

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

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 
