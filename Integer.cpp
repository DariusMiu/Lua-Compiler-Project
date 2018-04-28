#include <iostream>
#include "Integer.h"

Integer::Integer (std::string _name, int _value) : name(_name), value(_value) {};
void Integer::Print()
{ std::cout << "[name:" << name << " value:" << value << ']' << std::endl; };
