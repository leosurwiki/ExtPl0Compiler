#ifndef GRAMMAR_H_INCLUDED
#define GRAMMAR_H_INCLUDED
#include "tokenizer.h"
#include "symbol.h"
#include <deque>
#include <vector>
#include "gen.h"
#include <exception>
#include <ios>
using namespace gen;
//语法分析与语义分析
#define Next_isTOKEN(tktype,pass,notpass) getsym();\
if (now_sym.get_type()==tktype) \
{\
grammarbuilder::errorflag.clear();\
pass \
}\
else \
{\
grammarbuilder::errorflag=grammarbuilder::errorflag+" "+type2string[tktype]; \
notpass\
}\

namespace grammar
{
	class grammarbuilder
	{
	public:
		string errorflag;
		token now_sym;
		token last_sym;
		void getsym();
		tokenizer tk;
		int now_line;
		int now_level;
		symbol::table symbol_list;
		grammarbuilder(tokenizer &tk);
		int shift2sp;
		void push_back();
		int block();
		bool constdec();
		bool vardec();
		bool statement();
		bool complex_statement();
		bool becoming_statement();
		bool if_statement();
		bool while_statement();
		bool for_statement();
		bool condition();
		bool proceduredec();
		bool functiondec();
		bool program();
		bool write_statement();
		bool read_statement();
		int expre();
		int term();
		int factor();
		deque<token> cache;
		vector<string> functionsituation;
		void grammar_error_handler();
		bool executable;
		string unexpected,expected;
	};
}
#endif