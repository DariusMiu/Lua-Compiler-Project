#include <iostream>

#include "interpreter.h"
#include "parser.h"
#include "Token.h"

using namespace std;

void interpreter::interpret(char* filename)
{
	parser::parse(filename, this);
}