#ifndef interpreter_h
#define interpreter_h

#include <iostream>
#include <string>
#include <vector>
#include "Integer.h"
#include "Token.h"

class interpreter
{
	public:
		void interpret(std::ifstream* inFile, bool log);
		void assignment(Token _variable, Token _value);
		void removeInteger(std::string _name);
		Token resolveBool(Token _tokens[3]);
		Token arithmetic(Token _tokens[3]);
		void print(Token TokenToPrint);
		std::vector<bool> ifStack;
		std::vector<int> loopStack;
	protected:
		std::vector<Integer> variables;
		Integer* findVariable(std::string VariableName);
};

#endif