#pragma once

#include <iostream>

using namespace std;

template <typename T>
class Stack
{
protected:

	T* begin;
	T*end;
	int max_size;

public:

	Stack(int size = 100)
	{
		if (size < 0)
			throw 1;

		max_size = size;
		begin = new T[size];
		end = 0;
	}

	Stack(const Stack<T> &a)
	{
		max_size = a.max_size;
		begin = new T[max_size];
		end = begin + (a.end - a.begin);
		for (int i = 0; i < end - begin + 1; i++)
		{
			begin[i] = a.begin[i];
		}
	}

	void push(T elem)
	{
		if (end - begin == max_size - 1)
		{
			T* base_begin = new T[int(max_size * 1.3) + 1];
			for (int i = 0; i < end - begin + 1; i++)
			{
				base_begin[i] = begin[i];
			}
			delete[] begin;
			begin = new T[int(max_size * 1.3) + 1];
			for (int i = 0; i < max_size; i++)
				begin[i] = base_begin[i];
			delete[] base_begin;
			end = begin + max_size - 1;
			max_size = int(max_size * 1.3) + 1;

		}
		if (end != NULL)
			end++;
		else
			end = begin;
		*end = elem;
	}

	T pop()
	{
		if (end == NULL)
			throw 1;
		if (end == begin)
		{
			T res = *end;
			end = NULL;
			return res;
		}
		return *(end--);
	}

	int size() const
	{
		if (end == NULL)
			return 0;
		return end - begin + 1;
	}

	bool empty()
	{
		return end == NULL;
	}

	T& top()
	{
		if (end == NULL)
			throw 1;
		else
			return *end;
	}

	friend ostream& operator<<(ostream& out, const Stack<T> &a)
	{
		if (a.end == NULL)
		{
			out << "{}";
			return out;
		}
		out << "{ ";
		for (int i = 0; i < a.size(); i++)
		{
			out << '(' <<  a.begin[i] << ')' << ' ';
		}
		out << '}';
		return out;
	}

};