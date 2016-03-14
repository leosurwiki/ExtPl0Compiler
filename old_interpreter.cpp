#include "interpreter.h"
void interpreter::interpreter::init_sp_fp_ra()
{
	fout<<"main:"<<endl;
	fout<<"add $30,$0,$29"<<endl;
	fout<<"li $31,0x00400018"<<endl;
	fout<<"add $gp,$29,$0"<<endl;
	fout<<"addi $gp,$gp,-4096"<<endl;
}
interpreter::interpreter::interpreter(std::ostream &_fout):fout(_fout)
{init_sp_fp_ra();shift2sp=0;}
bool interpreter::interpreter::ifalloc(string s)
{
	if (s=="") return false;
	if ((s.find('@')!=string::npos&&s[0]=='t')||s.find('@')==string::npos)
	{
		return true;
	}
	return false;	
}
bool interpreter::interpreter::iflabel(string s)
{
	if (s.find('@')!=string::npos&&s[0]=='l')
	{
		return true;
	}
	return false;
}
bool interpreter::interpreter::iftemp(string s)
{
	if (s.find('@')!=string::npos&&s[0]=='t')
	{
		return true;
	}
	return false;
}
bool interpreter::interpreter::iflitteral(string s)
{
	if (s.find('@')!=string::npos&&s[0]=='i')
	{
		return true;
	}
	return false;
}
int interpreter::interpreter::literal2int(string s)
{
	s.erase(0,2);
	return helper::type2type::string2int(s);
}

