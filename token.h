#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED
#include <string>
#include "helper.h"
#include <iostream>
#include <map>
#include <set>
//pascal����
class token
{
public:

	typedef unsigned int line_number;
    const static enum token_type{
        nul,
        ident,
        number,
        plus,
        mul,
        oddsym,
        eql,
        neq,
        lss,
        leq,
		lbracket,
		rbracket,
        gtr,
        geq,
        lparen,
        rparen,
        comma,
        semicolon,
        period,
        becomes,
		arraysym,
		beginsym,
		charsym,
		constsym,
		dosym,
		downtosym,
		endsym,
		forsym,
		functionsym,
		ifsym,
		ofsym,
		proceduresym,
		readsym,
		thensym,
		tosym,
		varsym,
		whilesym,
		writesym,
		constchr,
		conststr,
		colon,
		integersym,
		elsesym
    };

	token();
	/*
		����һ���������кŵ�token������һЩ��ʱ��token���߿�tokenʱ�õ�
	*/
    token(std::string,int);
	token(std::string, token::token_type, token::line_number);
    /*
			���token���ַ���ֵ���ַ��������������ţ��ַ�������˫����
	*/
	std::string get_value() const;
	/*
	*/
	token_type get_type() const;
	/*
		���token�����к�
	*/
	line_number get_line() const;
	static const token::token_type reservetype[];
	static const std::string token::reserve[];
	static void mapping();
	const static unsigned int NUMBER_OF_SYMTYPE = 43;
	const static unsigned int NUMBER_OF_RESERVE = 20;
	void debug();
private:
	std::string value;
	token_type type;
	line_number line;
};
extern std::map<token::token_type,std::string> type2string;
extern std::map<std::string,token::token_type> string2type;
extern std::set<std::string> set_of_reserveword;
#endif // TOKEN_H_INCLUDED
