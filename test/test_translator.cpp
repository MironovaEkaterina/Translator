#include "translator.h"

#include <gtest.h>



TEST(Translator, uncorrect_symbols) {
	std::string str = "a+2=";
	Arithmetic_expressions a(str);
	ASSERT_ANY_THROW(a.GetResult());
}

TEST(Translator, two_operations) {
	std::string str = "*+2=";
	Arithmetic_expressions a(str);
	ASSERT_ANY_THROW(a.GetResult());
}

TEST(Translator, operation_and_bracket) {
	std::string str = "1+)2=";
	Arithmetic_expressions a(str);
	ASSERT_ANY_THROW(a.GetResult());
}

TEST(Translator, two_different_brackets) {
	std::string str = "(1+2)(1+3)=";
	Arithmetic_expressions a(str);
	ASSERT_ANY_THROW(a.GetResult());
}


TEST(Translator, too_many_opening_brackets) {
	std::string str = "(1+(1+2)=";
	Arithmetic_expressions a(str);
	ASSERT_ANY_THROW(a.GetResult());
}

TEST(Translator, too_many_closing_brackets) {
	std::string str = "1+(1+2))=";
	Arithmetic_expressions a(str);
	ASSERT_ANY_THROW(a.GetResult());
}

TEST(Translator, no_equal_sign) {
	std::string str = "1+1";
	Arithmetic_expressions a(str);
	ASSERT_ANY_THROW(a.GetResult());
}

TEST(Translator, do_addition) {
	std::string str = "1.1+1=";
	Arithmetic_expressions a(str);
	EXPECT_EQ(2.1,a.GetResult());
}

TEST(Translator, do_subtraction) {
	std::string str = "2-1=";
	Arithmetic_expressions a(str);
	EXPECT_EQ(1, a.GetResult());
}

TEST(Translator, do_multiplication) {
	std::string str = "1.1*2=";
	Arithmetic_expressions a(str);
	EXPECT_EQ(2.2, a.GetResult());
}

TEST(Translator, do_division) {
	std::string str = "2.2/2=";
	Arithmetic_expressions a(str);
	EXPECT_EQ(1.1, a.GetResult());
}

TEST(Translator, correct_result) {
	std::string str = "(2+4*5)/11-4*2+3.5=";
	Arithmetic_expressions a(str);
	EXPECT_EQ(-2.5, a.GetResult());
}