void interpreter::interpreter::get_shift(vector<quartral_element> &vqe)
{
	int shift2sp = 0;
	vector<quartral_element*> nowfun;
	vector<quartral_element>::iterator idx;
	for (idx=vqe.begin();idx!=vqe.end();idx++){cout<<"------------"<<gen::I2string[idx->op]<<' '<<idx->first<<' '<<idx->second<<' '<<idx->target<<"-------"<<endl;}
	for (int i=0;i<vqe.size();i++)
	{
		if (vqe[i].op==Iname::NOD) continue;
		if (vqe[i].op==Iname::FUN)
		{
			spfunshift.push_back(0);
			nowfun.push_back(&vqe[i]);
		}
		if (vqe[i].op==Iname::CREATE)
		{
			*spfunshift.rbegin()+=helper::type2type::string2int(vqe[i].target);		
		}
		if (vqe[i].op==Iname::RETURN)
		{
			(*nowfun.rbegin())->first = helper::type2type::int2string(*spfunshift.rbegin());
			nowfun.pop_back();
			spfunshift.pop_back();
		}
		if (vqe[i].first[0]=='t'&&vqe[i].first[1]=='@')
		{
			if (string2shiftfromsp.find(vqe[i].first)==string2shiftfromsp.end())
			{
				string2shiftfromsp[vqe[i].first]=*spfunshift.rbegin();
				*spfunshift.rbegin()+=4;				
			}
		}
		if (vqe[i].second[0]=='t'&&vqe[i].second[1]=='@')
		{
			if (string2shiftfromsp.find(vqe[i].second)==string2shiftfromsp.end())
			{
				string2shiftfromsp[vqe[i].second]=*spfunshift.rbegin();
				*spfunshift.rbegin()+=4;				
			}
		}
		if (vqe[i].target[0]=='t'&&vqe[i].target[1]=='@')
		{
			if (string2shiftfromsp.find(vqe[i].target)==string2shiftfromsp.end())
			{
				string2shiftfromsp[vqe[i].target]=*spfunshift.rbegin();
				*spfunshift.rbegin()+=4;				
			}
		}
	}
}
string interpreter::interpreter::get_trait_string(int x)
{
	if (x==0) return "";
	return functionstack[x-1];
}//functionstack前x项的特征序列
void interpreter::interpreter::symbol_load(string s)
{
	if (iftemp(s))
	{
		fout<<"lw $t0,-"<<string2shiftfromsp[s]<<"($sp)"<<endl;
		return ;
	}
	if (iflitteral(s))
	{
		fout<<"li $t0,"<<literal2int(s)<<endl;
		return;
	}
	for (int i=functionstack.size();i>=0;i--)
	{
		if (s.find(get_trait_string(i))!=string::npos||i==-1)
		{
			if (i==functionstack.size())
			{
				if ()fout<<"lw $t0,-"<<string2shiftfromsp[s]<<"($sp)"<<endl;break;
			}			
			else
			{
				int tmp=(i+1)*4;
				fout<<"lw $t0,"<<tmp<<"($fp)"<<endl;
				fout<<"lw $t0,-"<<string2shiftfromsp[s]<<"($t0)"<<endl;
			}
			break;
		}
	}
}
void interpreter::interpreter::symbol_save(string s)
{
	if (iftemp(s))
	{
		fout<<"sw $t8,-"<<string2shiftfromsp[s]<<"($sp)"<<endl;
		return ;
	}
	for (int i=functionstack.size();i>=0;i--)
	{
		if (s.find(get_trait_string(i))!=string::npos||i==-1)
		{
			if (i==functionstack.size())
			{
				fout<<"sw $t8,-"<<string2shiftfromsp[s]<<"($sp)"<<endl;break;
			}			
			else
			{
				int tmp=(i+1)*4;
				fout<<"lw $t0,"<<tmp<<"($fp)"<<endl;
				fout<<"sw $t8,-"<<string2shiftfromsp[s]<<"($t0)"<<endl;
			}
			break;
		}
	}
}
void interpreter::interpreter::interpret_ADD(quartral_element qe)
{
	if (ifalloc(qe.first))
	{
		symbol_load(qe.first);
		fout<<"add $t7,$t0,$0"<<endl;
	}
	else{fout<<"li $t7,"<<literal2int(qe.first)<<endl;}
	if (ifalloc(qe.second))
	{
		symbol_load(qe.second);
		fout<<"add $t8,$t0,$0"<<endl;
	}
	else{fout<<"li $t8,"<<literal2int(qe.second)<<endl;}
	fout<<"add $t8,$t7,$t8"<<endl;
	symbol_save(qe.target);
}
void interpreter::interpreter::interpret_SUB(quartral_element qe)
{
	if (ifalloc(qe.first))
	{
		symbol_load(qe.first);
		fout<<"add $t7,$t0,$0"<<endl;
	}
	else{fout<<"li $t7,"<<literal2int(qe.first)<<endl;}
	if (ifalloc(qe.second))
	{
		symbol_load(qe.second);
		fout<<"add $t8,$t0,$0"<<endl;
	}
	else{fout<<"li $t8,"<<literal2int(qe.second)<<endl;}
	fout<<"sub $t8,$t7,$t8"<<endl;
	symbol_save(qe.target);
}
void interpreter::interpreter::interpret_MUL(quartral_element qe)
{
	if (ifalloc(qe.first))
	{
		symbol_load(qe.first);
		fout<<"add $t7,$t0,$0"<<endl;
	}
	else{fout<<"li $t7,"<<literal2int(qe.first)<<endl;}
	if (ifalloc(qe.second))
	{
		symbol_load(qe.second);
		fout<<"add $t8,$t0,$0"<<endl;
	}
	else{fout<<"li $t8,"<<literal2int(qe.second)<<endl;}
	fout<<"mul $t8,$t7,$t8"<<endl;
	symbol_save(qe.target);
}
void interpreter::interpreter::interpret_DIV(quartral_element qe)
{
	if (ifalloc(qe.first))
	{
		symbol_load(qe.first);
		fout<<"add $t7,$t0,$0"<<endl;
	}
	else{fout<<"li $t7,"<<literal2int(qe.first)<<endl;}
	if (ifalloc(qe.second))
	{
		symbol_load(qe.second);
		fout<<"add $t8,$t0,$0"<<endl;
	}
	else{fout<<"li $t8,"<<literal2int(qe.second)<<endl;}
	fout<<"div $t8,$t7,$t8"<<endl;
	symbol_save(qe.target);
}
void interpreter::interpreter::interpret_BR(quartral_element qe)
{
	fout<<"j "<<"LABEL"<<qe.target.substr(2,qe.target.size()-2)<<endl;
}
void interpreter::interpreter::interpret_LABEL(quartral_element qe)
{
	fout<<"LABEL"<<qe.target.substr(2,qe.target.size()-2)<<":"<<endl;}
