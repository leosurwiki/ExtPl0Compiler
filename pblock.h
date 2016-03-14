#ifndef INCLUDE_PBLOCK_H
#define INCLUDE_PBLOCK_H
#include "gen.h"
#include <vector>
#include <set>
using namespace std;
//数据流分析
namespace pblock
{
	class PBLOCK
	{
	public:
		int begin;
		int end;
		set<string> in;
		set<string> out;
		set<string> def;
		set<string> use;
		vector<int> next;
		string functionname;
		vector<string> var;
		int level;
		bool valid;
	};
	int findnext(string s);
	bool isgap(gen::quartral_element I);
	bool iscal(gen::quartral_element I);
	bool isbr(gen::quartral_element I);
	bool isalloc(string s);
	bool isasr(string s);	
	set<string>isdef(const vector<gen::quartral_element> &ic,int begin,int end);
	set<string>isuse(const vector<gen::quartral_element> &ic,int begin,int end);
	extern vector<PBLOCK> split(vector<gen::quartral_element> INTERNAL);
	extern vector<PBLOCK> resplit(vector<gen::quartral_element> INTERNAL);
}
#endif