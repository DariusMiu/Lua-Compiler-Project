#ifndef scanner_h
#define scanner_h

class Token;

#include <fstream>
//#include <string>

static std::string LexemeStrings[] = {"operator", "keyword", "digit", "id"};
static char operators[][2] = {{'='}, {'<'}, {'>'}, {'+'}, {'-'}, {'*'}, {'/'}, {'('}, {')'}, {'<','='}, {'=','='}, {'~','='}, {'>','='}};
static std::string keywords[] = {"and", "break", "do", "else", "elseif", "end", "false", "for", "function", "if", "in", "local", "nil",
	"not", "or", "repeat", "return", "then", "true", "until", "while", "print"};
static char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
static char ids[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
	'x', 'y', 'z'};


class scanner
{
	public:
		static Token getToken(std::ifstream *inFile);
		static Token peekToken(std::ifstream *inFile);
	private: 
		static bool isop(char test);
};

#endif