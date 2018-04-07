#ifndef parser_h
#define parser_h

#include <fstream>
#include <string>
#include <queue>
#include "ParseNode.h"


class parser
{
	public:
		static void parse(char* filename);
	protected: 
		static void program(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static void block(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static void statement(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static void if_statement(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static void while_statement(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static void assignment_statement(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static void repeat_statement(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static void print_statement(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static void boolean_expression(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static void relative_op(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static void arithmetic_expression(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static void arithmetic_op(std::queue<ParseNode> *parsetree, std::ifstream *inFile);
		static void enternode(std::queue<ParseNode> *parsetree, ParseNode newnode);
		static void exitnode(std::queue<ParseNode> *parsetree, std::string newnode);
};

#endif