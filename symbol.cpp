#include "symbol.h"
vector<string> symbol::function_stack;
symbol::entry symbol::entry::VAR_entry(string value,int capacity,symbol::value_type vt,symbol::symbol_type st)
{
	symbol::entry e = symbol::entry();
	e.name=value;
	e.size = capacity;
	e.valtype=vt;
	e.symtype=st;
	return e;
}
symbol::entry symbol::entry::CONST_entry(string value,symbol::value_type vt)
{
	symbol::entry e = symbol::entry();
	e.valtype=vt;
	e.symtype=CONST;
	e.name=value;
	switch(vt)
	{
		case symbol::value_type::CHAR:e.size=sizeof(char);break;
		case symbol::value_type::INT:e.size=sizeof(int);break;
		default:return entry();
	}	
	return e;
}
symbol::entry symbol::entry::LITERALCHR_entry(string value)
{
	entry e = entry();
	e.symtype=symbol::symbol_type::LITERALCHR;
	e.name=value;
	e.name.erase(value.length()-1,1);
	e.name.erase(0,1);
	return e;
}
symbol::entry symbol::entry::LITERALINT_entry(string value)
{
	entry e = entry();
	e.symtype = symbol::symbol_type::LITERALINT;
	e.name = value;
	e.valtype = symbol::value_type::INT;
	return e;
}
symbol::entry symbol::entry::LITERALSTR_entry(string value)
{
	entry e = entry();
	e.symtype=symbol::symbol_type::LITERALSTR;
	e.name=value;
	e.name.erase(value.length()-1,1);
	e.name.erase(0,1);
	return e;
}
symbol::entry symbol::entry::PARA_entry(string value,symbol::value_type vt,bool _isR)
{
	entry e = entry();
	e.name = value;
	e.isR = _isR;
	e.valtype=vt;
	e.symtype=symbol::symbol_type::VAR;
	return e;
}
symbol::entry::entry()
{
	loop=false;
	symtype=symbol_type::ERROR;
}
symbol::entry::entry(int l)
{
	loop=false;
	level = l;
}
void symbol::table::enter(symbol::entry e,int l)
{
	e.level=l;
	string key=helper::type2type::int2string(l)+"@"+e.name;
	if (symbollist.find(key)!=symbollist.end())
	{
		cout<<"warnning:duplicate identifier "<<e.name<<", the latter one is ignored"<<endl;
		return;
	}
	for (vector<string>::reverse_iterator index=function_stack.rbegin();index!=function_stack.rend();index++)
	{
		e.name=*index+":"+e.name;
	}
	symbollist.insert(pair<string,entry>(key,e));
}
void symbol::table::reverseloop(string s,int l)
{
	map<string,entry>::iterator index;	
	for (int i=l;i>=0;i--)
	{
		string key=helper::type2type::int2string(i)+"@"+s;
		if (symbollist.find(key)!=symbollist.end())
		{
			symbollist[key].loop=!symbollist[key].loop;
		}
		else {/*pass*/}
	}
}

symbol::entry symbol::table::getentry(string s,int l)
{
	map<string,entry>::iterator index;	
	for (int i=l;i>=0;i--)
	{
		string key=helper::type2type::int2string(i)+"@"+s;
		if (symbollist.find(key)!=symbollist.end())
		{
			return symbollist[key];
		}
		else {/*pass*/}
	}
	cout<<"because "<<s<<" is not found"<<endl;
	return entry();//Ã»ÓÐÕÒµ½·ûºÅ
}
void symbol::table::erasenowlevel(int level)
{
	map<string,entry>::iterator index;
	string ss;
	for (index=symbollist.begin();index!=symbollist.end();)
	{
		ss = index->first.substr(0,index->first.find("@"));
		if (level==helper::type2type::string2int(ss))
		{
			symbollist.erase(index++);
		}
		else
		{
			index++;
		}
	}
}

void symbol::table::debug()
{
	for (map<string,entry>::iterator i=symbollist.begin();i!=symbollist.end();i++)
	{
		cout<<i->first<<endl;
	}
	cout<<"**************SymbolTable****************"<<endl;
}


/*
int main()
{
	symbol::table tb=symbol::table(); 
	symbol::entry e =symbol::entry::CONST_entry("abc",symbol::value_type::CHAR);
	tb.enter(e,0);
	tb.enter(e,1);
	e = tb.getentry("abc",1);
	cout<<e.addr<<endl;
	system("pause");
}
*/