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
		false ��ʾ��eof,���У��ո��ж��ˣ�true��ʾ��Ȼ������ʹ��now_charΪ����ȡ�����ַ�
	*/
    token getsym(); 
	/*
		ȡһ���ʣ����ôʷǷ�����һ���յ��ַ���
	*/
	std::istream& input;
	token::line_number line_num;
	/*
		now_char��λ�á�
	*/
private:
    
    char now_char;
	/*
		����ȡ�����ַ���
	*/
   token identifier(char); //�ж�һ��identifier�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token number(char); //�ж�һ��number�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token becomes(char); //�ж�һ��becomes�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token left_paren(char); //�ж�һ��left_paren�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token right_paren(char); //�ж�һ��right_paren�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token left_bracket(char); //�ж�һ��left_bracket�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token right_bracket(char); //�ж�һ��right_bracket�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token semicolon(char); //�ж�һ��semicolon�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token comma(char); //�ж�һ��comma�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token grt_geq(char); //�ж�һ��grt_geq�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token lss_leq_neq(char); //�ж�һ��lss_leq_neq�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token eql(char); //�ж�һ��eql�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token plus_minus_op(char); //�ж�һ��plus_minus_op�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token mul_div_op(char); //�ж�һ��mul_div_op�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token constchr(char); //�ж�һ��constchr�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
   token conststr(char); //�ж�һ��conststr�����ɹ����ظ�token������nowchar()�ص������ַ���ǰһ���ַ���
};
#endif // TOKENIZER_H_INCLUDED
