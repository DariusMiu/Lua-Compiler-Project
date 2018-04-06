#include <iostream>
#include <vector>
#include <locale>

#include "parser.h"
#include "scanner.h"
#include "Token.h"

using namespace std;


void parser::parse(char* filename)
{
	ifstream inFile;
	inFile.open(filename);
	if (!inFile)
	{
		cout << "file not found" << endl;
		cout << "to scan a file, run like so:\n./{executable} file.lua" << endl;
		return;
	}

	vector<Token> sentence;

	Token tempToken = Token (0, "");
	while (tempToken.tokenID != -1)
	{
		tempToken = scanner::getToken(&inFile);
		//tempToken.Print();
		
		sentence.push_back(tempToken);

		while (isspace(inFile.peek()) || tempToken.tokenID == -1)
		{
			if (inFile.peek() == '\n' || tempToken.tokenID == -1)
			{
				// do whatever with the sentence
				
				for (vector<Token>::iterator i = sentence.begin(); i != sentence.end(); i++)
				{
					cout << "[t:" << (*i).tokenID << " l:" << (*i).lexemeWord << "]";
				}
				cout << endl;
				
				sentence.clear();
				break;
			}
			else
			{ inFile.ignore(); }
		};

		
		//if (!(tempToken.tokenID >= 0 && tempToken.tokenID <= 10) && !(tempToken.tokenID >= 100 && tempToken.tokenID <= 120) && tempToken.tokenID != 200 && tempToken.tokenID != 300 && tempToken.tokenID != -1)
		//{ cout << "error" << endl; }
	};

	//if (lastLexeme != "eof")
	//{ Token (-1, "eof").Print(); }

	inFile.close();

}