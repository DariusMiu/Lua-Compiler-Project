#ifndef ParseNode_h
#define ParseNode_h

#include <string>
#include "Token.h"

class ParseNode
{
	public:
		ParseNode (std::string _name, Token _token);
		std::string name;
		Token token;
};

#endif