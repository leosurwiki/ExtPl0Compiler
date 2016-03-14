#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED
#include <ctype.h>
#include <sstream>
#include <string>
//¹¤¾ß¼¯
namespace helper
{
namespace tokenizer
{
	bool is_alphabet(char ch);
	bool is_number_except0(char ch);
	bool is_number(char ch);
	bool is_white_space(char ch);
}
namespace type2type
{
	int string2int(std::string);
	std::string int2string(int);
}
namespace qetrait
{
	int literal2int(std::string s);
	bool iflitteral(std::string s);
	bool iftemp(std::string s);
	bool iflabel(std::string s);
	bool ifalloc(std::string s);
}
}
#endif