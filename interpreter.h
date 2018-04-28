#ifndef interpreter_h
#define interpreter_h

#include <string>
#include <vector>
#include "Integer.h"


class interpreter
{
	public:
		void interpret(char* filename);
		std::vector<Integer> variables;
		if_statement();
		while_statement();
		assignment_statement();
		repeat_statement();
		print_statement();
		boolean_expression();
		relative_op();
		arithmetic_expression();
		arithmetic_op();
};

#endif