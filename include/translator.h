#pragma once
#include <iostream>
#include <string>
#include <vector>

enum  Type { number, operation, openingbracket, closingbracket };

class Term {
protected:
	Type type;
public:
	Type GetType() { return type; }
	virtual void print() {}
};

class Number : public Term {
public:
	double value;
	Number(double x) {
		type = number;
		value = x;
	}
	void print() { std::cout << value; }
};

class Operation : public Term {
public:
	char oper;
	int priority;
	Operation(char c) {
		type = operation;
		oper = c;
		if ((oper == '+') || (oper == '-')) priority = 0;
		if ((oper == '*') || (oper == '/')) priority = 1;
	}
	int GetPriority() { return priority; }
	char GetOperation() { return oper; }
	void print() { std::cout << oper; }
};

class OpeningBracket : public Term {
public:
	char o_bracket;
	OpeningBracket() {
		type = openingbracket;
		o_bracket = '(';

	}
	void print() { std::cout << o_bracket; }
};

class ClosingBracket : public Term {
public:
	char c_bracket;
	ClosingBracket() {
		type = closingbracket;
		c_bracket = ')';
	}
	void print() { std::cout << c_bracket; }
};

class Arithmetic_expressions {
public:
	std::vector<Term*> v;
	std::string s;
	std::vector<Term*> p_notation;

	Arithmetic_expressions(std::string str) {
		s = str;
	}

	~Arithmetic_expressions() {
		for (int i = 0; i < v.size(); i++)
			delete v[i];
	}

	double calculating() { //вычисление результата по обратной польской записи
		std::vector<double> stack;
		double t1, t2, t3;
		for (int i = 0; i < p_notation.size(); i++)
			switch (p_notation[i]->GetType()) {
			case number:
				stack.push_back(((Number*)p_notation[i])->value); break;
			case operation:
				t2 = stack.back();
				stack.pop_back();
				t1 = stack.back();
				stack.pop_back();
				switch (((Operation*)(p_notation[i]))->GetOperation()) {
				case '+':
					t3 = t1 + t2;
					break;
				case '-':
					t3 = t1 - t2;
					break;
				case '*':
					t3 = t1 * t2;
					break;
				case '/':
					if (t2!=0) t3 = t1 / t2;
					else throw std::logic_error("Uncorrect division");
					break;
				}
				stack.push_back(t3);
				break;

			}
		return stack.back();
	}

	bool check_brackets() { //проверка корректности скобок
		std::vector<char> stack;
		bool f = 1;
		for (int i = 0; i < s.size()-1; i++) {
			if (s[i] == '(')
				stack.push_back(s[i]);
			if (s[i] == ')')
				if (stack.empty() != 0) {
					f = 0;
					break;
				}
				else
					stack.pop_back();
		}
		if (stack.empty() != 1) f = 0;
		return f;
	}


	bool check_decimal() { //проверка корректности .
		bool f = 1;
		if ((s[0] == '.') || (s[s.size() - 2] == '.')) f = 0;
		for (int i = 1; i < s.size() - 2; i++) {
			if (s[i] == '.') {
				if ((s[i - 1] < '0') || (s[i - 1] > '9')) f = 0;
				if ((s[i + 1] < '0') || (s[i + 1] > '9')) f = 0;
			}
		}
		return f;
	}

	int syntax_analysis() { //синтаксический анализ
		int state = 1;
		if (check_brackets() == 0) state = 0; 
		if (check_decimal() == 0) state = 0;
		if (((s[0] < '0') || (s[0] > '9')) && (s[0] != '(')) state = 0;
		if (s[s.size() - 1] != '=') state = 0;
		if (state == 1) {
			for (int i = 0; i < v.size() - 1; i++) {
				switch (v[i]->GetType()) {
				case openingbracket:
					if ((v[i + 1]->GetType() == closingbracket) || (v[i + 1]->GetType() == operation))
						state = 0; break;
				case closingbracket:
					if ((v[i + 1]->GetType() == openingbracket) || (v[i + 1]->GetType() == number))
						state = 0; break;
				case number:
					if ((v[i + 1]->GetType() == openingbracket) || (v[i + 1]->GetType() == number))
						state = 0; break;
				case operation:
					if ((v[i + 1]->GetType() == closingbracket) || (v[i + 1]->GetType() == operation))
						state = 0; break;
			    }
			}
		}
		return state;
	}

	std::vector<Term*> reverse_polish_notation() { 
		std::vector<Term*> stack;
		for (int i = 0; i < v.size(); i++) {
			switch (v[i]->GetType()) {
			case number:
				p_notation.push_back(v[i]); break;
			case operation:
				if (stack.empty() == 1) stack.push_back(v[i]);
				else
					if (stack.back()->GetType() != operation) stack.push_back(v[i]);
					else {
						while ((stack.empty() == 0) && (stack.back()->GetType() == operation) && (((Operation*)(stack.back()))->GetPriority() >= ((Operation*)(v[i]))->GetPriority())) {
							p_notation.push_back(stack.back());
							stack.pop_back();
						}
						stack.push_back(v[i]);
					}
				break;
			case openingbracket:
				stack.push_back(v[i]); break;
			case closingbracket:
				while (stack.back()->GetType() != openingbracket) {
					p_notation.push_back(stack.back());
					stack.pop_back();
				}
				stack.pop_back();
				break;
			}
		}
		while (stack.empty() == 0) {
			p_notation.push_back(stack.back());
			stack.pop_back();
		}
		return p_notation;
	}

	std::vector<Term*> lexical_analysis() { //перевод из cтроки в вектор указателей на лексемы
			bool f = 0;
			std::string N;
			for (int i = 0; i < s.size(); i++) {
				if (f == 0) {
					if ((s[i] == '+') || (s[i] == '-') || (s[i] == '*') || (s[i] == '/'))
						v.push_back(new Operation(s[i]));
					if (s[i] == '(')
						v.push_back(new OpeningBracket());
					if (s[i] == ')')
						v.push_back(new ClosingBracket());
					if ((s[i] >= '0') && (s[i] <= '9')) {
						f = 1;
						N.push_back(s[i]);
					}
					if (s[i] == '=') break;
				}
				else {
					if ((s[i] == '+') || (s[i] == '-') || (s[i] == '*') || (s[i] == '/')) {
						f = 0;
						v.push_back(new Number(std::stod(N)));
						v.push_back(new Operation(s[i]));
						N.clear();
					}
					if (s[i] == '(') {
						f = 0;
						v.push_back(new Number(std::stod(N)));
						v.push_back(new OpeningBracket());
						N.clear();
					}
					if (s[i] == ')') {
						f = 0;
						v.push_back(new Number(std::stod(N)));
						v.push_back(new ClosingBracket());
						N.clear();
					}
					if ((s[i] >= '0') && (s[i] <= '9'))
						N.push_back(s[i]);
					if (s[i] == '=') {
						v.push_back(new Number(std::stod(N)));
						break;
					}
					if (s[i] == '.') {
						int k = 0;
						for (int i = 0; i < N.size(); i++)
							if (N[i] == '.') k++;
						if (k<1)
							N.push_back(s[i]);
						else throw std::logic_error("Uncorrect syntax");
					}
				}
			}
			return v;
	}

	double GetResult() {
		lexical_analysis();
		if (syntax_analysis()!=0) {
			reverse_polish_notation();
			return calculating();
		}
		else throw std::logic_error("Uncorrect syntax");
	}
};