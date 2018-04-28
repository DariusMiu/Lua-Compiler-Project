#include <iostream>
#include <vector>
#include <stack>

#include "interpreter.h"
#include "parser.h"
#include "scanner.h"
#include "Token.h"

using namespace std;

void interpreter::interpret(char* filename)
{
	parser::parse(filename);
}