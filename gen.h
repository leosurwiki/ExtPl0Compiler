#ifndef GEN_H_INCLUDE
#define GEN_H_INCLUDE
#include <vector>
#include <set>
#include <iostream>
#include <string>
#include "helper.h"
#include "symbol.h"
using namespace std;
//四元式类
namespace gen{
typedef enum Iname 
{
	LOAD,
	LOADI,
	STORE,
	ADD,
	SUB,
	MUL,
	DIV,
	BR,
	BREQL,
	BRNEQ,
	BRLSS,
	BRLEQ,
	BRGRT,
	BRGQL,
	BRIFTRUE,
	BRIFFALSE,
	EQL,
	NEQ,
	LSS,
	LEQ,
	GRT,
	GQL,
	JAL,
	ADDi,
	SUBi,
	MULi,
	DIVi,
	LABEL,
	CALL,
	CALLFUN,//first为函数层数 second为函数跳转位置 如果是CALLFUN target为存回的临时变量
	TAR,
	PAR,
	RETURN,
	FUN,
	CREATE,
	ADDRSHIFT,
	ADDRSHIFTarc,
	WRITE,
	READ,
	NOD
};
struct quartral_element
{
	Iname op;
	string first;
	string second;
	string target;
	int order;	
};
bool ismath(quartral_element qe);
extern vector<quartral_element> internalcode;
extern string I2string[];
extern vector<pair<Iname,string>> system_stack;
extern string now_parameter;
extern Iname now_Instuction;
extern int labelsum,Isum,tsum;
extern int *now_line;
pair<Iname,string> genI(gen::Iname ins,string para);
Iname string2Iname(string);
void emit(Iname I,string para);
void emit(Iname I,int para);
void emit(string);
void emitbuffer(Iname I,string para);
void LINEinit(int *_now_line);
extern vector<string> stack_op;
void emit_JR(int r);
void emit_JAL(int l);
void emit_par(string s);
void emit_tar(string s);
void emit_FUN(string s);
void emit_CREATE(string para,string size);
void emit_ADDRSHIFT(string para);
void emit_ADDRSHIFTarc(string para);
void emit_str(string para);
void emit_expre();
void emit_read(string para);
void emit_char();
void join();
}
#endif