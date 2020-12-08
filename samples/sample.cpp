#include "translator.h"

int main() {
	std::cout << "Ånter arithmetic expression with = at the end:" << std::endl;
	std::string s;
	std::cin >> s;
	Arithmetic_expressions a(s);
	double rez=a.GetResult();
	std::cout << rez << std::endl;
	return 0;
}