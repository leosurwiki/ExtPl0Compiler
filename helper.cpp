#include "helper.h"
bool helper::tokenizer::is_alphabet(char ch)
{
	return isalpha(ch) || ch == '_';
}
bool helper::tokenizer::is_number_except0(char ch)
{
	return (ch >= '1' && ch <= '9');
}
bool helper::tokenizer::is_number(char ch)
{
	return (ch >= '0' && ch <= '9');
}
bool helper::tokenizer::is_white_space(char ch)
{
	if(isspace(ch)){return true;}
	else{ return false;	}
}
int helper::type2type::string2int(std::string s)
{
	return atoi(s.c_str());
}
std::string helper::type2type::int2string(int x)
{
	std::stringstream ss;
	ss<<x;
	return ss.str();
}
bool helper::qetrait::ifalloc(std::string s)
{
	if (s=="") return false;
	if ((s.find('@')!=std::string::npos&&s[0]=='t')||s.find('@')==std::string::npos)
	{
		return true;
	}
	return false;	
}
bool helper::qetrait::iflabel(std::string s)
{
	if (s.find('@')!=std::string::npos&&s[0]=='l')
	{
		return true;
	}
	return false;
}
bool helper::qetrait::iftemp(std::string s)
{
	if (s.find('@')!=std::string::npos&&s[0]=='t')
	{
		return true;
	}
	return false;
}
bool helper::qetrait::iflitteral(std::string s)
{
	if (s.find('@')!=std::string::npos&&s[0]=='i')
	{
		return true;
	}
	return false;
}
int helper::qetrait::literal2int(std::string s)
{
	s.erase(0,2);
	return helper::type2type::string2int(s);
}
