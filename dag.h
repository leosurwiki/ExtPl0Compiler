#ifndef INCLUDE_DAG_H
#define INCLUDE_DAG_H
#include "gen.h"
#include "pblock.h"
#include "helper.h"
#include <hash_map>
using namespace std;
using namespace gen;
//dagÕº¿‡
namespace dag
{	
	extern hash_map<string,int> string2node;
	struct node
	{
		gen::Iname op;
		int left;
		int right;
		string entity;
	};
	extern vector<node> vqn;
	extern void dag_symplify(vector<quartral_element> &vqe,vector<pblock::PBLOCK> vqb);
}
#endif