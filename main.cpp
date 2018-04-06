#include <iostream>
#include "scanner.h"
#include "Token.h"

using namespace std;

int main(int argc, char** argv)
{
	char* filename = "test.lua";
	if (argc > 1)
	{ filename = argv[1]; }
	
	ifstream inFile;
	inFile.open(filename);
	if (!inFile)
	{
		cout << "file not found" << endl;
		cout << "to scan a file, run like so:\n./{executable} file.lua" << endl;
		return 0;
	}

	string lastLexeme;

	Token tempToken = Token (0, "");
	while (tempToken.tokenID != -1)
	{
		tempToken = scanner::getToken(&inFile);
		lastLexeme = tempToken.lexemeWord;
		//tempToken.Print();
		cout << "token:" << tempToken.tokenID << " lexeme:" << tempToken.lexemeWord << endl;
		if (!(tempToken.tokenID >= 0 && tempToken.tokenID <= 10) && !(tempToken.tokenID >= 100 && tempToken.tokenID <= 120) && tempToken.tokenID != 200 && tempToken.tokenID != 300 && tempToken.tokenID != -1)
		{ cout << "error" << endl; }
	};

	//if (lastLexeme != "eof")
	//{ Token (-1, "eof").Print(); }

	inFile.close();
	return 0;
}
