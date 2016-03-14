#include "grammar.h"
grammar::grammarbuilder::grammarbuilder(tokenizer& _tk):tk(_tk),symbol_list()
{now_sym=token();last_sym=token();now_level=0;token::mapping();executable=true;}
void grammar::grammarbuilder::grammar_error_handler()
{
	executable=false;
	cout<<"line."<<now_line<<":";
	cout<<"missing tokens:"<<errorflag<<endl;
	cout<<"grammar error happened in statement:";
	if (tk.error_handler()) throw runtime_error("error");	
	cache.push_back(token(";",token::token_type::semicolon,now_line));
}
void grammar::grammarbuilder::getsym() 
{
	if (cache.empty())
	{
		do
		{
			last_sym=now_sym;
			now_sym=tk.getsym();
			if (now_sym.get_value()=="EOF")
			{
				break;
			}
			if (now_sym.get_type()==token::nul)
			{
				cout<<"line."<<now_line<<":";
				cout<<"syntax error: invalid token"<<endl;
				tk.error_handler();
				now_sym=last_sym;
			}
			else {break;}
		}
		while (true);
	}
	else
	{
		now_sym=*(cache.begin());
		cache.pop_front();
	}
	now_line = now_sym.get_line();
}
void grammar::grammarbuilder::push_back()
{
	cache.push_back(now_sym);
	if (now_sym.get_line()>last_sym.get_line())
	{
		now_line--;
	}
}
int grammar::grammarbuilder::block()
{
	int start_loc=0;
	bool constdecerror=false,vardecerror=false;
	Next_isTOKEN(token::constsym,constdecerror = !constdec();,push_back();/*pass*/)
	if (constdecerror)
	{
		cout<<"There is somethingwrong with constdec"<<endl;
		executable=false;
	}	
	getsym();
	if (now_sym.get_type()!=token::varsym&&now_sym.get_type()!=token::proceduresym&&now_sym.get_type()!=token::functionsym&&now_sym.get_type()!=token::beginsym)
	{
		cout<<"There is somethingwrong with constdec"<<endl;
		executable=false;
		while (now_sym.get_type()!=token::varsym&&now_sym.get_type()!=token::proceduresym&&now_sym.get_type()!=token::functionsym&&now_sym.get_type()!=token::beginsym)
		{
			getsym();
		}
		push_back();
	}
	else
	{
		push_back();
	}
		
	Next_isTOKEN(token::varsym,vardecerror = !vardec();,push_back();/*pass*/)
	if (vardecerror)	
	{
		cout<<"There is somethingwrong with vardec"<<endl;
		executable=false;
	}
	getsym();
	if (now_sym.get_type()!=token::proceduresym&&now_sym.get_type()!=token::functionsym&&now_sym.get_type()!=token::beginsym)
	{
		cout<<"There is somethingwrong with vardec"<<endl;
		executable=false;
 		while (now_sym.get_type()!=token::proceduresym&&now_sym.get_type()!=token::functionsym&&now_sym.get_type()!=token::beginsym)
		{
			getsym();
		}
		push_back();
	}
	else
	{
		push_back();
	}
	emit(BR,-1);
	int tmpint = internalcode.size()-1;
	bool go_on=true;
	while (go_on)
	{
		getsym();
		switch (now_sym.get_type())
		{
		case token::functionsym:push_back();functiondec();Next_isTOKEN(token::semicolon,,return false;)break;
		case token::proceduresym:push_back();proceduredec();Next_isTOKEN(token::semicolon,,return false;)break;
			default:
				push_back();
				go_on=false;
		}
	}
	internalcode[tmpint].target = "l@"+helper::type2type::int2string(labelsum);
	emit(LABEL,labelsum++);	
	if (!complex_statement()) 
	{
		throw runtime_error("error");
		return false;
	}
	return true;
}
bool grammar::grammarbuilder::constdec()
{
	string svalue;
	int bytevalue;
	symbol::value_type ty;
	Next_isTOKEN(token::ident,svalue = now_sym.get_value();,grammar_error_handler();return false;)
	Next_isTOKEN(token::eql,,grammar_error_handler();return false;)
	Next_isTOKEN(token::constchr,
		ty=symbol::value_type::CHAR;
		bytevalue=(int)(unsigned char)now_sym.get_value()[1];
		goto constdecEND;,
		push_back();)
	bytevalue=1;
	ty=symbol::value_type::INT;
	Next_isTOKEN(token::plus,bytevalue=now_sym.get_value()[0]=='+'?1:-1;,push_back();)
	Next_isTOKEN(token::number,bytevalue*=helper::type2type::string2int(now_sym.get_value());,grammar_error_handler();return false;)
	constdecEND:
	symbol_list.enter(symbol::entry::CONST_entry(svalue,ty),now_level);
	emit(LOADI,bytevalue);
	emit(LOADI,0);
	emit_CREATE(symbol_list.getentry(svalue,now_level).name,"4");
	emit(ADD,symbol_list.getentry(svalue,now_level).name);
	Next_isTOKEN(token::semicolon,return true;,push_back();)
	Next_isTOKEN(token::comma,return constdec();,push_back();grammar_error_handler();return false;)
	return true;
}
bool grammar::grammarbuilder::vardec() 
{
	vector<string> svalue;
	symbol::value_type vty;
	symbol::symbol_type sty;
	symbol::entry e;
	int capacity,shift2sp=0;	
	while (1)
	{
		Next_isTOKEN(token::ident,svalue.push_back(now_sym.get_value());,grammar_error_handler();return false;)
		Next_isTOKEN(token::comma,/*pass*/;,push_back();break;)
	}
	Next_isTOKEN(token::colon,/*pass*/;,grammar_error_handler();return false;)
	Next_isTOKEN(token::arraysym,,sty=symbol::symbol_type::VAR;push_back();capacity=1;goto vardecvaluetype;)
	sty=symbol::symbol_type::ARRAY;
	Next_isTOKEN(token::lbracket,,grammar_error_handler();return false;)
	Next_isTOKEN(token::number,,grammar_error_handler();return false;)
	capacity=helper::type2type::string2int(now_sym.get_value());
	Next_isTOKEN(token::rbracket,,;grammar_error_handler();return false;)
	Next_isTOKEN(token::ofsym,/*pass*/;,grammar_error_handler();return false;)
vardecvaluetype:
	Next_isTOKEN(token::integersym,vty=symbol::value_type::INT;goto vardecEND;,push_back();)
	Next_isTOKEN(token::charsym,vty=symbol::value_type::CHAR;goto vardecEND;,grammar_error_handler();return false;)
vardecEND:
	Next_isTOKEN(token::semicolon,/*pass*/,grammar_error_handler();return false;)
	while (!svalue.empty())
	{
		e = symbol::entry::VAR_entry(*(svalue.end()-1),capacity,vty,sty);
		emit_CREATE(e.name,helper::type2type::int2string(4*capacity));
		symbol_list.enter(e,now_level);
		svalue.pop_back();
	}
	Next_isTOKEN(token::ident,push_back();return vardec();,push_back();return true;)
	return true;
}
int grammar::grammarbuilder::expre()
{
	bool firstMinus=false;
	Iname op;
	int factorsum=0,tmpsum;
	Next_isTOKEN(token::plus,factorsum++;firstMinus=(now_sym.get_value()[0]=='-');,push_back();)
	tmpsum = term();
	if (!tmpsum) return 0;
	factorsum += tmpsum;
	if (firstMinus)
	{
		emit(LOADI,-1);
		emit(MUL,"");
	}
	while (true)
	{
		Next_isTOKEN(token::plus,op=(now_sym.get_value()[0]=='-'?SUB:ADD);,push_back();return factorsum;)
		tmpsum = term();
		if (!tmpsum) return 0;
		factorsum += tmpsum;
		emit(op,"");
	}
}
int grammar::grammarbuilder::term()
{
	symbol::entry e = symbol::entry::entry();
	Iname op;
	int factorsum=0,tmpsum;
	tmpsum = factor();
	if (!tmpsum) return 0;
	factorsum += tmpsum;
	while (true)
	{
		Next_isTOKEN(token::mul,,push_back();return factorsum;)
		switch (now_sym.get_value()[0])
		{
			case '*':op=MUL;break;
			case '/':op=DIV;break;
			default:return false;
		}
		tmpsum = factor();
		if (!tmpsum) return 0;
		factorsum += tmpsum;
		emit(op,"");
	}
}
bool grammar::grammarbuilder::proceduredec()
{
	vector<symbol::varlist> var_list;
	deque<string> svalue;
	symbol::varlist tmp;
	string name;
	symbol::entry e;
	bool isvar;
	symbol::value_type vty;
	Next_isTOKEN(token::proceduresym,,cout<<"dec error"<<endl;while (now_sym.get_type()!=token::semicolon) {getsym();}push_back();goto proceduredecEND;)
	Next_isTOKEN(token::ident,,cout<<"dec error"<<endl;while (now_sym.get_type()!=token::semicolon) {getsym();}push_back();goto proceduredecEND;)
	name = now_sym.get_value();
	Next_isTOKEN(token::lparen,,push_back();goto proceduredecEND;)
	while (true)
	{
		Next_isTOKEN(token::varsym,isvar=true;,isvar=false;push_back();)
		while (true)
		{
			Next_isTOKEN(token::ident,svalue.push_front(now_sym.get_value());,cout<<"dec error"<<endl;while (now_sym.get_type()!=token::semicolon) {getsym();}push_back();goto proceduredecEND;)
			Next_isTOKEN(token::colon,break;,push_back();)
			Next_isTOKEN(token::comma,,cout<<"dec error"<<endl;while (now_sym.get_type()!=token::semicolon) {getsym();}push_back();goto proceduredecEND;)
		}		
		getsym();
		switch (now_sym.get_type())
		{
		case token::integersym: vty=symbol::value_type::INT;break;
		case token::charsym:vty=symbol::value_type::CHAR;break;
		default:
			cout<<"dec error"<<endl;while (now_sym.get_type()!=token::semicolon) {getsym();}push_back();goto proceduredecEND;
		}
		while (!svalue.empty())
		{
			symbol::varlist tmp;
			symbol::entry e;
			tmp.isVar=isvar;tmp.name=*(svalue.end()-1);tmp.Val_type = vty;
			var_list.push_back(tmp);
			svalue.pop_back();		
		}
		Next_isTOKEN(token::rparen,break;,push_back();)
		Next_isTOKEN(token::semicolon,,cout<<"dec error"<<endl;while (now_sym.get_type()!=token::semicolon) {getsym();}push_back();goto proceduredecEND;)
	}	
proceduredecEND:
	Next_isTOKEN(token::semicolon,,while (now_sym.get_type()!=token::constsym&&now_sym.get_type()!=token::varsym&&now_sym.get_type()!=token::beginsym)
	{
		getsym();
	}push_back();)
	e.level=now_level;e.name=name;e.symtype=symbol::symbol_type::FUNCTION;e.parameter=var_list;
	e.label=labelsum;e.valtype=symbol::value_type::VOID;
	emit(LABEL,labelsum++);
	emit_FUN(e.name);
	symbol_list.enter(e,now_level);
	now_level++;
	symbol::function_stack.push_back(e.name);
	functionsituation.push_back(e.name);
	for (vector<symbol::varlist>::reverse_iterator index=var_list.rbegin();index!=var_list.rend();index++)
	{
		e = symbol::entry();
		e.level=now_level;e.name=index->name;e.symtype=symbol::symbol_type::VAR;e.valtype=index->Val_type;
		symbol_list.enter(e,now_level);//值已经在调用时放入了栈中
		emit_CREATE(e.name,"4");
		emit_tar(e.name);
	}
	if (!block()) return false;
	for (vector<symbol::varlist>::iterator index=var_list.begin();index!=var_list.end();index++)
	{
		if (index->isVar)
		{
			emit_par(index->name);
		}
	}
	symbol_list.erasenowlevel(now_level);
	emit("return");
	//***********************************************************************
	now_level--;
	symbol::function_stack.pop_back();
	return true;
}
bool grammar::grammarbuilder::functiondec()
{
	vector<symbol::varlist> var_list;
	deque<string> svalue;
	symbol::varlist tmp;
	string name;
	symbol::entry e;
	bool isvar;
	symbol::value_type vty;
	Next_isTOKEN(token::functionsym,,return false;)
	Next_isTOKEN(token::ident,,return false;)
	name = now_sym.get_value();
	shift2sp = 0;
	Next_isTOKEN(token::lparen,,push_back();goto functiondecEND;)
	while (true)
	{
		Next_isTOKEN(token::varsym,isvar=true;,isvar=false;push_back();)
		while (true)
		{
			Next_isTOKEN(token::ident,svalue.push_front(now_sym.get_value());,cout<<"dec error!"<<endl;executable=false;while (now_sym.get_type()!=token::colon) {getsym();}push_back();goto functiondecEND;)
			Next_isTOKEN(token::colon,break;,push_back();)
			Next_isTOKEN(token::comma,,cout<<"dec error!"<<endl;executable=false;while (now_sym.get_type()!=token::colon) {getsym();}push_back();goto functiondecEND;)
		}		
		getsym();
		switch (now_sym.get_type())
		{
		case token::integersym: vty=symbol::value_type::INT;break;
		case token::charsym:vty=symbol::value_type::CHAR;break;
		default:
			cout<<"dec error!"<<endl;executable=false;while (now_sym.get_type()!=token::colon) {getsym();}push_back();goto functiondecEND;
		}
		while (!svalue.empty())
		{
			symbol::varlist tmp;
			symbol::entry e;
			tmp.isVar=isvar;tmp.name=*(svalue.end()-1);tmp.Val_type = vty;
			var_list.push_back(tmp);
			svalue.pop_back();		
		}
		Next_isTOKEN(token::rparen,break;,push_back();)
		Next_isTOKEN(token::semicolon,,cout<<"dec error!"<<endl;executable=false;while (now_sym.get_type()!=token::colon) {getsym();}push_back();goto functiondecEND;)
	}	
functiondecEND:
	Next_isTOKEN(token::colon,,executable=false;cout<<"dec error"<<endl;goto here;)
	getsym();
	switch (now_sym.get_type())
	{
		case token::integersym:e.valtype=symbol::value_type::INT;break;
		case token::charsym:e.valtype=symbol::value_type::CHAR;break;
	default:
		e.valtype=symbol::value_type::INT;
		executable=false;cout<<"dec error"<<endl;goto here;
	}
here:
	Next_isTOKEN(token::semicolon,,executable=false;cout<<"dec error";while (now_sym.get_type()!=token::constsym&&now_sym.get_type()!=token::varsym&&now_sym.get_type()!=token::beginsym)
	{
		getsym();
	}push_back();)
	e.level=now_level;e.name=name;e.symtype=symbol::symbol_type::FUNCTION;e.parameter=var_list;
	e.label=labelsum;
	symbol_list.enter(e,now_level);
	symbol::entry functe=e;
	emit(LABEL,labelsum++);
	emit_FUN(e.name);
	functionsituation.push_back(name);
	symbol::function_stack.push_back(e.name);
	now_level++;
	for (vector<symbol::varlist>::reverse_iterator index=var_list.rbegin();index!=var_list.rend();index++)
	{
		e = symbol::entry();
		e.level=now_level;e.name=index->name;e.symtype=symbol::symbol_type::VAR;e.valtype=index->Val_type;
		symbol_list.enter(e,now_level);//值已经在调用时放入了栈中
		emit_CREATE(e.name,"4");
		emit_tar(e.name);
	}
	e = symbol::entry();
	e.level=now_level;e.name=functe.name;e.symtype=symbol::symbol_type::VAR;e.valtype=functe.valtype;
	symbol_list.enter(e,now_level);
	emit_CREATE(e.name,"4");
	if (!block()) return false;
	for (vector<symbol::varlist>::iterator index=var_list.begin();index!=var_list.end();index++)
	{
		if (index->isVar)
		{
			emit_par(index->name);
		}
	}	
	emit_par(functe.name);
	emit("return");
	symbol_list.erasenowlevel(now_level);
	//***********************************************************************
	functionsituation.pop_back();
	now_level--;
	symbol::function_stack.pop_back();
	return true;
}
bool grammar::grammarbuilder::condition()
{
	Iname op;
	if (!expre()) return false;
	Next_isTOKEN(token::leq,op=LEQ;goto conditionEND;,push_back();)
	Next_isTOKEN(token::lss,op=LSS;goto conditionEND;,push_back();)
	Next_isTOKEN(token::gtr,op=GRT;goto conditionEND;,push_back();)
	Next_isTOKEN(token::geq,op=GQL;goto conditionEND;,push_back();)
	Next_isTOKEN(token::eql,op=EQL;goto conditionEND;,push_back();)
	Next_isTOKEN(token::neq,op=NEQ;goto conditionEND;,return false;)
conditionEND:
	if (!expre()) return false;
	emit(op,"");
	return true;
}
bool grammar::grammarbuilder::statement()
{
	getsym();
	switch (now_sym.get_type())
	{
		case token::ident:push_back();if (!becoming_statement()) return false;break;
		case token::whilesym:push_back();if (!while_statement()) return false;break;
		case token::ifsym:push_back();if (!if_statement()) return false;break;
		case token::beginsym:push_back();if (!complex_statement()) return false;break;
		case token::forsym:push_back();if (!for_statement()) return false;break;
		case token::endsym:push_back();return true;break;
		case token::semicolon:if (!statement()) return false;break;
		case token::writesym:push_back();if (!write_statement()) return false;break;
		case token::readsym:push_back();if (!read_statement()) return false;break;
		default:
			push_back();return false;
	}
	return true;
}
bool grammar::grammarbuilder::write_statement()
{
	int tmpsum;
	Next_isTOKEN(token::writesym,,return false;);
	Next_isTOKEN(token::lparen,,return false;);
	Next_isTOKEN(token::conststr,,push_back();goto write_expre;);
	emit_str(now_sym.get_value());
	Next_isTOKEN(token::comma,,push_back();goto write_end;)
write_expre:
	tmpsum = expre();
	if (!tmpsum) return 0;
	if (tmpsum==1)
	{
		emit_char();
	}
	else
	{
		emit_expre();
	}
write_end:
 	Next_isTOKEN(token::rparen,return true;,return false;);
}
bool grammar::grammarbuilder::read_statement()
{
	Next_isTOKEN(token::readsym,,return false;);
	Next_isTOKEN(token::lparen,,return false;);
	Next_isTOKEN(token::ident,,return false;);
	emit_read(symbol_list.getentry(now_sym.get_value(),now_level).name);
	while (true)
	{
		Next_isTOKEN(token::comma,,push_back();break;);
		Next_isTOKEN(token::ident,,return false;);
		emit_read(symbol_list.getentry(now_sym.get_value(),now_level).name);
	}
	Next_isTOKEN(token::rparen,,return false;);
	return true;
}
bool grammar::grammarbuilder::becoming_statement()
{
	symbol::entry e;
	string result,ans;
	string target;
	Next_isTOKEN(token::ident,/*pass*/,return false;)
	e = symbol_list.getentry(now_sym.get_value(),now_level);
	if (e.loop)
	{
		if (errorflag=="")
		{
			cout<<now_line<<":loop identifier is unchangible!"<<endl;
			executable=false;
		}
	}
	if (e.symtype==symbol::symbol_type::FUNCTION) goto becoming_statementpro;
	result = e.name;
	if (e.symtype==symbol::symbol_type::VAR) {goto becoming_statementEND;}
	Next_isTOKEN(token::lbracket,,return false;)
	if (!expre()){return false;}
	Next_isTOKEN(token::rbracket,,return false;)
	Next_isTOKEN(token::becomes,/*pass*/,return false;)
	if (!expre()){return false;}
	emit_ADDRSHIFTarc(e.name);
	return true;
becoming_statementEND:
	Next_isTOKEN(token::becomes,/*pass*/,return false;)
	if (!expre()){return false;}
	emit(LOADI,0);
	emit(ADD,result);
	return true;
becoming_statementpro:
	vector<symbol::varlist> var_list=e.parameter;
	vector<string> change;
	deque<quartral_element> recoverqe;
	target = helper::type2type::int2string(e.label)+'@'+helper::type2type::int2string(e.level);
	if (var_list.empty()) goto becoming_statementproEND;
	Next_isTOKEN(token::lparen,,return false;)
	for (vector<symbol::varlist>::iterator index=var_list.begin();index!=var_list.end();index++)
	{
		if (index->isVar)
		{
			Next_isTOKEN(token::ident,,return false;)
			symbol::entry se = symbol_list.getentry(now_sym.get_value(),now_level);
			if (se.symtype!=symbol::symbol_type::VAR&&se.symtype!=symbol::symbol_type::ARRAY) return false;
			if (se.symtype==symbol::symbol_type::VAR)
			{
				emit_par(now_sym.get_value());
				change.push_back(now_sym.get_value());
			}
			else
			{
				Next_isTOKEN(token::lbracket,,return false;)
				if (!expre()) return false;
				Next_isTOKEN(token::rbracket,,return false;)
				string idx = *stack_op.rbegin();
				emit_ADDRSHIFT(se.name);
				ans = *stack_op.rbegin();
				stack_op.pop_back();
				if (ans.rfind(':')>=0)
				{
					ans = ans.substr(ans.rfind(':')+1,(ans.length())-ans.rfind(':'));
				}				
				quartral_element qe;
				qe.op=Iname::ADDRSHIFTarc;qe.first=idx;qe.second=ans;qe.target=se.name;
				recoverqe.push_back(qe);
				change.push_back(ans);
				emit_par(ans);
			}
		}
		else
		{
			if (!expre()) return false;
			ans = *stack_op.rbegin();
			if (ans.rfind(':')>=0)
			{
				ans = ans.substr(ans.rfind(':')+1,(ans.length())-ans.rfind(':'));
			}
			emit_par(ans);
			stack_op.pop_back();
		}
		getsym();
		switch (now_sym.get_type())
		{
		case token::comma:if (index==var_list.end()-1) return false;break;
		case token::rparen:if (index!=var_list.end()-1) return false;break;
		default:return false;
		}
	}
becoming_statementproEND:
	emit(CALL,target);
	while (!change.empty())
	{
		emit_tar(*change.rbegin());
		change.pop_back();
	}
	while (!recoverqe.empty())
	{
		internalcode.push_back(*recoverqe.begin());
		recoverqe.pop_front();
	}
	return true;
}

