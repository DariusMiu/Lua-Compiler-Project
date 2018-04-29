#include <iostream>
#include "scanner.h"
#include "parser.h"
#include "interpreter.h"

using namespace std;

int main(int argc, char** argv)
{
	bool options[] = {false, false, false};
	char* filename = "test.lua";
	for (int i = 1; i < argc; i++)
	{
		string arg = argv[i];
		if (arg == "-log")
		{ options[0] = true; }
		else if (arg == "-parse")
		{ options[1] = true; }
		else if (arg == "-scan")
		{ options[2] = true; }
		/*else if (arg == "-file" && i < argc-1)
		{ filename = argv[i+1]; }*/
		else
		{ filename = argv[i]; }
	} /**/
	
	if (options[1]) // -parse
	{ parser::parse(filename, options[0]); }
	else if (options[2]) // -scan
	{ 
		// put scan loop here
	}
	else
	{
		interpreter interp;
		interp.interpret(filename, options[0]);
	}
	return 0;
}
