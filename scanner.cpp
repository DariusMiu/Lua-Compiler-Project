#include <iostream>
#include <sstream>


#include "scanner.h"
#include "Token.h"

using namespace std;

bool scanner::isop(char test)
{
	for (int i = 0; i < 10; i++)
	{
		if (operators[i][0] == test)
		{ return true; }
	}
	return false;
};

Token scanner::getToken(ifstream *inFile)
{
	stringstream ss;
	int returnToken = -4; // unknown character found
	char char1;
	string lex;
	while (isspace((*inFile).peek()))
	{ (*inFile).ignore(); }

	if ((*inFile).eof())
	{ return Token(-1, "eof"); }

	(*inFile) >> char1;
	char1 = (char)tolower(char1);
	ss << char1;

	//operators
	if (isop(char1))
	{
		int size = 13; //sizeof(operators) / sizeof(operators[0]);
		int singleCharOps = 9;	// this is COUNT of the single character operators in operators[]
		char char2 = (*inFile).peek();

		if (char1 == operators[5][0])
		{ // char1 == '('
			returnToken = 5;
		}
		else if (char1 == operators[6][0])
		{ // char1 == ')'
			returnToken = 6;
		}
		else if (!isop(char2))
		{
			for (int i = 0; i < singleCharOps; i++)
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
			for (int i = singleCharOps; i < size; i++)
			{
				if (operators[i][0] == char1 && operators[i][1] == char2)
				{
					lex = "operator";
					returnToken = i;
					(*inFile) >> char2;
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
			char2 = (*inFile).peek();
			wasDigit = false;
			for (int i = 0; i < size; i++)
			{
				if (digits[i] == char2)
				{
					(*inFile) >> char2;
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
			char2 = (*inFile).peek();
			wasLetter = false;
			for (int i = 0; i < size; i++)
			{
				if (ids[i] == char2)
				{
					(*inFile) >> char2;
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

		//if (returnToken == 300 && wholeword.length() > 1)
		//{ returnToken = -3; }	// REMOVE THIS IF TO RESTORE FULL VARIABLE NAME FUNCTIONALITY
	}
	if (returnToken == 200 || returnToken == 300)
	{
		char char2;
		while (!(*inFile).eof())
		{
			char2 = (*inFile).peek();
			if (isspace(char2) || isop(char2))
			{ break; }
			else
			{
				returnToken = -2; // error
				(*inFile) >> char2;
				char2 = (char)tolower(char2);
				ss << char2;
			}
		};
	}
	}
	
	return Token (returnToken, ss.str());
};

Token scanner::peekToken(ifstream *inFile)
{
	unsigned inFileLocation = (*inFile).tellg();
	Token tempToken = scanner::getToken(inFile);
	(*inFile).seekg(inFileLocation);
	return tempToken;
};
