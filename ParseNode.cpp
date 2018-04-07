#include <iostream>
#include "ParseNode.h"

using namespace std;

string name;
Token token;

ParseNode::ParseNode (string _name, Token _token) : name(_name), token(_token) {};
