#include <iostream>
#include <vector>
#include <sstream>
#include <stack>

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
		cout << "error: file not found" << endl;
		cout << "to run on a file, run like so:\n./{executable} file.lua" << endl;
		return;
	}

	queue<ParseNode> parsetree;
	parser::program(&parsetree, &inFile);

	if (!parsetree.empty())
	{
		cout << "\nparsetree dump:" << endl;
		while (!parsetree.empty())
		{
			if (parsetree.front().name.substr(0, 5) == "exit ")
			{ cout << "</"; }
			else
			{ cout << '<'; }
			cout << parsetree.front().name << "> " << parsetree.front().token.lexeme <<  endl;
			parsetree.pop();
		}
	}


	inFile.close();
}


void parser::program(queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("program", Token(-1)));
	// find functionID()
	Token tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 300) // int
	{ cout << "error: invalid function name" << endl; }
	tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 5) // (
	{ cout << "error: () expected after function name" << endl; }
	tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 6) // )
	{ cout << "error: () expected after function name" << endl; }
	parser::block(parsetree, inFile);
	// find end
	tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 105) // end
	{ cout << "error: function does not contain 'end'" << endl; }

	parser::exitnode(parsetree, "program");
}
void parser::block(queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("block", Token(-1)));
	parser::statement(parsetree, inFile);
	// determine if another block is needed
	Token tempToken = scanner::peekToken(inFile);
	cout << "peek:"; tempToken.Print();
	if (tempToken.ID >= 0 && tempToken.ID != 105 && tempToken.ID != 103 && // 105='end', 103='else'
		tempToken.ID != 119) // 119='until', 
	{ cout << endl; parser::block(parsetree, inFile); }
	parser::exitnode(parsetree, "block");
}
void parser::statement(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("statement", Token(-1)));
	// determine statement: if / assignment / while / print / repeat
	Token tempToken = scanner::peekToken(inFile);
	cout << "peek:"; tempToken.Print();
	if (tempToken.ID == 109) // if
	{ parser::if_statement(parsetree, inFile); }
	else if (tempToken.ID == 300) // assignment
	{ parser::assignment_statement(parsetree, inFile); }
	else if (tempToken.ID == 120) // while
	{ parser::while_statement(parsetree, inFile); }
	else if (tempToken.ID == 121) // print
	{ parser::print_statement(parsetree, inFile); }
	else if (tempToken.ID == 115) // repeat
	{ parser::repeat_statement(parsetree, inFile); }
	else if (tempToken.ID == -1 || tempToken.ID == -4) // error
	{ return; }	
	else
	{
		cout << "error: statement invalid" << endl;
		(*inFile).ignore();
		while (!(*inFile).eof() && (*inFile).peek() != '\n')
		{ (*inFile).ignore(); }
	}

	parser::exitnode(parsetree, "statement");
}
void parser::if_statement(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("if_statement", Token(-1)));
	// find "if"
	Token tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 109) // if
	{ cout << "error: 'if' expected" << endl; }
	parser::boolean_expression(parsetree, inFile);
	// find "then"
	tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 117) // then
	{ cout << "error: 'then' expected" << endl; }
	parser::block(parsetree, inFile);
	// find "else"
	tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 103) // else
	{ cout << "error: 'else' expected" << endl; }
	parser::block(parsetree, inFile);
	// find "end"
	tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 105) // end
	{ cout << "error: 'end' expected" << endl; }
	parser::exitnode(parsetree, "if_statement");
}
void parser::while_statement(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("while_statement", Token(-1)));

	// find initial 'while'
	Token tempToken = scanner::getToken(inFile); // already confirmed by <statement>
	tempToken.Print();
	// get boolean expression
	parser::boolean_expression(parsetree, inFile);
	// find 'do'
	tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 102) // do
	{ cout << "error: 'do' expected" << endl; }
	parser::block(parsetree, inFile);
	// find "end"
	tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 105) // end
	{ cout << "error: 'end' expected" << endl; }

	parser::exitnode(parsetree, "while_statement");
}
void parser::assignment_statement(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("assignment_statement", Token(-1)));
	// find variable (save for later)
	Token variable = scanner::getToken(inFile);
	variable.Print();
	// find assignment operator
	Token tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 0)
	{ cout << "error: '=' expected" << endl; }
	parser::arithmetic_expression(parsetree, inFile);
	parser::exitnode(parsetree, "assignment_statement");
}
void parser::repeat_statement(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("repeat_statement", Token(-1)));
	
	// find initial 'repeat'
	Token tempToken = scanner::getToken(inFile); // already confirmed by <statement>
	tempToken.Print();
	// get block
	parser::block(parsetree, inFile);
	// find 'until'
	tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 119) // until
	{ cout << "error: 'until' expected" << endl; }
	// find boolean expression
	parser::boolean_expression(parsetree, inFile);
	
	parser::exitnode(parsetree, "repeat_statement");
}
void parser::print_statement(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("print_statement", Token(-1)));

	// find initial 'repeat'
	Token tempToken = scanner::getToken(inFile); // already confirmed by <statement>
	tempToken.Print();
	// find '('
	tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 5)
	{
		cout << "error: () expected after <print_statement>" << endl;
		(*inFile).ignore();
		while (!(*inFile).eof() && (*inFile).peek() != '\n')
		{ (*inFile).ignore(); }
		parser::exitnode(parsetree, "print_statement");
		return;
	}
	// find arithmetic_expression
	parser::arithmetic_expression(parsetree, inFile);
	// find ')'
	tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 6)
	{
		cout << "error: () expected after <print_statement>" << endl;
		(*inFile).ignore();
		while (!(*inFile).eof() && (*inFile).peek() != '\n')
		{ (*inFile).ignore(); }
	}

	parser::exitnode(parsetree, "print_statement");
}
void parser::boolean_expression(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("boolean_expression", Token(-1)));

	Token tempToken = scanner::peekToken(inFile);
	cout << "peek:"; tempToken.Print();
	if (tempToken.ID == 106) // false
	{
		// handle it
	}
	else if (tempToken.ID == 118) // true
	{

	}
	else if (tempToken.ID == -1 || tempToken.ID == -4) // error
	{ return; }
	else
	{
		parser::relative_op(parsetree, inFile);
		parser::arithmetic_expression(parsetree, inFile);
		parser::arithmetic_expression(parsetree, inFile);
	}

	parser::exitnode(parsetree, "boolean_expression");
}
void parser::relative_op(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("relative_op", Token(-1)));

	Token tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID >= 7 && tempToken.ID <= 12) // 7='<', 8='>', 9='~=', 10='<=', 11='==', 12='>='
	{
		// handle it
	}
	else if (tempToken.ID == -1 || tempToken.ID == -4) // error
	{ return; }
	else
	{ cout << "error: invalid <relative_op>" << endl; }

	parser::exitnode(parsetree, "relative_op");
}
void parser::arithmetic_expression(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("arithmetic_expression", Token(-1)));

	Token tempToken = scanner::peekToken(inFile);
	cout << "peek "; tempToken.Print();
	if (tempToken.ID == 300) // var
	{ 
		// handle it
		tempToken = scanner::getToken(inFile);
		tempToken.Print();
	}
	else if (tempToken.ID == 200) // int
	{
		// handle it
		tempToken = scanner::getToken(inFile);
		tempToken.Print();
	}
	else if (tempToken.ID >= 1 && tempToken.ID <= 4) // 1='+', 2='-', 3='*', 4='/'
	{
		// handle it
		parser::arithmetic_op(parsetree, inFile);
		parser::arithmetic_expression(parsetree, inFile);
		parser::arithmetic_expression(parsetree, inFile);

	}
	else if (tempToken.ID == -1 || tempToken.ID == -4) // error
	{ return; }
	else
	{
		tempToken = scanner::getToken(inFile);
		tempToken.Print();
		cout << "error: arithmetic_expression invalid" << endl;
	}

	parser::exitnode(parsetree, "arithmetic_expression");
}
void parser::arithmetic_op(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("arithmetic_op", Token(-1)));

	Token tempToken = scanner::getToken(inFile);
	tempToken.Print();

	// handle it

	parser::exitnode(parsetree, "arithmetic_op");
}
void parser::enternode(queue<ParseNode> *parsetree, ParseNode newnode)
{
	(*parsetree).push(newnode);
	cout << "Enter <" << (*parsetree).back().name << '>' << endl;
}
void parser::exitnode(queue<ParseNode> *parsetree, string newnode)
{
	(*parsetree).push(ParseNode(string("exit ") + newnode, Token()));
	cout << "</" << (*parsetree).back().name << '>' << endl;
}
