#include <iostream>
#include <fstream>
#include "grammar.h"
#include "gen.h"
#include "interpreter.h"
#include "dag.h"
#include "pblock.h"
#include "flow.h"
int main()
{
	string fname;
	cout<<"filename:";
	cin>>fname;
	ifstream fin(fname);
	ofstream fout("output.txt");
	grammar::grammarbuilder gr= grammar::grammarbuilder(tokenizer(fin));
	vector<pblock::PBLOCK> vbp;
	gr.program();
	if (gr.executable) 
	{
		vbp=pblock::split(internalcode);
		dag::dag_symplify(internalcode,vbp);
		vbp=pblock::resplit(internalcode);
		flow::active_variable(vbp,internalcode);
		cout<<"sequence of quartral_elements is above:"<<endl;
		interpreter::interpreter translator =  interpreter::interpreter(fout);
		translator.translate(internalcode);		
	}	
	system("pause");
	return 0;	
}