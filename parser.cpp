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
		cout << "file not found" << endl;
		cout << "to scan a file, run like so:\n./{executable} file.lua" << endl;
		return;
	}

	queue<ParseNode> parsetree;

	parser::program(&parsetree, &inFile);

	/*stringstream ss;
	stack <string> opstack;
	
	vector<Token> sentence;


	Token tempToken = Token ();
	while (tempToken.ID != -1)
	{
		tempToken = scanner::getToken(&inFile);
		//tempToken.Print();
		
		sentence.push_back(tempToken);


		/*if (tempToken.ID >= 100 && tempToken.ID <= 120)
		{ // keyword
			if (tempToken.ID == 102 || tempToken.ID == 120 || tempToken.ID == 103 || tempToken.ID == 109 || // do while else if
				tempToken.ID == 117 || tempToken.ID == 115) // then repeat
			{ opstack.push(tempToken.lexeme); }
			else if (tempToken.ID == 105)
			{
				if (opstack.top() != "while")
				{ cout << "error: unexpected ')'" << endl; }
				if (opstack.top() != "while")
				{ cout << "error: unexpected ')'" << endl; }
				opstack.pop();

			}
		}
		else/** / if (tempToken.ID >= 000 && tempToken.ID <= 012)
		{ // operators
			if (tempToken.ID == 007)
			{ opstack.push(tempToken.lexeme); }
			else if (tempToken.ID == /*00* /8)
			{
				if (opstack.empty() || opstack.top() != "(")
				{ cout << "error: unexpected ')'" << endl; }
				if (!opstack.empty()) 
				{ opstack.pop(); }
			}
		}

		while (isspace(inFile.peek()) || tempToken.ID == -1)
		{
			if (inFile.peek() == '\n' || tempToken.ID == -1)
			{ // do whatever with the sentence
				for (vector<Token>::iterator i = sentence.begin(); i != sentence.end(); i++)
				{ cout << /** /"[t:" << (*i).ID << " l:" << /** /(*i).lexeme << ' '; }
				cout << endl;
				
				sentence.clear();
				break;
			}
			else
			{ inFile.ignore(); }
		};

		//if (!(tempToken.ID >= 0 && tempToken.ID <= 10) && !(tempToken.ID >= 100 && tempToken.ID <= 120) && tempToken.ID != 200 && tempToken.ID != 300 && tempToken.ID != -1)
		//{ cout << "error" << endl; }
	};

	if (!opstack.empty())
	{
		cout << "stack dump:" << endl;
		while (!opstack.empty())
		{
			cout << opstack.top() << ' ';
			opstack.pop();
		}
		cout << endl;
	}/**/
	if (!parsetree.empty())
	{
		cout << "parsetree dump:" << endl;
		while (!parsetree.empty())
		{
			cout << '<' << parsetree.front().name << "> " << parsetree.front().token.lexeme <<  endl;
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
	if (tempToken.ID != 300)
	{ cout << "error: invalid function name" << endl; }
	tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 7)
	{ cout << "error: () expected after function name" << endl; }
	tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 8)
	{ cout << "error: () expected after function name" << endl; }
	parser::block(parsetree, inFile);
	// find end
	tempToken = scanner::getToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 5)
	{ cout << "error: function does not contain end" << endl; }

	parser::exitnode(parsetree, "program");
}
void parser::block(queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("block", Token(-1)));
	parser::statement(parsetree, inFile);
	// determine if another block is needed
	Token tempToken = scanner::peekToken(inFile);
	tempToken.Print();
	if (tempToken.ID != 5)
	{ parser::block(parsetree, inFile); }
	parser::exitnode(parsetree, "block");
}
void parser::statement(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("statement", Token(-1)));
	// determine statement: if / assignment / while / print / repeat
	Token tempToken = scanner::peekToken(inFile);
	tempToken.Print();
	if (tempToken.ID == 109) // if
	{ parser::if_statement(parsetree, inFile); }
	else if (tempToken.ID == 300) // assignment
	{ parser::assignment_statement(parsetree, inFile); }
	else if (tempToken.ID == 102) // do
	{ parser::while_statement(parsetree, inFile); }
	else if (tempToken.ID == 121) // print
	{ parser::print_statement(parsetree, inFile); }
	else if (tempToken.ID == 115) // repeat
	{ parser::repeat_statement(parsetree, inFile); }
	else
	{
		cout << "error: statement invalid" << endl;
		(*inFile).ignore();
		while ((*inFile).peek() != '\n')
		{ (*inFile).ignore(); }
	}

	parser::exitnode(parsetree, "statement");
}
void parser::if_statement(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("if_statement", Token(-1)));
	// find "if"
	parser::boolean_expression(parsetree, inFile);
	// find "then"
	parser::block(parsetree, inFile);
	// find "else"
	parser::block(parsetree, inFile);
	// find "end"
	parser::exitnode(parsetree, "if_statement");
}
void parser::while_statement(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("while_statement", Token(-1)));

	parser::exitnode(parsetree, "while_statement");
}
void parser::assignment_statement(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("assignment_statement", Token(-1)));

	parser::exitnode(parsetree, "assignment_statement");
}
void parser::repeat_statement(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("repeat_statement", Token(-1)));

	parser::exitnode(parsetree, "repeat_statement");
}
void parser::print_statement(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("statement", Token(-1)));

	parser::exitnode(parsetree, "print_statement");
}
void parser::boolean_expression(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("boolean_expression", Token(-1)));

	parser::exitnode(parsetree, "boolean_expression");
}
void parser::relative_op(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("relative_op", Token(-1)));

	parser::exitnode(parsetree, "relative_op");
}
void parser::arithmetic_expression(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("arithmetic_expression", Token(-1)));

	parser::exitnode(parsetree, "arithmetic_expression");
}
void parser::arithmetic_op(std::queue<ParseNode> *parsetree, ifstream *inFile)
{
	parser::enternode(parsetree, ParseNode("arithmetic_op", Token(-1)));

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
