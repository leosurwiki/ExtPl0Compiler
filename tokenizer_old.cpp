#include "tokenizer.h"
tokenizer::tokenizer(std::istream& _input):input(_input)
{
	line_num = 1;
}

bool tokenizer::getch()
{
	char tmpc;
	bool constantly=true;
	if (input.eof())
	{
		return false;
	}
	while (input.get(tmpc)&&(tmpc=='\n'||isspace(tmpc)))
	{
		if (tmpc=='\n') {line_num+=1;}
		constantly = false;
	}
	now_char=tmpc;
	return constantly;
}


token tokenizer::getsym()
{
	tokenizer::getch();
	if (helper::tokenizer::is_alphabet(now_char))
	{
		return identifier(now_char);	
	}
	if (helper::tokenizer::is_number(now_char))
	{
		return number(now_char);
	}
	if ((int)(unsigned char)now_char<0||now_char=='\n'||now_char==' ') return token("EOF",line_num);
	switch (now_char)
	{
		case '\'':return constchr(now_char);
		case '\"':return conststr(now_char);
		case ':':return becomes(now_char);
		case '<':return lss_leq_neq(now_char);
		case '>':return grt_geq(now_char);
		case '=':return eql(now_char);
		case '+':case '-':return plus_minus_op(now_char);
		case '*':case '/':return mul_div_op(now_char);
		case '(':return left_paren(now_char);
		case ')':return right_paren(now_char);
		case '[':return left_bracket(now_char);
		case ']':return right_bracket(now_char);
		case ';':return semicolon(now_char);
		case ',':return comma(now_char);
		case '.':return token(".",token::token_type::period,line_num);
		default :return token("wrong ascII",line_num);
	}	
}
token tokenizer::constchr(char start)
{
	std::string value;
	int now_line=line_num;
	if (!tokenizer::getch()) {if (!input.eof()) {input.unget();};return token("������û���",now_line);}
	value += now_char;
	if (helper::tokenizer::is_alphabet(now_char))
	{
		if (!tokenizer::getch()) {if (!input.eof()) {input.unget();};return token("������û���",now_line);}
		if (now_char=='\'')
		{
			return token(value, token::token_type::constchr, now_line);	
		}
		else 
		{
			input.unget();
			return token("������û���",now_line);
		}
	}
	else 
	{
		input.unget();
		return token("�����ż��зǷ��ַ�",now_line);	
	}
}
token tokenizer::conststr(char start)
{
	std::string value;
	int now_line=line_num;
	value += now_char;
	while (tokenizer::getch())
	{
		value += now_char;
		if (!(helper::tokenizer::is_alphabet(now_char)||helper::tokenizer::is_number(now_char)))
		{
			if (now_char=='\"')
			{
				return token(value,token::token_type::conststr,now_line);
			}
			else
			{
				if (!input.eof()) {input.unget();}
				return token("˫���ź�����˷Ƿ��ַ�",now_line);
			}
		}
	}
	if (!input.eof()) {input.unget();}
	return token("˫����û���",now_line);
}

token tokenizer::identifier(char start)
{
	std::string value;
	int now_line=line_num;
	value += now_char;
	while (tokenizer::getch())
	{
		if (!(helper::tokenizer::is_alphabet(now_char)||helper::tokenizer::is_number(now_char)))
		{break;}
		value += now_char;
	}
	//�ڴ�һ�������˿հף����߷���ĸ�����ֵ��ַ�������ʱ��ָΪ�÷���ĸ�����ֵ��ַ��������жϺ���һ��ʵ���ַ���
	if (!input.eof()) {input.unget();}
	//��δ���������³����ַ�
	if (set_of_reserveword.find(value)==set_of_reserveword.end())
	{
		return token(value, token::token_type::ident, now_line);
	}
	else
	{
		return token(value,string2type[value+"sym"],now_line);
	}
}

token tokenizer::number(char start)
{
	std::string value;
	int now_line=line_num;
	value += now_char;
	while (tokenizer::getch())
	{
		if (!helper::tokenizer::is_number(now_char))
		{break;}
		value += now_char;
	}
	//�ڴ�һ�������˿հף����߷����ֵ��ַ�������ʱ��ָΪ�÷���ĸ�����ֵ��ַ��������жϺ���һ��ʵ���ַ���
	if (!input.eof()) {input.unget();}
	//��δ���������³����ַ�
	return token(value, token::token_type::number, now_line);
}

token tokenizer::becomes(char start)
{
	int now_line=line_num;
	if (getch()==false)
	{
		if (!input.eof()) {input.unget();}
		return token(":",token::token_type::colon,now_line);
	}
	//���µ��ַ�һ�����룺������
	if (now_char=='=')
	{
		return token(":=",token::token_type::becomes,line_num);
	}
	else 
	{
		if (!input.eof()) {input.unget();}
		return token(":",token::token_type::colon,line_num);//ԭ����ÿ�δʷ�����������getch���ж�����Ч���ݣ����ᱻ�³�
	}
}

token tokenizer::left_paren(char start)
{
	return token("(", token::token_type::lparen, line_num);
}

token tokenizer::right_paren(char start)
{
	return token(")", token::token_type::rparen, line_num);
}

token tokenizer::left_bracket(char start)
{
	return token("[", token::token_type::lbracket, line_num);
}

token tokenizer::right_bracket(char start)
{
	return token("]", token::token_type::rbracket, line_num);
}

token tokenizer::semicolon(char start)
{
	return token(";",token::token_type::semicolon,line_num);
}

token tokenizer::comma(char start)
{
	return token(",",token::token_type::comma,line_num);
}
token tokenizer::grt_geq(char start)
{
	int nowline=line_num;
	if (!getch())
	{
		if (!input.eof()) {input.unget();}
		return token(">",token::token_type::gtr,nowline);
	}
	//���µ��ַ�һ������>������
	if (now_char=='=')
	{
		return token(">=",token::token_type::geq,line_num);
	}
	else
	{
		if (!input.eof()) {input.unget();}
		return token(">",token::token_type::gtr,line_num);
	}
}
token tokenizer::lss_leq_neq(char start)
{
	int now_line=line_num;
	if (!getch())
	{
		if (!input.eof()) {input.unget();}
		return token("<",token::token_type::lss,now_line);
	}
	//���µ��ַ�һ������>������
	if (now_char=='=')
	{
		return token("<=",token::token_type::leq,line_num);
	}
	else
	{
		if (now_char=='>')
		{
			return token("<>",token::token_type::neq,line_num);
		}
		else
		{
			if (!input.eof()) {input.unget();}
			return token("<",token::token_type::lss,line_num);
		}
	}
}
token tokenizer::eql(char start)
{
	return token("=",token::token_type::eql,line_num);
}
token tokenizer::plus_minus_op(char start)
{
	std::string tmp="";
	tmp+=start;
	return token(tmp,token::token_type::plus,line_num);
}
token tokenizer::mul_div_op(char start)
{
	std::string tmp="";
	tmp+=start;
	return token(tmp,token::token_type::mul,line_num);
}
/*
int main()
{ 
	std::ifstream fin = std::ifstream("input.txt");
	tokenizer tk = tokenizer(fin);
	token sym = token();
	token::mapping();
	while (!tk.input.eof())
	{
		sym = tk.getsym();
		sym.debug();
	}
	system("pause");
}
*/