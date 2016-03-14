#include "token.h"
const token::token_type token::reservetype[] =
{
    token_type::nul,
    token_type::ident,
    token_type::number,
    token_type::plus,
    token_type::mul,
    token_type::oddsym,
    token_type::eql,
    token_type::neq,
    token_type::lss,
    token_type::leq,
    token_type::lbracket,
	token_type::rbracket,
    token_type::gtr,
    token_type::geq,
    token_type::lparen,
    token_type::rparen,
    token_type::comma,
    token_type::semicolon,
    token_type::period,
    token_type::becomes,
    token_type::arraysym,
    token_type::beginsym,
    token_type::charsym,
    token_type::constsym,
    token_type::dosym,
    token_type::downtosym,
    token_type::endsym,
    token_type::forsym,
    token_type::functionsym,
    token_type::ifsym,
    token_type::ofsym,
    token_type::proceduresym,
    token_type::readsym,
    token_type::thensym,
    token_type::tosym,
    token_type::varsym,
    token_type::whilesym,
    token_type::writesym,
    token_type::constchr,
    token_type::conststr,
	token_type::colon,
	token_type::integersym,
	token_type::elsesym
};
const std::string token::reserve[] =
{
    "nul",
    "ident",
    "number",
    "plus",
    "mul",
    "oddsym",
    "eql",
    "neq",
    "lss",
    "leq",
    "lbracket",
	"rbracket",
    "gtr",
    "geq",
    "lparen",
    "rparen",
    "comma",
    "semicolon",
    "period",
    "becomes",
    "arraysym",
    "beginsym",
    "charsym",
    "constsym",
    "dosym",
    "downtosym",
    "endsym",
    "forsym",
    "functionsym",
    "ifsym",
    "ofsym",
    "proceduresym",
    "readsym",
    "thensym",
    "tosym",
    "varsym",
    "whilesym",
    "writesym",
    "constchr",
    "constst",
	"colon",
	"integersym",
	"elsesym"
};
const std::string reserveword[] = 
{
   "array",
   "begin",
   "char",
   "const",
   "do",
   "downto",
   "else",
   "end",
   "for",
   "function",
   "if",
   "integer",
   "of",
   "procedure",
   "read",
   "then",
   "to",
   "var",
   "while",
   "write"
};
std::map<token::token_type,std::string> type2string;
std::map<std::string,token::token_type> string2type;
std::set<std::string> set_of_reserveword;
void token::mapping()
{
	for (int i=0;i<NUMBER_OF_SYMTYPE;i++)
	{
		string2type[reserve[i]]=reservetype[i];
		type2string[reservetype[i]]=reserve[i];
	}
	for (int i=0;i<NUMBER_OF_RESERVE;i++)
	{
		set_of_reserveword.insert(reserveword[i]);
	}
}
token::token()
{
	type=token_type::nul;
}
token::token(std::string s,int l)
{
	value = s;
	type=token_type::nul;
	line=l;
}
void token::debug()
{
	if (value!="")
	{
		std::cout<<value<<' '<<type2string[type]<<' '<<line<<std::endl;
	}
}

token::token(std::string _value, token::token_type _type, token::line_number _line)
{
	value = _value;
	type = _type;
	line = _line;
	if(type != conststr && type != constchr)
	{
		for(unsigned int i = 0; i < value.size(); i++){value[i] = value[i];}
	}
}
std::string token::get_value() const
{
	return this->value;
}

token::token_type token::get_type() const
{
	return this->type;
}

token::line_number token::get_line() const
{
	return this->line;
}


#undef _NO_LINE_INITIALIZED

