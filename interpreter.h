#include "gen.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <deque>
#include "flow.h"
using namespace gen;
//编译四元式为MIPS指令
namespace interpreter
{
	const int zero = 0;
	const int at = 1;
	const int v[] = {2,3};
	const int vsize =2;
	const int a[] = {4,5,6,7};
	const int asize =4;
	const int t[] = {8,9,10,11,12,13,14,15,24,25};
	const int tsize = 10;
	const int s[] = {16,17,18,19,20,21,22,23};
	const int ssize = 8;
	const int gp = 28;
	const int sp = 29;
	const int fp = 30;
	const int ra = 31;
	enum MIPS_CODE  
	{
		lw,sw,add,sub,mult,div,addi,subi,beq,bne,bgez,blez,bgtz,bltz,syscall,jr,jal,j,mflo,mfhi	
	};
	class RigisterFile	
	{
		bool r[32];
	};
	class interpreter 
	{	
	public:
		std::ostream &fout;
		interpreter(std::ostream&_fout);
		void init_sp_fp_ra();
		bool ifalloc(string s);
		bool iflitteral(string s);
		bool iflabel(string s);
		bool iftemp(string s);
		int literal2int(string s);
		map<int,int> reg2mem;
		void get_shift(vector<quartral_element>& v);
		/*
			计算相对$sp的偏移量
		*/
		void interpret_ADD(quartral_element qe);//返回是否成功
		void interpret_SUB(quartral_element qe);//返回是否成功
		void interpret_MUL(quartral_element qe);//返回是否成功
		void interpret_DIV(quartral_element qe);//返回是否成功
		void interpret_BR(quartral_element qe);//返回是否成功
		void interpret_BREQL(quartral_element qe);//返回是否成功
		void interpret_BRNEQ(quartral_element qe);//返回是否成功
		void interpret_BRLSS(quartral_element qe);//返回是否成功
		void interpret_BRLEQ(quartral_element qe);//返回是否成功
		void interpret_BRGRT(quartral_element qe);//返回是否成功
		void interpret_BRGQL(quartral_element qe);//返回是否成功
		void interpret_LABEL(quartral_element qe);
		void interpret_PAR(quartral_element qe);
		void interpret_TAR(quartral_element qe);
		void interpreter_ADDRSHIFT(quartral_element qe);
		void interpreter_ADDRSHIFTarc(quartral_element qe);
		void deal_qe(quartral_element qe);
		void interpret_CREATE(quartral_element qe);
		void interpreter_fun(quartral_element qe);
		void interpreter_subprogram(quartral_element qe);
		void interpreter_write(quartral_element qe);
		void interpreter_read(quartral_element qe);
		void translate(vector<quartral_element>& v);
		vector<string> data_area;
		vector<string> functionstack;//函数嵌套环境
		deque<int> spfunshift;//进入某个函数后sp移动的长度
		int shift2sp;
		string get_trait_string(int);
		void symbol_load(string);
		void symbol_save(string);
		map<string,int> string2shiftfromsp;
		vector<string> offset;
	};
}