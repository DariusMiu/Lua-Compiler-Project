#include <iostream>
#include "interpreter.h"

using namespace std;

int main(int argc, char** argv)
{
	char* filename = "test.lua";
	if (argc > 1)
	{ filename = argv[1]; }
	
	interpreter interp;
	interp.interpret(filename);

	return 0;
}
