#include <iostream>
#include <string>
#include "expressions.h"

using namespace std;



int main()
{

	Expression a("3*2^(sin(1,57)+abs(1-2))");

	a.to_vector();
	a.to_postfix();
	a.calculate();
	cout << a << endl;	
	cout << a.get_result() << endl;
	cout << a.get_s_infix();

}