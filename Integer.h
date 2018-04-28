#ifndef Integer_h
#define Integer_h

class Integer
{
	public:
		Integer (std::string _name, int _value);
		std::string name;
		int value;
		void Print();
};

#endif