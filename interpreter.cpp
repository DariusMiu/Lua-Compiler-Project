#include "interpreter.h"
#include "parser.h"

using namespace std;

void interpreter::interpret(ifstream* inFile, bool log)
{ parser::parse(inFile, this, log); }

Token interpreter::resolveBool(Token _tokens[3])
{
	if (_tokens[0].ID >= 7 && _tokens[0].ID <= 12) // 7='<', 8='>', 9='~=', 10='<=', 11='==', 12='>='
	{
		Integer tempinteger1;
		Integer tempinteger2;
		if (_tokens[1].ID == 300)
		{ tempinteger1 = *(findVariable(_tokens[1].lexeme)); }
		else if (_tokens[1].ID == 200)
		{ tempinteger1 = (Integer("", stoi(_tokens[1].lexeme))); }
		else
		{ return Token(-1, ""); }
		if (_tokens[2].ID == 300)
		{ tempinteger2 = *(findVariable(_tokens[2].lexeme)); }
		else if (_tokens[2].ID == 200)
		{ tempinteger2 = (Integer("", stoi(_tokens[2].lexeme))); }
		else
		{ return Token(-1, ""); }


		if (_tokens[0].ID == 7) // 7='<'
		{
			if (tempinteger1.value < tempinteger2.value)
			{ return Token(118, "true"); }
			else
			{ return Token(106, "false"); }
		}
		else if (_tokens[0].ID == 8) // 8='>'
		{
			if (tempinteger1.value > tempinteger2.value)
			{ return Token(118, "true"); }
			else
			{ return Token(106, "false"); }
		}
		else if (_tokens[0].ID == 9) // 9='~='
		{
			if (tempinteger1.value != tempinteger2.value)
			{ return Token(118, "true"); }
			else
			{ return Token(106, "false"); }
		}
		else if (_tokens[0].ID == 10) // 10='<='
		{
			if (tempinteger1.value <= tempinteger2.value)
			{ return Token(118, "true"); }
			else
			{ return Token(106, "false"); }
		}
		else if (_tokens[0].ID == 11) // 11='=='
		{
			if (tempinteger1.value == tempinteger2.value)
			{ return Token(118, "true"); }
			else
			{ return Token(106, "false"); }
		}
		else if (_tokens[0].ID == 12) // 12='>='
		{
			if (tempinteger1.value >= tempinteger2.value)
			{ return Token(118, "true"); }
			else
			{ return Token(106, "false"); }
		}
	}
	else
	{ return Token(-1, ""); }
}

Token interpreter::arithmetic(Token _tokens[3])
{
	if (_tokens[0].ID >= 1 && _tokens[0].ID <= 4) // 1='+', 2='-', 3='*', 4='/'
	{
		Integer tempinteger1;
		Integer tempinteger2;
		if (_tokens[1].ID == 300)
		{ tempinteger1 = *(findVariable(_tokens[1].lexeme)); }
		else if (_tokens[1].ID == 200)
		{ tempinteger1 = (Integer("", stoi(_tokens[1].lexeme))); }
		else
		{ return Token(-1, ""); }
		if (_tokens[2].ID == 300)
		{ tempinteger2 = *(findVariable(_tokens[2].lexeme)); }
		else if (_tokens[2].ID == 200)
		{ tempinteger2 = (Integer("", stoi(_tokens[2].lexeme))); }
		else
		{ return Token(-1, ""); }

		if (_tokens[0].ID == 1) // 1='+'
		{ return Token(200, to_string((tempinteger1).value + (tempinteger2).value)); }
		else if (_tokens[0].ID == 2) // 2='-'
		{ return Token(200, to_string((tempinteger1).value - (tempinteger2).value)); }
		else if (_tokens[0].ID == 3) // 3='*'
		{ return Token(200, to_string((tempinteger1).value * (tempinteger2).value)); }
		else if (_tokens[0].ID == 4) // 4='/'
		{ return Token(200, to_string((tempinteger1).value / (tempinteger2).value)); }
	}
	else
	{ return Token(-1, ""); }
}

void interpreter::assignment(Token _variable, Token _value)
{
	Integer* tempinteger1 = findVariable(_variable.lexeme);
	Integer* tempinteger2 = findVariable(_value.lexeme);
	if (tempinteger1 == NULL)
	{
		if (tempinteger2 == NULL)
		{ variables.push_back(Integer(_variable.lexeme, stoi(_value.lexeme))); }
		else
		{ variables.push_back(Integer(_variable.lexeme, (*tempinteger2).value)); }
	}
	else
	{
		if (tempinteger2 == NULL)
		{ (*tempinteger1).value = stoi(_value.lexeme); }
		else
		{ (*tempinteger1).value = (*tempinteger2).value; }
	}
}

void interpreter::print(Token TokenToPrint)
{
	if (TokenToPrint.ID == 200)
	{ cout << TokenToPrint.lexeme << endl; }
	else if (TokenToPrint.ID == 300)
	{
		Integer* tempinteger = findVariable(TokenToPrint.lexeme);
		if (tempinteger != NULL)
		{ cout << (*tempinteger).value << endl; }
		else
		{ cout << "null" << endl; }
	}
}

Integer* interpreter::findVariable(string VariableName)
{
	unsigned size = variables.size();
	for (unsigned i = 0; i < size; i++)
	{
		if (variables[i].name == VariableName)
		{ return &variables[i]; }
	}
	return NULL;
}