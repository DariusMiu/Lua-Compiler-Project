#ifndef Token_h
#define Token_h

#include <fstream>
//#include <string>

class Token
{
	public:
		Token (int _token, std::string _lexeme);
		int tokenID;
		std::string lexemeWord;
		void Print();
};

#endif