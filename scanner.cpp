#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <locale>
using namespace std;

class Token 
{
	public:
		int tokenID;
		string lexemeWord;

		Token (int _token, string _lexeme) : tokenID(_token), lexemeWord(_lexeme) {};
		void Print()
		{ cout << "token:" << tokenID << " lexeme:" << lexemeWord << endl; };
};

string LexemeStrings[] = {"operator", "keyword", "digit", "id"};
char operators[][2] = {{'='}, {'<'}, {'>'}, {'+'}, {'-'}, {'*'}, {'/'}, {'<','='}, {'=','='}, {'~','='}, {'>','='}};
string keywords[] = {"and", "break", "do", "else", "elseif", "end", "false", "for", "function", "if", "in", "local", "nil", "not", "or", "repeat", "return", "then", "true", "until", "while"};
char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char ids[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
ifstream inFile;

Token getToken();
bool isop(char test);

int main(int argc, char** argv)
{
	string teststring = "A = 13";
	char* filename = "test.lua";
	if (argc > 1)
	{ filename = argv[1]; }

	inFile.open(filename);
	if (!inFile)
	{
		cout << "file not found" << endl;
		cout << "to scan a file, run like so:\n./{executable} file.lua" << endl;
		return 0;
	}

	string lastLexeme;

	while (!inFile.eof())
	{
		Token tempToken = getToken();
		lastLexeme = tempToken.lexemeWord;
		tempToken.Print();
		if (!(tempToken.tokenID >= 0 && tempToken.tokenID <= 10) && !(tempToken.tokenID >= 100 && tempToken.tokenID <= 120) && tempToken.tokenID != 200 && tempToken.tokenID != 300 && tempToken.tokenID != -1)
		{ cout << "error" << endl; }
	};

	if (lastLexeme != "eof")
	{ Token (-1, "eof").Print(); }

	inFile.close();
	return 0;
}

Token getToken()
{
	stringstream ss;
	int returnToken = -4;
	char char1;
	string lex;
	while (isspace(inFile.peek()))
	{ inFile.ignore(); }

	if (inFile.eof())
	{ return Token (-1, "eof"); }

	inFile >> char1;
	char1 = (char)tolower(char1);
	ss << char1;

	//operators
	if (isop(char1))
	{
		int size = 11; //sizeof(operators) / sizeof(operators[0]);
		char char2 = inFile.peek();

		if (!isop(char2))
		{
			for (int i = 0; i < 7; i++)
			{
				if (operators[i][0] == char1)
				{
					lex = "operator";
					returnToken = i;
				}
			}
		}
		else
		{
			for (int i = 7; i < size; i++)
			{
				if (operators[i][0] == char1 && operators[i][1] == char2)
				{
					lex = "operator";
					returnToken = i;
					inFile >> char2;
					char2 = (char)tolower(char2);
					ss << char2;
				}
			}
		}
		if (returnToken < 0)
		{ returnToken = -5; }
	}
	else
	{
	{
		//digits
		int size = sizeof(digits) / sizeof(digits[0]);
		bool wasDigit = false;
		for (int i = 0; i < size; i++)
		{
			if (digits[i] == char1)
			{
				lex = "integer";
				wasDigit = true;
				returnToken = 200;
			}
		}

		char char2;
		while (wasDigit)
		{
			char2 = inFile.peek();
			wasDigit = false;
			for (int i = 0; i < size; i++)
			{
				if (digits[i] == char2)
				{
					inFile >> char2;
					char2 = (char)tolower(char2);
					ss << char2;
					wasDigit = true;
				}
			}
		};
	}
	{
		//ids&keywords
		int size = sizeof(ids) / sizeof(ids[0]);
		bool wasLetter = false;
		for (int i = 0; i < size; i++)
		{
			if (ids[i] == char1)
			{
				lex = "id";
				wasLetter = true;
				returnToken = 300;
			}
		}

		char char2;
		while (wasLetter)
		{
			char2 = inFile.peek();
			wasLetter = false;
			for (int i = 0; i < size; i++)
			{
				if (ids[i] == char2)
				{
					inFile >> char2;
					char2 = (char)tolower(char2);
					ss << char2;
					wasLetter = true;
				}
			}
		};

		size = sizeof(keywords) / sizeof(keywords[0]);
		string wholeword = ss.str();
		for (int i = 0; i < size; i++)
		{
			if (keywords[i] == wholeword)
			{
				lex = "keyword";
				wasLetter = true;
				returnToken = i + 100;
			}
		}

		if (returnToken == 300 && wholeword.length() > 1)
		{ returnToken = -3; }	// REMOVE THIS IF TO RESTORE FULL VARIABLE NAME FUNCTIONALITY
	}
	if (returnToken == 200 || returnToken == 300)
	{
		char char2;
		while (!inFile.eof())
		{
			char2 = inFile.peek();
			if (isspace(char2) || isop(char2))
			{ break; }
			else
			{
				returnToken = -2; // error
				inFile >> char2;
				char2 = (char)tolower(char2);
				ss << char2;
			}
		};
	}
	}
	
	return Token (returnToken, ss.str());
};

bool isop(char test)
{
	for (int i = 0; i < 11; i++)
	{
		if (operators[i][0] == test)
		{ return true; }
	}
	return false;
};