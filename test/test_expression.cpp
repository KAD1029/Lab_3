#include "expressions.h"
#include <gtest.h>


TEST(Expression, can_create_expression)
{
	ASSERT_NO_THROW(Expression a("1+2"));
}

TEST(Expression, can_create_expression_1)
{
	ASSERT_NO_THROW(Expression a({ "1", "+", "2" }));
}

TEST(Expression, check_works)
{
	Expression a("1+2*ln(2)+3^(abs(3-6+sin(4)))");
	ASSERT_EQ(a.check_expression(), true);
}

TEST(Expression, to_vector_works)
{
	Expression a("1+2^sin(2)");
	a.to_vector();
	vector<string> v = { "1", "+", "2", "^", "sin", "(", "2", ")" };
	ASSERT_EQ(a.get_v_infix(), v);
}

TEST(Expression, to_postfix_works)
{
	Expression a("1+2*3");
	a.to_postfix();
	vector<string> v = { "1", "2", "3", "*", "+" };
	ASSERT_EQ(a.get_postfix(), v);
}

TEST(Expression, calculate_works)
{
	Expression a("1+2*3");
	a.calculate();
	ASSERT_EQ(a.get_result(), 7);
}