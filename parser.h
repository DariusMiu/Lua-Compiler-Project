#ifndef parser_h
#define parser_h

#include <fstream>
#include <string>
#include <queue>
#include "ParseNode.h"
#include "Token.h"
#include "interpreter.h"

static interpreter* interp;
static bool log;

class parser
{
	public:
		static void parse(char* filename, bool Log);
		static void parse(char* filename, interpreter* itpr, bool Log);
	protected: 
		static void program(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static void block(std::queue<ParseNode> *parsetree, std::ifstream *inFile, bool run);
		static void statement(std::queue<ParseNode> *parsetree, std::ifstream *inFile, bool run);
		static void if_statement(std::queue<ParseNode> *parsetree, std::ifstream *inFile, bool run);
		static void while_statement(std::queue<ParseNode> *parsetree, std::ifstream *inFile, bool run);
		static void assignment_statement(std::queue<ParseNode> *parsetree, std::ifstream *inFile, bool run);
		static void repeat_statement(std::queue<ParseNode> *parsetree, std::ifstream *inFile, bool run);
		static void print_statement(std::queue<ParseNode> *parsetree, std::ifstream *inFile, bool run);
		static Token boolean_expression(std::queue<ParseNode> *parsetree, std::ifstream *inFile, bool run);
		static Token relative_op(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static Token arithmetic_expression(std::queue<ParseNode> *parsetree, std::ifstream *inFile, bool run);
		static Token arithmetic_op(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static void enternode(std::queue<ParseNode> *parsetree, ParseNode newnode);
		static void exitnode(std::queue<ParseNode> *parsetree, std::string newnode);
};

#endif