#include <iostream>
#include "Token.h"

using namespace std;

int ID;
string lexeme;

Token::Token () { ID = 0; lexeme = ""; };
Token::Token (int _token) : ID(_token) { lexeme = ""; };
Token::Token (int _token, string _lexeme) : ID(_token), lexeme(_lexeme) {};
void Token::Print()
{ std::cout << "[ID:" << ID << " lex:" << lexeme << ']' << std::endl; };