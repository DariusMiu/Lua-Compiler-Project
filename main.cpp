#include <iostream>
#include "interpreter.h"
#include "parser.h"
#include "scanner.h"
#include "Token.h"

using namespace std;

int main(int argc, char** argv)
{
	char* filename = "test.lua";
	if (argc > 1)
	{ filename = argv[1]; }
	
	interpreter::interpret(filename);

	return 0;
}
