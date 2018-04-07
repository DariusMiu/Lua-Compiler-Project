#ifndef Token_h
#define Token_h

#include <fstream>
//#include <string>

class Token
{
	public:
		Token (int _token, std::string _lexeme);
		Token (int _token);
		Token ();
		int ID;
		std::string lexeme;
		void Print();
};

#endif