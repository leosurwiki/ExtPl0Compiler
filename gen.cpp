#include "gen.h"
vector<pair<gen::Iname,string>> gen::system_stack;
string gen::now_parameter;
gen::Iname gen::now_Instuction;
int gen::tsum=0;
vector<gen::quartral_element> gen::internalcode;
extern string gen::I2string[]= 
{	"LOAD",
	"LOADI",
	"STORE",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"BR",
	"BREQL",
	"BRNEQ",
	"BRLSS",
	"BRLEQ",
	"BRGRT",
	"BRGQL",
	"BRIFTRUE",
	"BRIFFALSE",
	"EQL",
	"NEQ",
	"LSS",
	"LEQ",
	"GRT",
	"GQL",
	"JAL",
	"ADDi",
	"SUBi",
	"MULi",
	"DIVi",
	"LABEL",
	"CALL",
	"CALLFUN",
	"TAR",
	"PAR",
	"RETURN",
	"FUN",
	"CREATE",
	"ADDRSHIFT",
	"ADDRSHIFTarc",
	"WRITE",
	"READ",
	"NOD"
};

int * gen::now_line;
int gen::Isum=0;
vector<string> gen::stack_op;
void gen::LINEinit(int *_now_line)
{
	now_line=_now_line;
}
pair<gen::Iname,string> gen::genI(gen::Iname I,string para)
{
	return pair<gen::Iname,string>(I,para);
}
void gen::emit(gen::Iname I,string para)
{
	Isum++;
	if(I==CALL)
	{
		quartral_element q;
		q.op=I;q.second=para.substr(para.find('@')+1,para.size()-para.find('@')-1);q.first=para.substr(0,para.find('@'));
		internalcode.push_back(q);
		return;
	}	
	if (I==CALLFUN)
	{
		tsum++;
//		cout<<internalcode.size()+1<<I2string[I]<<" l@"<<para<<" t@"<<tsum<<endl;
		quartral_element q;
		q.op=I;q.second=para.substr(para.find('@')+1,para.size()-para.find('@')-1);q.first=para.substr(0,para.find('@'));q.target="t@"+helper::type2type::int2string(tsum);
		internalcode.push_back(q);
		stack_op.push_back("t@"+helper::type2type::int2string(tsum));
		return;
	}
	if (I==LOAD||I==LOADI)
	{
	//	cout<<internalcode.size()+1<<I2string[I]<<' '<<para<<endl;
		stack_op.push_back(para);
	}
	else 
	{
		if (para =="")
		{
			tsum++;
		//	cout<<internalcode.size()+1<<I2string[I]<<' '<<*(stack_op.end()-2)<<' '<<*(stack_op.end()-1)<<' '<<"t@"<<tsum<<endl;
			quartral_element q;
			q.op=I;q.first=*(stack_op.end()-2);q.second=*(stack_op.end()-1);q.target="t@"+helper::type2type::int2string(tsum);
			internalcode.push_back(q);
			stack_op.pop_back();stack_op.pop_back();
			stack_op.push_back("t@"+helper::type2type::int2string(tsum));
		}
		else
		{
	//		cout<<internalcode.size()+1<<I2string[I]<<' '<<*(stack_op.end()-2)<<' '<<*(stack_op.end()-1)<<' '<<para<<endl;
			quartral_element q;
			q.op=I;q.first=*(stack_op.end()-2);q.second=*(stack_op.end()-1);q.target=para;
			internalcode.push_back(q);
			stack_op.pop_back();stack_op.pop_back();
		}
	}
}
void gen::emit(gen::Iname I,int para)
{
	Isum++;
	if (I==BR||I==LABEL) 
	{
//		cout<<internalcode.size()+1<<I2string[I]<<" l@"<<para<<endl;
		quartral_element q;
		q.op=I;q.first="";q.second="";q.target="l@"+helper::type2type::int2string(para);
		internalcode.push_back(q);
		return ;
	}
	if (I==BRNEQ)
	{
		quartral_element q;
		q.op=I;q.first=*(stack_op.end()-1);q.second=*(stack_op.end()-2);q.target="l@"+helper::type2type::int2string(para);	
		internalcode.push_back(q);
		stack_op.pop_back();stack_op.pop_back();
	//	cout<<internalcode.size()<<' '<<I2string[q.op]<<' '<<q.first<<' '<<q.second<<' '<<q.target<<endl;
		return ;	
	}

	if (I==LOAD||I==LOADI)
	{
//		cout<<internalcode.size()+1<<I2string[I]<<' '<<para<<endl;
		stack_op.push_back("i@"+helper::type2type::int2string(para));
	}
	else 
	{
		if (I!=BRIFFALSE&&I!=BRIFTRUE) 
		{
			tsum++;
	//		cout<<internalcode.size()+1<<I2string[I]<<' '<<*(stack_op.end()-1)<<' '<<helper::type2type::int2string(para)<<" t@"<<tsum<<endl;
			quartral_element q;
			q.op=I;q.first=*(stack_op.end()-1);q.second="i@"+helper::type2type::int2string(para);q.target="t@"+helper::type2type::int2string(tsum);
			internalcode.push_back(q);
			stack_op.pop_back();
			stack_op.push_back("t@"+helper::type2type::int2string(tsum));
		}
		else
		{
	//		cout<<internalcode.size()+1<<I2string[I]<<' '<<*(stack_op.end()-1)<<" l@"<<para<<endl;
			quartral_element q;
			q.op=I;q.first=*(stack_op.end()-1);q.second="";q.target="l@"+helper::type2type::int2string(para);
			stack_op.pop_back();
			internalcode.push_back(q);
		}
	}
}
void gen::emit(string s)
{
	quartral_element q;
	q.op=RETURN;
	internalcode.push_back(q);
//	cout<<internalcode.size()<<s<<endl;
}
void gen::emit_JR(int r)
{
//	cout<<"JR $"<<r<<endl;	
}
void gen::emit_JAL(int l)
{
//	cout<<"JAL"<<'+'<<l<<endl;
}
void gen::emit_par(string s)
{
	quartral_element q;
	q.op=PAR;q.first="";q.second="";q.target=s;
	if (s[0]!='t'||s[1]!='@')
	{
		for (vector<string>::reverse_iterator index=symbol::function_stack.rbegin();index!=symbol::function_stack.rend();index++)
		{
			q.target=*index+":"+q.target;
		}
	}
	internalcode.push_back(q);
//	cout<<internalcode.size()<<"PAR"<<" "<<q.target<<endl;
}
void gen::emit_tar(string s)
{
	quartral_element q;
	q.op=TAR;q.first="";q.second="";q.target=s;
	if (s[0]!='t'||s[1]!='@')
	{
		for (vector<string>::reverse_iterator index=symbol::function_stack.rbegin();index!=symbol::function_stack.rend();index++)
		{
			q.target=*index+":"+q.target;
		}
	}	
	internalcode.push_back(q);
//	cout<<internalcode.size()<<"TAR"<<" "<<q.target<<endl;
}
void gen::emit_FUN(string s)
{
	quartral_element q;
	int levelsum=0;
	q.op=FUN;q.first="";q.second="";q.target=s;
	for (vector<string>::reverse_iterator index=symbol::function_stack.rbegin();index!=symbol::function_stack.rend();index++)
	{
		q.target=(*index+":")+s;
		levelsum++;
	}
	q.first=helper::type2type::int2string(levelsum);
	internalcode.push_back(q);
}
int gen::labelsum=0;
void gen::join()
{
	int now =0,i=0;
	while (i<internalcode.size())
	{
		if ((i<internalcode.size()-1)&&internalcode[i+1].op==BRIFFALSE)
		{
			switch (internalcode[i].op)
			{
			case EQL:internalcode[i].op=BRNEQ;internalcode[i].target=internalcode[i+1].target;break;
			case NEQ:internalcode[i].op=BREQL;internalcode[i].target=internalcode[i+1].target;break;
			case LSS:internalcode[i].op=BRGQL;internalcode[i].target=internalcode[i+1].target;break;
			case LEQ:internalcode[i].op=BRGRT;internalcode[i].target=internalcode[i+1].target;break;
			case GRT:internalcode[i].op=BRLEQ;internalcode[i].target=internalcode[i+1].target;break;
			case GQL:internalcode[i].op=BRLSS;internalcode[i].target=internalcode[i+1].target;break;
			default: break;
			}
			internalcode[now++]=internalcode[i];		
			i++;
		}
		else
		{
			if ((i<internalcode.size()-1)&&internalcode[i+1].op==ADDi&&internalcode[i+1].second=="i@0"&&internalcode[i+1].first==internalcode[i].target)			
			{
				internalcode[i].target=internalcode[i+1].target;
				internalcode[now++]=internalcode[i];
				i++;
			}
			else
			{
				internalcode[now++]=internalcode[i];
			}
		}
		i++;
	}
	while (internalcode.size()>now)
	{
		internalcode.pop_back();
	}
	/*
	for (vector<gen::quartral_element>::iterator idx=internalcode.begin()+1;idx!=internalcode.end();)
	{
		if (idx->op==gen::ADD&&idx->second=="i@0"&&idx->first==(idx-1)->target)
		{
			(idx-1)->target=idx->target;
			idx=internalcode.erase(idx);
		}
		else
		{
			idx++;
		}
	}
	*/

}
void gen::emit_CREATE(string para,string size)
{
	quartral_element q;
	q.first = para;
	for (vector<string>::reverse_iterator index=symbol::function_stack.rbegin();index!=symbol::function_stack.rend();index++)
	{
		q.first=*index+":"+q.first;
	}
	q.target = size;
	q.op = CREATE;
	internalcode.push_back(q);
}
void gen::emit_ADDRSHIFT(string para)//数组读,seconde为读出的临时变量，first为偏移量
{
	tsum++;
	quartral_element q;
	q.op=ADDRSHIFT;q.first=*(stack_op.end()-1);q.target=para;q.second="t@"+helper::type2type::int2string(tsum);
	stack_op.pop_back();
	stack_op.push_back("t@"+helper::type2type::int2string(tsum));
	internalcode.push_back(q);
}
void gen::emit_ADDRSHIFTarc(string para)//数组写,second为所存的变量,first为偏移量
{
	tsum++;
	quartral_element q;
	q.op=ADDRSHIFTarc;q.second=*(stack_op.end()-1);q.first=*(stack_op.end()-2);q.target=para;
	stack_op.pop_back();
	stack_op.pop_back();
	internalcode.push_back(q);
}
void gen::emit_str(string para)
{
	quartral_element q;
	q.op=WRITE;q.first=para;q.target="str";
	internalcode.push_back(q);
}
void gen::emit_expre()
{
	quartral_element q;
	q.op=WRITE;q.first=*(stack_op.end()-1);q.target="expre";
	stack_op.pop_back();
	internalcode.push_back(q);
}
void gen::emit_char()
{
	quartral_element q;
	q.op=WRITE;q.first=*(stack_op.end()-1);q.target="chr";
	stack_op.pop_back();
	internalcode.push_back(q);
}
void gen::emit_read(string para)
{
	quartral_element q;
	q.op=READ;q.first=para;
	internalcode.push_back(q);
}
bool gen::ismath(quartral_element qe)
{
	if (qe.op<=6&&qe.op>=3)//+-*/
	{
		return true;
	}
	if (qe.op>=34&&qe.op<=36)//+-*/i
	{
		return true;
	}
	if (qe.op>=24&&qe.op<=26)//CREATE,ARRAYSHIFT,ARRAYSHIFTarc
	{
		return true;
	}
	return false;
}