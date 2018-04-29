#include <iostream>
#include "Token.h"
#include "scanner.h"
#include "parser.h"
#include "interpreter.h"

using namespace std;

int main(int argc, char** argv)
{
	bool options[] = {false, false, false, false};
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
		else if (arg == "-h" || arg == "-help")
		{ options[3] = true; }
		/*else if (arg == "-file" && i < argc-1)
		{ filename = argv[i+1]; }*/
		else
		{ filename = argv[i]; }
	}
	
	ifstream inFile;
	inFile.open(filename);
	if (!inFile)
	{
		cout << "error: file not found" << endl;
		cout << "to run on a file, run like so:\n" << argv[0] << " file.lua" << endl;
		return 0;
	}

	if (options[1]) // -parse
	{ parser::parse(&inFile, options[0]); }
	else if (options[2]) // -scan
	{
		while (!inFile.eof())
		{
			Token tempToken = scanner::getToken(&inFile);
			tempToken.Print();
		}
	}
	else if (options[3]) // -help
	{
		cout << "to run on a file, run like so:\n" << argv[0] << " file.lua" << endl;
		cout << "-log will make the parser print all parse messages as the program runs" << endl;
		cout << "-parse will output only the parsetree (-log will enable ALL parsing messages)" << endl;
		cout << "-scan will enable only the scanner" << endl;
	}
	else
	{
		interpreter interp;
		interp.interpret(&inFile, options[0]);
	}

	inFile.close();
	return 0;
}
