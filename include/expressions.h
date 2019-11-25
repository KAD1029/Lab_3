#pragma once
#include <iostream>
#include "Stack.h"
#include <string>
#include <vector>
using namespace std;
bool is_unar_operation(char a)
{
	if (a == '+' || a == '-' || a == '*' || a == '/' || a == '(' || a == ')' || a == '^')
		return 1;
	return 0;
}

bool is_operation(string a)
{
	vector<string> operations = { "+", "-", "*", "/", "(", ")", "^", "abs", "ln", "sin", "cos" };
	for (auto x : operations)
	{
		if (x == a)
			return true;
	}
	return false;
}

bool is_number(string a)
{
	for (int i = 0; i < a.size(); i++)
	{
		if (!(a[i] <= 57 && a[i] >= 48 || a[i] == 44))
		{
			return false;
		}
	}
	return true;
}

template <typename T>
ostream& operator<<(ostream& out, vector<T> a)
{
	out << "{ ";
	for (int i = 0; i < a.size(); i++)
		out << '"' << a[i] << '"' << ' ';
	out << "}";
	return out;
}

class Operation
{
private:
	void set_priorty()
	{
		if (operation == "^")
			priority = 5;
		else if (operation == "*" || operation == "/")
			priority = 4;
		else if (operation == "+" || operation == "-")
			priority = 3;
		else if (operation == "sin" || operation == "cos" || operation == "ln" || operation == "abs")
			priority = 2;
		else if (operation == "(" || operation == ")")
			priority = 1;
		else
			throw 1;
	}
public:
	string operation;
	int priority;
	Operation()
	{
		operation = "";
		priority = 0;
	}

	Operation(string _sym)
	{
		operation = _sym;
		set_priorty();
	}

	Operation& operator=(Operation a)
	{
		operation = a.operation;
		set_priorty();
		return *this;
	}
};

ostream& operator<<(ostream& out, Operation a)
{
	out << "[" << a.operation << ", " << a.priority << "]";
	return out;
}


//-------------------------------------------------------------------------------------------------------------

class Expression
{
private:
	double string_to_int(string s)
	{
		double res = 0;
		int a, pos = -1;
		for (int i = s.size() - 1; i >= 0; i--)
		{
			if (s[i] == ',')
			{
				if (pos != -1)
					throw 1;
				pos = s.size() - i - 1;
				continue;
			}
			a = s[i] - 48;;
			for (int j = 0; j < s.size() - i - 1 - (pos != -1); j++)
			{
				a *= 10;
			}
			res += a;
		}
		for (int i = 0; i < pos; i++)
		{
			res /= 10;
		}
		return res;
	}
	
	string string_infix;
	vector<string> infix;
	vector<string> postfix;
	double res;

public:
	
	Expression() 
	{
		res = 0;
	}

	Expression(string s)
	{
		string_infix = s;
		res = 0;
	}

	Expression(vector<string> v)
	{
		infix = v;
		for (int i = 0; i < v.size(); i++)
			string_infix += v[i];
	}

	bool check_expression()
	{
		if (string_infix != "" && infix.size() == 0)
			this->to_vector();

		int status = 0;
		int n = infix.size();
		int k = 0;

		for (int i = 0; i <= n; i++)
		{
			switch (status)
			{
			case 0:
				if (i == n)
					status = 3;
				else if (infix[i] == "(")
				{
					k++;
					status = 0;
				}
				else if (infix[i][0] >= 48 && infix[i][0] <= 57 || infix[i][0] == 44)
					status = 1;
				else if (infix[i] == "abs" || infix[i] == "sin" || infix[i] == "cos" || infix[i] == "ln")
				{
					if (i < n - 1 && infix[i + 1] == "(")
					{
						k++;
						i++;
						continue;
					}
				}
				else
					status = 3;
				break;
			case 1:
				if (i == n)
					if (k == 0)
						status = 4;
					else
						status = 3;
				else if (infix[i] == ")")
					k--;
				else if (infix[i] == "+" || infix[i] == "-" || infix[i] == "*" || infix[i] == "/" || infix[i] == "^")
					status = 0;
				else
					status = 3;
				break;
			case 2:
				break;
			case 3:
				return 0;
				break;
			case 4:
				return 1;
				break;
			}
			//cout << endl << i << ' ' << status << ' '  << k << ' ' << expression << endl;
		}
		return status == 4;
	}

