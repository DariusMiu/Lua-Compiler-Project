#include <iostream>

#include "parser.h"
#include "scanner.h"

using namespace std;


void parser::parse(char* filename, bool Log)
{
	log = Log;
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

	if (interp == NULL && !parsetree.empty())
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
	} /**/


	inFile.close();
}

void parser::parse(char* filename, interpreter* itpr, bool Log)
{
	interp = itpr;
	parser::parse(filename, Log);
}

void parser::program(queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("program", Token(-1)));
	// find functionID()
	Token tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }
	if (tempToken.ID != 300) // int
	{ cout << "error: invalid function name" << endl; }
	tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }
	if (tempToken.ID != 5) // (
	{ cout << "error: () expected after function name" << endl; }
	tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }
	if (tempToken.ID != 6) // )
	{ cout << "error: () expected after function name" << endl; }
	parser::block(parsetree, inFile, true);
	// find end
	tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }
	if (tempToken.ID != 105) // end
	{ cout << "error: function does not contain 'end'" << endl; }

	parser::exitnode(parsetree, "program");
}
void parser::block(queue<ParseNode> *parsetree, ifstream *inFile, bool run)
{
	parser::enternode(parsetree, ParseNode("block", Token(-1)));
	parser::statement(parsetree, inFile, run);
	// determine if another block is needed
	Token tempToken = scanner::peekToken(inFile);
	if (log) { cout << "peek:"; tempToken.Print(); }
	if (tempToken.ID >= 0 && tempToken.ID != 105 && tempToken.ID != 103 && // 105='end', 103='else'
		tempToken.ID != 119) // 119='until', 
	{ if (log) { cout << endl; } parser::block(parsetree, inFile, run); }
	parser::exitnode(parsetree, "block");
}
void parser::statement(std::queue<ParseNode> *parsetree, ifstream *inFile, bool run)
{
	parser::enternode(parsetree, ParseNode("statement", Token(-1)));
	// determine statement: if / assignment / while / print / repeat
	Token tempToken = scanner::peekToken(inFile);
	if (log) { cout << "peek:"; tempToken.Print(); }
	if (tempToken.ID == 109) // if
	{ parser::if_statement(parsetree, inFile, run); }
	else if (tempToken.ID == 300) // assignment
	{ parser::assignment_statement(parsetree, inFile, run); }
	else if (tempToken.ID == 120) // while
	{ parser::while_statement(parsetree, inFile, run); }
	else if (tempToken.ID == 121) // print
	{ parser::print_statement(parsetree, inFile, run); }
	else if (tempToken.ID == 115) // repeat
	{ parser::repeat_statement(parsetree, inFile, run); }
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
void parser::if_statement(std::queue<ParseNode> *parsetree, ifstream *inFile, bool run)
{
	parser::enternode(parsetree, ParseNode("if_statement", Token(-1)));
	// find "if"
	Token tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }
	if (tempToken.ID != 109) // if
	{ cout << "error: 'if' expected" << endl; }
	
	bool ifblock = false;
	if (parser::boolean_expression(parsetree, inFile, run).ID == 118)
	{ ifblock = true; }
	if (run && interp != NULL)
	{ (*interp).ifStack.push_back(ifblock); }
	// find "then"
	tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }
	if (tempToken.ID != 117) // then
	{ cout << "error: 'then' expected" << endl; }

	if (run && interp != NULL && (*interp).ifStack.back() == true)
	{ parser::block(parsetree, inFile, true); }
	else
	{ parser::block(parsetree, inFile, false); }

	// find "else"
	tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }
	if (tempToken.ID != 103) // else
	{ cout << "error: 'else' expected" << endl; }

	if (run && interp != NULL && (*interp).ifStack.back() == true)
	{ parser::block(parsetree, inFile, false); }
	else
	{ parser::block(parsetree, inFile, true); }
	// find "end"
	tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }
	if (run && interp != NULL)
	{ (*interp).ifStack.pop_back(); }
	if (tempToken.ID != 105) // end
	{ cout << "error: 'end' expected" << endl; }
	parser::exitnode(parsetree, "if_statement");
}
void parser::while_statement(std::queue<ParseNode> *parsetree, ifstream *inFile, bool run)
{
	parser::enternode(parsetree, ParseNode("while_statement", Token(-1)));

	// find initial 'while'
	Token tempToken = scanner::getToken(inFile); // already confirmed by <statement>
	if (log) { tempToken.Print(); }
	// get boolean expression

	do
	{
		unsigned inFileLocation = (*inFile).tellg();
		bool ifblock = false;
		if (parser::boolean_expression(parsetree, inFile, run).ID == 118)
		{ ifblock = true; }
		if (run && interp != NULL)
		{
			(*interp).ifStack.push_back(ifblock);
			(*interp).loopStack.push_back(inFileLocation);
		}

		// find 'do'
		tempToken = scanner::getToken(inFile);
		if (log) { tempToken.Print(); }
		if (tempToken.ID != 102) // do
		{ cout << "error: 'do' expected" << endl; }

		if (run && interp != NULL && (*interp).ifStack.back() == true)
		{
			parser::block(parsetree, inFile, true);
			(*inFile).seekg((*interp).loopStack.back());
		}
		else
		{ parser::block(parsetree, inFile, false); }

		if (run && interp != NULL)
		{ (*interp).loopStack.pop_back(); }
	} while (run && interp != NULL && (*interp).ifStack.back() == true);
	if (run && interp != NULL)
	{ (*interp).ifStack.pop_back(); }

	// find "end"
	tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }	
	if (tempToken.ID != 105) // end
	{ cout << "error: 'end' expected" << endl; }
	parser::exitnode(parsetree, "while_statement");
}
void parser::assignment_statement(std::queue<ParseNode> *parsetree, ifstream *inFile, bool run)
{
	parser::enternode(parsetree, ParseNode("assignment_statement", Token(-1)));
	// find variable (save for later)
	Token variable = scanner::getToken(inFile);
	if (log) { variable.Print(); }
	// find assignment operator
	Token tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }
	if (tempToken.ID != 0)
	{ cout << "error: '=' expected" << endl; }
	Token value = parser::arithmetic_expression(parsetree, inFile, run);
	if (run && interp != NULL)
	{ (*interp).assignment(variable, value); }
	parser::exitnode(parsetree, "assignment_statement");
}
void parser::repeat_statement(std::queue<ParseNode> *parsetree, ifstream *inFile, bool run)
{
	parser::enternode(parsetree, ParseNode("repeat_statement", Token(-1)));
	
	// find initial 'repeat'
	Token tempToken = scanner::getToken(inFile); // already confirmed by <statement>
	if (log) { tempToken.Print(); }

	unsigned inFileLocation = (*inFile).tellg();
	// initial run
	// get block
	parser::block(parsetree, inFile, false);
	// find 'until'
	tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }
	if (tempToken.ID != 119) // until
	{ cout << "error: 'until' expected" << endl; }
	
	// find boolean expression
	bool ifblock = true; // ifblock is backwards because repeat statement STOPS when until = true
	if (parser::boolean_expression(parsetree, inFile, run).ID == 118)
	{ ifblock = false; }
	if (run && interp != NULL)
	{
		(*interp).ifStack.push_back(ifblock);
		(*interp).loopStack.push_back(inFileLocation);
	}

	while (run && interp != NULL && (*interp).ifStack.back() == true)
	{
		(*inFile).seekg((*interp).loopStack.back());

		// get block
		parser::block(parsetree, inFile, run);
		// find 'until'
		tempToken = scanner::getToken(inFile);
		if (log) { tempToken.Print(); }
		if (tempToken.ID != 119) // until
		{ cout << "error: 'until' expected" << endl; }
	
		// find boolean expression
		bool ifblock = true; // ifblock is backwards because repeat statement STOPS when until = true
		if (parser::boolean_expression(parsetree, inFile, run).ID == 118)
		{ ifblock = false; }
		(*interp).ifStack.back() = ifblock;
	}
	if (run && interp != NULL)
	{ (*interp).ifStack.pop_back(); }
	
	parser::exitnode(parsetree, "repeat_statement");
}
void parser::print_statement(std::queue<ParseNode> *parsetree, ifstream *inFile, bool run)
{
	parser::enternode(parsetree, ParseNode("print_statement", Token(-1)));

	// find initial 'repeat'
	Token tempToken = scanner::getToken(inFile); // already confirmed by <statement>
	if (log) { tempToken.Print(); }
	// find '('
	tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }
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
	Token tokenToPrint = parser::arithmetic_expression(parsetree, inFile, run);
	if (run && interp != NULL)
	{ (*interp).print(tokenToPrint); }
	// find ')'
	tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }
	if (tempToken.ID != 6)
	{
		cout << "error: () expected after <print_statement>" << endl;
		(*inFile).ignore();
		while (!(*inFile).eof() && (*inFile).peek() != '\n')
		{ (*inFile).ignore(); }
	}

	parser::exitnode(parsetree, "print_statement");
}
Token parser::boolean_expression(std::queue<ParseNode> *parsetree, ifstream *inFile, bool run)
{
	parser::enternode(parsetree, ParseNode("boolean_expression", Token(-1)));

	Token tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }
	if (tempToken.ID == 106) // false
	{ /* do nothing (Token already bool) */ }
	else if (tempToken.ID == 118) // true
	{ /* do nothing (Token already bool) */ }
	else if (tempToken.ID == -1 || tempToken.ID == -4) // error
	{ return Token(-1, ""); }
	else
	{
		Token booltokens[3];
		booltokens[0] = tempToken;
		booltokens[1] = parser::arithmetic_expression(parsetree, inFile, run);
		booltokens[2] = parser::arithmetic_expression(parsetree, inFile, run);
		if (run && interp != NULL)
		{
			tempToken = (*interp).resolveBool(booltokens);
			if (log) { tempToken.Print(); }
		}
	}

	parser::exitnode(parsetree, "boolean_expression");
	return tempToken;
}
Token parser::relative_op(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("relative_op", Token(-1)));

	Token tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }
	if (tempToken.ID >= 7 && tempToken.ID <= 12) // 7='<', 8='>', 9='~=', 10='<=', 11='==', 12='>='
	{
		// handle it
	}
	else if (tempToken.ID == -1 || tempToken.ID == -4) // error
	{ return Token(-1, ""); }
	else
	{ cout << "error: invalid <relative_op>" << endl; }

	parser::exitnode(parsetree, "relative_op");
	return tempToken;
}
Token parser::arithmetic_expression(std::queue<ParseNode> *parsetree, ifstream *inFile, bool run)
{
	parser::enternode(parsetree, ParseNode("arithmetic_expression", Token(-1)));

	Token tempToken = scanner::peekToken(inFile);
	if (log) { cout << "peek:"; tempToken.Print(); }
	if (tempToken.ID == 300) // var
	{ 
		// handle it
		tempToken = scanner::getToken(inFile);
		if (log) { tempToken.Print(); }
	}
	else if (tempToken.ID == 200) // int
	{
		// handle it
		tempToken = scanner::getToken(inFile);
		if (log) { tempToken.Print(); }
	}
	else if (tempToken.ID >= 1 && tempToken.ID <= 4) // 1='+', 2='-', 3='*', 4='/'
	{
		// handle it
		Token arithmetictokens[3];
		arithmetictokens[0] = parser::arithmetic_op(parsetree, inFile);
		arithmetictokens[1] = parser::arithmetic_expression(parsetree, inFile, run);
		arithmetictokens[2] = parser::arithmetic_expression(parsetree, inFile, run);
		if (run &&interp != NULL)
		{
			tempToken = (*interp).arithmetic(arithmetictokens);
			if (log) { tempToken.Print(); }
		}
	}
	else if (tempToken.ID == -1 || tempToken.ID == -4) // error
	{ return Token(-1, ""); }
	else
	{
		tempToken = scanner::getToken(inFile);
		if (log) { tempToken.Print(); }
		cout << "error: arithmetic_expression invalid" << endl;
		return Token(-1, "");
	}

	parser::exitnode(parsetree, "arithmetic_expression");
	return tempToken;
}
Token parser::arithmetic_op(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("arithmetic_op", Token(-1)));

	Token tempToken = scanner::getToken(inFile);
	if (log) { tempToken.Print(); }

	// handle it

	parser::exitnode(parsetree, "arithmetic_op");
	return tempToken;
}
void parser::enternode(queue<ParseNode> *parsetree, ParseNode newnode)
{
	(*parsetree).push(newnode);
	if (log) { cout << "Enter <" << (*parsetree).back().name << '>' << endl; }
}
void parser::exitnode(queue<ParseNode> *parsetree, string newnode)
{
	(*parsetree).push(ParseNode(string("exit ") + newnode, Token()));
	if (log) { cout << "</" << (*parsetree).back().name << '>' << endl; }
}