void interpreter::interpreter::interpret_BREQL(quartral_element qe)
{
	if (ifalloc(qe.first))
	{
		symbol_load(qe.first);
		fout<<"add $t7,$t0,$0"<<endl;
	}
	else{fout<<"li $t7,"<<literal2int(qe.first)<<endl;}
	if (ifalloc(qe.second))
	{
		symbol_load(qe.second);
		fout<<"add $t8,$t0,$0"<<endl;
	}
	else{fout<<"li $t8,"<<literal2int(qe.second)<<endl;}
	fout<<"beq $t8,$t7 "<<"LABEL"<<qe.target.substr(2,qe.target.size()-2)<<endl;		
}
void interpreter::interpreter::interpret_BRNEQ(quartral_element qe)
{
	if (ifalloc(qe.first))
	{
		symbol_load(qe.first);
		fout<<"add $t7,$t0,$0"<<endl;
	}
	else{fout<<"li $t7,"<<literal2int(qe.first)<<endl;}
	if (ifalloc(qe.second))
	{
		symbol_load(qe.second);
		fout<<"add $t8,$t0,$0"<<endl;
	}
	else{fout<<"li $t8,"<<literal2int(qe.second)<<endl;}
	fout<<"bne $t8,$t7 "<<"LABEL"<<qe.target.substr(2,qe.target.size()-2)<<endl;		
}
void interpreter::interpreter::interpret_BRLSS(quartral_element qe)
{
	if (ifalloc(qe.first))
	{
		symbol_load(qe.first);
		fout<<"add $t7,$t0,$0"<<endl;
	}
	else{fout<<"li $t7,"<<literal2int(qe.first)<<endl;}
	if (ifalloc(qe.second))
	{
		symbol_load(qe.second);
		fout<<"add $t8,$t0,$0"<<endl;
	}
	else{fout<<"li $t8,"<<literal2int(qe.second)<<endl;}
	fout<<"sub $t8,$t7,$t8"<<endl;
	fout<<"bltz $t8,"<<"LABEL"<<qe.target.substr(2,qe.target.size()-2)<<endl;
}
void interpreter::interpreter::interpret_BRLEQ(quartral_element qe)
{
	if (ifalloc(qe.first))
	{
		symbol_load(qe.first);
		fout<<"add $t7,$t0,$0"<<endl;
	}
	else{fout<<"li $t7,"<<literal2int(qe.first)<<endl;}
	if (ifalloc(qe.second))
	{
		symbol_load(qe.second);
		fout<<"add $t8,$t0,$0"<<endl;
	}
	else{fout<<"li $t8,"<<literal2int(qe.second)<<endl;}
	fout<<"sub $t8,$t7,$t8"<<endl;
	fout<<"blez $t8,"<<"LABEL"<<qe.target.substr(2,qe.target.size()-2)<<endl;
}
void interpreter::interpreter::interpret_BRGRT(quartral_element qe)
{
	if (ifalloc(qe.first))
	{
		symbol_load(qe.first);
		fout<<"add $t7,$t0,$0"<<endl;
	}
	else{fout<<"li $t7,"<<literal2int(qe.first)<<endl;}
	if (ifalloc(qe.second))
	{
		symbol_load(qe.second);
		fout<<"add $t8,$t0,$0"<<endl;
	}
	else{fout<<"li $t8,"<<literal2int(qe.second)<<endl;}
	fout<<"sub $t8,$t7,$t8"<<endl;
	fout<<"bgtz $t8,"<<"LABEL"<<qe.target.substr(2,qe.target.size()-2)<<endl;
}
void interpreter::interpreter::interpret_BRGQL(quartral_element qe)
{
	if (ifalloc(qe.first))
	{
		symbol_load(qe.first);
		fout<<"add $t7,$t0,$0"<<endl;
	}
	else{fout<<"li $t7,"<<literal2int(qe.first)<<endl;}
	if (ifalloc(qe.second))
	{
		symbol_load(qe.second);
		fout<<"add $t8,$t0,$0"<<endl;
	}
	else{fout<<"li $t8,"<<literal2int(qe.second)<<endl;}
	fout<<"sub $t8,$t7,$t8"<<endl;
	fout<<"bgez $t8,"<<"LABEL"<<qe.target.substr(2,qe.target.size()-2)<<endl;
}
void interpreter::interpreter::interpret_PAR(quartral_element qe)
{
	fout<<"addi $gp,$gp,4"<<endl;
	symbol_load(qe.target);
	fout<<"sw $t0,0($gp)"<<endl;
}
void interpreter::interpreter::interpret_TAR(quartral_element qe)
{
	fout<<"lw $t8,0($gp)"<<endl;
	fout<<"addi $gp,$gp,-4"<<endl;
	symbol_save(qe.target);	
}
void interpreter::interpreter::interpret_CREATE(quartral_element qe)
{
	string2shiftfromsp[qe.first] =shift2sp;
	shift2sp+=helper::type2type::string2int(qe.target);
}
void interpreter::interpreter::interpreter_subprogram(quartral_element qe)
{
	fout<<"sw $sp,4($gp)"<<endl;//第i层的$sp位置临时存入$gp+4（不需要退栈）
	fout<<"addi $sp,$sp,4"<<endl;
	fout<<"sw $fp,0($sp)"<<endl;//第一格放上一层$fp的位置
	fout<<"sw $sp,8($gp)"<<endl;//临时存入该层$fp的开始位置
	if (functionstack.size()==helper::type2type::string2int(qe.second))
	{
		for (int i=0;i<helper::type2type::string2int(qe.second);i++)
		{
			fout<<"addi $sp,$sp,4"<<endl;
			fout<<"addi $fp,$fp,4"<<endl;
			fout<<"lw $t0,0($fp)"<<endl;
			fout<<"sw $t0,0($sp)"<<endl;
		}
		fout<<"addi $sp,$sp,4"<<endl;
		fout<<"lw $t0,4($gp)"<<endl;
		fout<<"sw $t0,0($sp)"<<endl;
		fout<<"lw $t0,8($gp)"<<endl;
		fout<<"addi $fp,$t0,0"<<endl;
	}//如果是进入更内一层则复制前者的display区然后再添加一层，最后$fp指向最开始保存的位置
	else
	{
		for (int i=0;i<=helper::type2type::string2int(qe.second);i++)
		{
			fout<<"addi $fp,$fp,4"<<endl;
			fout<<"lw $t0,0($fp)"<<endl;
			fout<<"addi $sp,$sp,4"<<endl;
			fout<<"sw $t0,0($sp)"<<endl;
		}
		fout<<"lw $t0,8($gp)"<<endl;
		fout<<"addi $fp,$t0,0"<<endl;
	}//之后的helper::type2type::string2int(qe.second)放入前者display区的前helper::type2type::string2int(qe.second)项
	fout<<"addi $sp,$sp,4"<<endl;
	fout<<"sw $ra,0($sp)"<<endl;
	fout<<"jal "<<"LABEL"<<qe.first<<endl;
	fout<<"lw $ra,0($sp)"<<endl;
	fout<<"addi $sp,$sp,-4"<<endl;
	for (int i=0;i<=helper::type2type::string2int(qe.second);i++)
	{
		fout<<"addi $sp,$sp,-4"<<endl;
	}//将helper::type2type::string2int(qe.second)层指针弹出
	fout<<"lw $fp,0($sp)"<<endl;//$fp归位
	fout<<"addi $sp,$sp,-4"<<endl;
}
void interpreter::interpreter::interpreter_fun(quartral_element qe)
{
	fout<<"sw $sp,4($gp)"<<endl;//第i层的$sp位置临时存入$gp+4（不需要退栈）
	fout<<"addi $sp,$sp,4"<<endl;
	fout<<"sw $fp,0($sp)"<<endl;//第一格放上一层$fp的位置
	fout<<"sw $sp,8($gp)"<<endl;//临时存入该层$fp的开始位置
	if (functionstack.size()==helper::type2type::string2int(qe.second))
	{
		for (int i=0;i<helper::type2type::string2int(qe.second);i++)
		{
			fout<<"addi $sp,$sp,4"<<endl;
			fout<<"addi $fp,$fp,4"<<endl;
			fout<<"lw $t0,0($fp)"<<endl;
			fout<<"sw $t0,0($sp)"<<endl;
		}
		fout<<"addi $sp,$sp,4"<<endl;
		fout<<"lw $t0,4($gp)"<<endl;
		fout<<"sw $t0,0($sp)"<<endl;
		fout<<"lw $t0,8($gp)"<<endl;
		fout<<"addi $fp,$t0,0"<<endl;
	}//如果是进入更内一层则复制前者的display区然后再添加一层，最后$fp指向最开始保存的位置
	else
	{
		for (int i=0;i<=helper::type2type::string2int(qe.second);i++)
		{
			fout<<"addi $fp,$fp,4"<<endl;
			fout<<"lw $t0,0($fp)"<<endl;
			fout<<"addi $sp,$sp,4"<<endl;
			fout<<"sw $t0,0($sp)"<<endl;
		}
		fout<<"lw $t0,8($gp)"<<endl;
		fout<<"addi $fp,$t0,0"<<endl;
	}//之后的helper::type2type::string2int(qe.second)放入前者display区的前helper::type2type::string2int(qe.second)项
	fout<<"addi $sp,$sp,4"<<endl;
	fout<<"sw $ra,0($sp)"<<endl;
	fout<<"jal "<<"LABEL"<<qe.first<<endl;
	fout<<"lw $ra,0($sp)"<<endl;
	fout<<"addi $sp,$sp,-4"<<endl;
	for (int i=0;i<=helper::type2type::string2int(qe.second);i++)
	{
		fout<<"addi $sp,$sp,-4"<<endl;
	}//将helper::type2type::string2int(qe.second)层指针弹出
	fout<<"lw $fp,0($sp)"<<endl;//$fp归位
	fout<<"addi $sp,$sp,-4"<<endl;
	fout<<"lw $t8,0($gp)"<<endl;
	fout<<"addi $gp,$gp,-4"<<endl;
	symbol_save(qe.target);
}
void interpreter::interpreter::interpreter_ADDRSHIFT(quartral_element qe)
{
	string s=qe.target;
	for (int i=functionstack.size();i>=0;i--)
	{
		if (s.find(get_trait_string(i))!=string::npos||i==-1)
		{
			if (i==functionstack.size())
			{
				fout<<"addi $t8,$sp,-"<<string2shiftfromsp[s]<<endl;break;
			}			
			else
			{
				int tmp=(i+1)*4;
				fout<<"lw $t8,"<<tmp<<"($fp)"<<endl;
				fout<<"addi $t8,$t8,-"<<string2shiftfromsp[s]<<endl;
			}
			break;
		}
	}
	symbol_load(qe.first);
	fout<<"sll $t0,$t0,2"<<endl;
	fout<<"sub $t8,$t8,$t0"<<endl;
	fout<<"lw $t8,0($t8)"<<endl;
	symbol_save(qe.second);	
}
void interpreter::interpreter::interpreter_ADDRSHIFTarc(quartral_element qe)
{
	string s=qe.target;
	for (int i=functionstack.size();i>=0;i--)
	{
		if (s.find(get_trait_string(i))!=string::npos||i==-1)
		{
			if (i==functionstack.size())
			{
				fout<<"addi $t8,$sp,-"<<string2shiftfromsp[s]<<endl;break;
			}			
			else
			{
				int tmp=(i+1)*4;
				fout<<"lw $t8,"<<tmp<<"($fp)"<<endl;
				fout<<"addi $t8,$t8,-"<<string2shiftfromsp[s]<<endl;
			}
			break;
		}
	}
	symbol_load(qe.first);
	fout<<"sll $t0,$t0,2"<<endl;
	fout<<"sub $t8,$t8,$t0"<<endl;
	symbol_load(qe.second);
	fout<<"sw $t0,0($t8)"<<endl;
}
void interpreter::interpreter::interpreter_write(quartral_element qe)
{
	if (qe.target=="chr")
	{
		symbol_load(qe.first);
		fout<<"sw $t0,4($gp)"<<endl;
		fout<<"sw $0,8($gp)"<<endl;
		fout<<"addi $a0,$gp,4"<<endl;
		fout<<"li $v0,4"<<endl;
		fout<<"syscall"<<endl;
		return;
	}
	if (qe.target=="str")
	{
		fout<<"la $a0,str"<<data_area.size()<<endl;		
		fout<<"li $v0,4"<<endl;
		fout<<"syscall"<<endl;
		data_area.push_back(qe.first);				
	}
	else
	{
		symbol_load(qe.first);
		fout<<"add $a0,$0,$t0"<<endl;
		fout<<"li $v0,1"<<endl;
		fout<<"syscall"<<endl;
	}
}
void interpreter::interpreter::interpreter_read(quartral_element qe)
{
	fout<<"li $v0,5"<<endl;
	fout<<"syscall"<<endl;
	fout<<"add $t8,$0,$v0"<<endl;
	symbol_save(qe.first);
}
void interpreter::interpreter::deal_qe(quartral_element qe)
{
	cout<<"------------"<<gen::I2string[qe.op]<<' '<<qe.first<<' '<<qe.second<<' '<<qe.target<<"-------"<<endl;
	if (qe.op==FUN)
	{
		if (qe.target!="program") functionstack.push_back(qe.target);
		offset.push_back(qe.first);
		shift2sp =0;
		fout<<"addi $sp,$sp,"<<qe.first<<endl;
		return;
	}
	if (qe.op==RETURN)
	{
		if (!functionstack.empty())functionstack.pop_back();
		fout<<"addi $sp,$sp,-"<<*offset.rbegin()<<endl;
		offset.pop_back();
		fout<<"jr $ra"<<endl;
		return;
	}
	switch (qe.op)
	{
		case CREATE:interpret_CREATE(qe);break;
		case ADD:interpret_ADD(qe);break;
		case ADDi:interpret_ADD(qe);break;
		case SUB:interpret_SUB(qe);break;
		case SUBi:interpret_SUB(qe);break;
		case MUL:interpret_MUL(qe);break;
		case DIV:interpret_DIV(qe);break;
		case BR:interpret_BR(qe);break;
		case BREQL:interpret_BREQL(qe);break;
		case BRNEQ:interpret_BRNEQ(qe);break;
		case BRLSS:interpret_BRLSS(qe);break;
		case BRLEQ:interpret_BRLEQ(qe);break;
		case BRGRT:interpret_BRGRT(qe);break;
		case BRGQL:interpret_BRGQL(qe);break;
		case LABEL:interpret_LABEL(qe);break;
		case TAR:interpret_TAR(qe);break;
		case PAR:interpret_PAR(qe);break;
		case CALL:interpreter_subprogram(qe);break;
		case CALLFUN:interpreter_fun(qe);break;
		case ADDRSHIFT:interpreter_ADDRSHIFT(qe);break;
		case ADDRSHIFTarc:interpreter_ADDRSHIFTarc(qe);break;
		case WRITE:interpreter_write(qe);break;
		case READ:interpreter_read(qe);break;
		case NOD:break;
		default:system("pause");
	}
 	return;
}
void interpreter::interpreter::translate(vector<quartral_element> &vqe)
{
	get_shift(vqe);
	for (int i=0;i<vqe.size();i++)
	{
		deal_qe(vqe[i]);		
	}
	fout<<".data"<<endl;
	for (int i=0;i<data_area.size();i++)
	{
		fout<<"str"<<i<<":.asciiz"<<data_area[i]<<endl;
	}
}
