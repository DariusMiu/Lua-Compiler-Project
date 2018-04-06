#include <iostream>
#include "Token.h"

using namespace std;

int tokenID;
string lexemeWord;

Token::Token (int _token, string _lexeme) : tokenID(_token), lexemeWord(_lexeme) {};
void Print()
{ std::cout << "token:" << tokenID << " lexeme:" << lexemeWord << std::endl; };