	vector<string> to_vector()
	{
		vector<string> res;
		string num;
		for (int i = 0; i < string_infix.size(); i++)
		{
			if (string_infix[i] <= 57 && string_infix[i] >= 48 || string_infix[i] == 44)
			{
				if (num != "" && !(num[0] <= 57 && num[0] >= 48))
				{
					res.push_back(num);
					num = "";
				}
				num += string_infix[i];
				continue;
			}
			else
			{
				if (num != "" && num[0] <= 57 && num[0] >= 48)
				{
					res.push_back(num);
					num = "";
				}
				if (is_unar_operation(string_infix[i]))
				{
					if (num != "")
					{
						res.push_back(num);
						num = "";
					}
					num += string_infix[i];
					res.push_back(num);
					num = "";
					continue;
				}
				num += string_infix[i];
			}
		}
		if (num != "")
		{
			res.push_back(num);
		}
		bool f = false;
		for (int i = 0; i < res.size(); i++)
		{
			if (!is_number(res[i]) && !is_operation(res[i]))
			{
				throw 1;
			}
		}
		infix = res;
		return res;
	}

	vector<string> to_postfix()
	{
		if (infix.size() == 0)
		{
			if (!(this->check_expression()))
				throw 1;
			this->to_vector();
		}

		vector<string> res;
		Stack<Operation> operators;
		string elem;
		int size = infix.size();
		for (int i = 0; i < size; i++)
		{
			//cout << endl << i << "_:_" << expression.top();
			elem = infix[i];
			//cout << "  Elem : " << elem;
			if (elem[0] <= 57 && elem[0] >= 48)
			{
				res.push_back(elem);
			}
			else
			{
				Operation operation(elem);
				if (operation.operation == "(")
					operators.push(operation);
				else if (operation.operation == ")")
				{
					while (operators.top().operation != "(")
					{
						elem = "";
						elem += operators.pop().operation;
						res.push_back(elem);
					}
					operators.pop();
					if (operators.top().operation == "ln" || operators.top().operation == "sin" || operators.top().operation == "cos" || operators.top().operation == "abs")
					{
						elem = "";
						elem += operators.pop().operation;
						res.push_back(elem);
					}
				}
				else if (operators.empty() || operators.top().priority < operation.priority || operation.priority == 2)
					operators.push(operation);
				else
				{
					while (!operators.empty() && operators.top().priority >= operation.priority)
					{
						elem = "";
						elem += operators.pop().operation;
						res.push_back(elem);
					}
					if (operators.empty() || operators.top().priority < operation.priority || operation.operation == "(")
						operators.push(operation);
				}
			}
			//cout << "  Res : " << res << "  Operators : " << operators;
		}
		while (!operators.empty())
		{
			elem = operators.pop().operation;
			res.push_back(elem);
		}

		postfix = res;
		return res;
	}

	double calculate()
	{
		if (postfix.size() == 0)
			this->to_postfix();

		double a;
		Stack<double> nums;
		char operat;
		for (int i = 0; i < postfix.size(); i++)
		{
			if (postfix[i][0] >= 48 && postfix[i][0] <= 57)
				nums.push(string_to_int(postfix[i]));
			else
			{
				a = nums.pop();
				switch (postfix[i][0])
				{
				case '+':
					nums.push(nums.pop() + a);
					break;
				case '-':
					nums.push(nums.pop() - a);
					break;
				case '*':
					nums.push(nums.pop() * a);
					break;
				case '/':
					nums.push(nums.pop() / a);
					break;
				case '^':
					nums.push(pow(nums.pop(), a));
					break;
				case 'l':
					nums.push(log(a));
					break;
				case 's':
					nums.push(sin(a));
					break;
				case 'c':
					nums.push(cos(a));
					break;
				case 'a':
					nums.push(abs(a));
					break;
				}
			}
		}
		res = nums.pop();
		return res;
	}

	string get_s_infix()
	{
		return string_infix;
	}

	vector<string> get_v_infix()
	{
		return infix;
	}

	vector<string> get_postfix()
	{
		return postfix;
	}

	double get_result()
	{
		return res;
	}

	friend ostream& operator<<(ostream& out, Expression a)
	{
		cout << "{ " << a.string_infix << ", " << a.infix << ", " << a.postfix << ", " << a.res << " }";
		return out;
	}

};
