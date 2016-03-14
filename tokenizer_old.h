#ifndef TOKENIZER_H_INCLUDED
#define TOKENIZER_H_INCLUDED
#include <iostream>
#include <fstream>
#include "token.h"
class tokenizer
{
public:
    tokenizer(std::istream& _input);
    bool getch();
	/*
		false 表示被eof,换行，空格中断了，true表示依然连续。使得now_char为现在取到的字符
	*/
    token getsym(); 
	/*
		取一个词，若该词非法返回一个空的字符。
	*/
	std::istream& input;
	token::line_number line_num;
	/*
		now_char的位置。
	*/
private:
    
    char now_char;
	/*
		现在取到的字符。
	*/
   token identifier(char); //判断一个identifier，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token number(char); //判断一个number，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token becomes(char); //判断一个becomes，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token left_paren(char); //判断一个left_paren，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token right_paren(char); //判断一个right_paren，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token left_bracket(char); //判断一个left_bracket，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token right_bracket(char); //判断一个right_bracket，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token semicolon(char); //判断一个semicolon，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token comma(char); //判断一个comma，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token grt_geq(char); //判断一个grt_geq，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token lss_leq_neq(char); //判断一个lss_leq_neq，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token eql(char); //判断一个eql，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token plus_minus_op(char); //判断一个plus_minus_op，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token mul_div_op(char); //判断一个mul_div_op，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token constchr(char); //判断一个constchr，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
   token conststr(char); //判断一个conststr，若成功返回该token，否则nowchar()回到出错字符的前一个字符处
};
#endif // TOKENIZER_H_INCLUDED
