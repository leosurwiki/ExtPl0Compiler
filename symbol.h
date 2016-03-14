#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED
#include <map>
#include <string>
#include <malloc.h>
#include <iostream>
#include "helper.h"
#include <vector>
//符号表
using namespace std;
namespace symbol
{
	extern vector<string> function_stack;
	typedef enum value_type
	{
		INT,
		CHAR,
		VOID
	};
	struct varlist
	{
	public:
		bool isVar;
		string name;
		value_type Val_type;		
	};
	typedef enum symbol_type 
	{
		LITERALCHR,
		LITERALSTR,
		CONST,
		VAR,
		ARRAY,
		FUNCTION,
		ERROR,
		LITERALINT
	};
	class entry
	{
	public:
		string name;
		symbol_type symtype;
		value_type valtype;
		vector<varlist> parameter;
		int size;
		int level;
		int label;
		int addr;
		bool isR;
		bool loop;
		entry();
		entry(int l);
		/*
			条目的层数
		*/
		static entry LITERALCHR_entry(string value);
		static entry LITERALSTR_entry(string value);
		static entry LITERALINT_entry(string value);
		static entry CONST_entry(string name,symbol::value_type vt);
		static entry VAR_entry(string name,int capacity,symbol::value_type vt,symbol::symbol_type);
		static entry FUNCTION_entry();
		static entry PARA_entry(string value,symbol::value_type vt,bool isR);
	};
	class table
	{
	public:
		map<string,entry> symbollist;
		entry getentry(string,int);
		void enter(symbol::entry,int);
		void debug();
		void erasenowlevel(int);
		void reverseloop(string,int);
	};
}
#endif