bool grammar::grammarbuilder::complex_statement()
{
	Next_isTOKEN(token::beginsym,/*pass*/,return false;)
	if (!statement()) 
	{
		grammar_error_handler();
	}
	while (true)
	{
		Next_isTOKEN(token::semicolon,,push_back();Next_isTOKEN(token::endsym,return true;,grammar_error_handler();))		
		if (!statement()){grammar_error_handler();}
	}
	
	return true;
}
bool grammar::grammarbuilder::for_statement()
{
	symbol::entry e;
	string result;
	Iname op;
	int loop;
	Next_isTOKEN(token::forsym,/*pass*/,return false;)
	Next_isTOKEN(token::ident,/*pass*/,return false;)
	e =symbol_list.getentry(now_sym.get_value(),now_level);
	result = e.name;
	symbol_list.reverseloop(now_sym.get_value(),now_level);
	Next_isTOKEN(token::becomes,/*pass*/,return false;)
	if (!expre()){return false;}
	getsym();
	switch (now_sym.get_type())
	{
		case token::tosym: op=ADDi;break;
		case token::downtosym: op=SUBi;break;
	default:
		return false;
	}
	emit(op,-1);
	emit(LOADI,0);
	emit(ADD,result);
	loop = labelsum;
	emit(LABEL,labelsum++);
	emit(LOAD,result);
	emit(op,1);
	emit(LOADI,0);
	emit(ADD,result);
	if (!expre()){return false;}
	Next_isTOKEN(token::dosym,/*pass*/,return false;)
	statement();
	stack_op.push_back(result);
	emit(BRNEQ,loop);
	symbol_list.reverseloop(now_sym.get_value(),now_level);
	return true;
}
bool grammar::grammarbuilder::while_statement()
{
	int loopbegin=labelsum,loopend=labelsum+1;
	labelsum+=2;	
	Next_isTOKEN(token::whilesym,/*pass*/,return false;)
	emit(LABEL,loopbegin);
	if (!condition()) {return false;}
	Next_isTOKEN(token::dosym,,return false;)
	emit(BRIFFALSE,loopend);
	if (!statement()) {return false;}
	emit(BR,loopbegin);
	emit(LABEL,loopend);
	return true;
}
bool grammar::grammarbuilder::if_statement()
{
	int label1=labelsum,label2;
	labelsum++;
	Next_isTOKEN(token::ifsym,/*pass*/,return false;)
	if (!condition()) return false;
	emit(BRIFFALSE,label1);
	Next_isTOKEN(token::thensym,,return false;)
	if (!statement()) {return false;}
	Next_isTOKEN(token::elsesym,label2=labelsum++;emit(BR,label2);emit(LABEL,label1);,emit(LABEL,label1);push_back();return true;)
	if (!statement()) {return false;}
	emit(LABEL,label2);
	return true;	
}
int grammar::grammarbuilder::factor()
{
	symbol::entry e,te;
	string result,ans;
	string target;
	int notcharplus1=1;
	/*
	Next_isTOKEN(token::constchr,stack_op.push_back(helper::type2type::int2string((int)now_sym.get_value()[0]));notcharplus1=0;return true;,push_back();)
	*/
	Next_isTOKEN(token::lparen,goto factorexpre;,push_back();)
	Next_isTOKEN(token::number,goto factornumber;,push_back();)
	Next_isTOKEN(token::ident,/*pass*/,return false;)
	e = symbol_list.getentry(now_sym.get_value(),now_level);
	result = e.name;
	notcharplus1 = (e.valtype==symbol::value_type::CHAR)?0:1;
	if (e.symtype==symbol::symbol_type::FUNCTION) {goto factorfun;}
	if (e.symtype==symbol::symbol_type::VAR||e.symtype==symbol::symbol_type::CONST) 
	{
		for (int i=now_level;i>=0;i--)
		{
			cout.clear(ios::failbit);
			te = symbol_list.getentry(now_sym.get_value(),i);
			cout.clear(ios::goodbit);
			if (te.symtype==symbol::symbol_type::FUNCTION)
			{
				e=te;
				notcharplus1 = (te.valtype==symbol::value_type::CHAR)?0:1;
				goto factorfun;
			}
		}
		goto factorvar;
	}
	
	Next_isTOKEN(token::lbracket,,return false;)
	if (!expre()){return false;}
	emit_ADDRSHIFT(e.name);
	result=*stack_op.rbegin();
	stack_op.pop_back();
	Next_isTOKEN(token::rbracket,,return false;)
factorvar:
	emit(LOAD,result);
	return 1+notcharplus1;
factornumber:
	emit(LOADI,helper::type2type::string2int(now_sym.get_value()));
	return 1+notcharplus1;
factorexpre:
	if (!expre()) return false;
	Next_isTOKEN(token::rparen,,return false;)
	return 1+notcharplus1;
factorfun:
	vector<symbol::varlist> var_list=e.parameter;
	vector<string> change;
	deque<quartral_element> recoverqe;
	target = helper::type2type::int2string(e.label)+'@'+helper::type2type::int2string(e.level);
	if (var_list.empty()) goto factorfunend;
	Next_isTOKEN(token::lparen,,return false;)
	for (vector<symbol::varlist>::iterator index=var_list.begin();index!=var_list.end();index++)
	{
		if (index->isVar)
		{
			Next_isTOKEN(token::ident,,return false;)
			symbol::entry se = symbol_list.getentry(now_sym.get_value(),now_level);
			if (se.symtype!=symbol::symbol_type::VAR&&se.symtype!=symbol::symbol_type::ARRAY) return false;
			if (se.symtype==symbol::symbol_type::VAR)
			{
				emit_par(now_sym.get_value());
				change.push_back(now_sym.get_value());
			}
			else
			{
				Next_isTOKEN(token::lbracket,,return false;)
				if (!expre()) return false;
				Next_isTOKEN(token::rbracket,,return false;)
				string idx = *stack_op.rbegin();
				emit_ADDRSHIFT(se.name);
				ans = *stack_op.rbegin();
				stack_op.pop_back();
				if (ans.rfind(':')>=0)
				{
					ans = ans.substr(ans.rfind(':')+1,(ans.length())-ans.rfind(':'));
				}				
				quartral_element qe;
				qe.op=Iname::ADDRSHIFTarc;qe.first=idx;qe.second=ans;qe.target=se.name;
				recoverqe.push_back(qe);
				change.push_back(ans);
				emit_par(ans);
			}
		}
		else
		{
			if (!expre()) return false;
			ans = *stack_op.rbegin();
			if (ans.rfind(':')>=0)
			{
				ans = ans.substr(ans.rfind(':')+1,(ans.length())-ans.rfind(':'));
			}
			emit_par(ans);
			stack_op.pop_back();
		}
		getsym();
		switch (now_sym.get_type())
		{
			case token::comma:if (index==var_list.end()-1) return false;break;
			case token::rparen:if (index!=var_list.end()-1) return false;break;
			default:
			cout<<now_sym.get_value();return false;
		}
	}
factorfunend:
	emit(CALLFUN,target);
	while (!change.empty())
	{
		emit_tar(*change.rbegin());
		change.pop_back();
	}
	while (!recoverqe.empty())
	{
		internalcode.push_back(*recoverqe.begin());
		recoverqe.pop_front();
	}
	return 1+notcharplus1;
}
bool grammar::grammarbuilder::program()
{
	emit_FUN("program");
	try
	{
		if (!block()) return false;
		LINEinit(&now_line);
		join();
	}
	catch (runtime_error& ex)
	{
		cout<<"fetal error happened in line:";
		cout<<now_line<<endl;
		cout<<tk.currentunit<<endl;
		executable=false;
	}
	emit("RETURN");
	Next_isTOKEN(token::period,return true;,executable=false;cout<<"period is not found!";)
}

