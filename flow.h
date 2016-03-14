#ifndef INCLUDE_FLOW_H
#define INCLUDE_FLOW_H
#include "pblock.h"
#include "gen.h"
#include <vector>
using namespace std;
//数据流分析
namespace flow
{
	void active_variable(vector<pblock::PBLOCK>& vqb,vector<gen::quartral_element>& vqe);
	extern map<string,int> color;
	const int colorsum=8;
}
#endif