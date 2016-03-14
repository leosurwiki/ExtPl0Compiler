#include "pblock.h"
bool pblock::iscal(gen::quartral_element I)
{
	if (I.op==gen::ADD||I.op==gen::SUB||I.op==gen::MUL||I.op==gen::DIV)
	{
		return true;
	}
	if (I.op==gen::ADDi||I.op==gen::SUBi||I.op==gen::MULi||I.op==gen::DIVi)
	{
		return true;
	}
	if (I.op==gen::ADDRSHIFT||I.op==gen::ADDRSHIFTarc)
	{
		return true;
	}
	return false;
}
bool pblock::isgap(gen::quartral_element I)
{
	if (I.op==gen::BR||I.op==gen::BREQL||I.op==gen::BRGQL||I.op==gen::BRGRT||I.op==gen::BRIFFALSE||I.op==gen::BRIFTRUE)
	{
		return true;
	}
	if (I.op==gen::BRLEQ||I.op==gen::BRLSS||I.op==gen::BRNEQ)
	{
		return true;
	}
	if (I.op==gen::RETURN||I.op==gen::CALLFUN||I.op==gen::CALL)
	{
		return true;
	}
	return false;
}
bool pblock::isbr(gen::quartral_element I)
{
	if (I.op==gen::BREQL||I.op==gen::BRGQL||I.op==gen::BRGRT)
	{
		return true;
	}
	if (I.op==gen::BRLEQ||I.op==gen::BRLSS||I.op==gen::BRNEQ)
	{
		return true;
	}
	return false;
}
set<string> pblock::isdef(const vector<gen::quartral_element> &ic,int begin,int end)
{
	map<string,bool> check;
	set<string> result;
	for (size_t idx=begin;idx<end;idx++)
	{
		if (iscal(ic[idx]))
		{
			if (check.find(ic[idx].first)==check.end())
			{
				check[ic[idx].first]=false;
			}
			if (check.find(ic[idx].second)==check.end())
			{
				check[ic[idx].second]=false;
			}
			if (check.find(ic[idx].target)==check.end())
			{
				check[ic[idx].target]=true;
				result.insert(ic[idx].target);
			}
		}
		if (ic[idx].op==gen::TAR)
		{
			if (check.find(ic[idx].target)==check.end())
			{
				check[ic[idx].target]=true;
				result.insert(ic[idx].target);
			}		
		}
		if (ic[idx].op==gen::PAR)
		{
			if (check.find(ic[idx].target)==check.end())
			{
				check[ic[idx].target]=false;
			}
		}
		if (ic[idx].op==gen::WRITE)
		{
			if (check.find(ic[idx].first)==check.end())
			{
				check[ic[idx].first]=false;
			}
		}
		if (ic[idx].op==gen::READ)
		{
			if (check.find(ic[idx].first)==check.end())
			{
				check[ic[idx].first]=true;
				result.insert(ic[idx].first);
			}
		}
		if (isbr(ic[idx]))
		{
			if (check.find(ic[idx].first)==check.end())
			{
				check[ic[idx].first]=false;
			}
			if (check.find(ic[idx].second)==check.end())
			{
				check[ic[idx].second]=false;
			}
		}
	}
	for (set<string>::iterator idx=result.begin();idx!=result.end();)
	{
		if (isasr(*idx)) idx++;
		else
		{
			idx=result.erase(idx);
		}
	}
	return result;
}
set<string> pblock::isuse(const vector<gen::quartral_element> &ic,int begin,int end)
{
	map<string,bool> check;
	set<string> result;
	for (size_t idx=begin;idx<end;idx++)
	{
		if (iscal(ic[idx]))
		{
			if (check.find(ic[idx].first)==check.end())
			{
				check[ic[idx].first]=true;
				result.insert(ic[idx].first);
			}
			if (check.find(ic[idx].second)==check.end())
			{
				check[ic[idx].second]=true;
				result.insert(ic[idx].second);
			}
			if (check.find(ic[idx].target)==check.end())
			{
				check[ic[idx].target]=false;
			}
		}
		if (ic[idx].op==gen::TAR)
		{
			if (check.find(ic[idx].target)==check.end())
			{
				check[ic[idx].target]=false;
			}		
		}
		if (ic[idx].op==gen::PAR)
		{
			if (check.find(ic[idx].target)==check.end())
			{
				check[ic[idx].target]=true;
				result.insert(ic[idx].target);
			}
		}
		if (ic[idx].op==gen::WRITE)
		{
			if (check.find(ic[idx].first)==check.end())
			{
				check[ic[idx].first]=true;
				result.insert(ic[idx].first);
			}
		}
		if (ic[idx].op==gen::READ)
		{
			if (check.find(ic[idx].first)==check.end())
			{
				check[ic[idx].first]=false;
			}
		}
		if (isbr(ic[idx]))
		{
			if (check.find(ic[idx].first)==check.end())
			{
				check[ic[idx].first]=true;
				result.insert(ic[idx].first);
			}
			if (check.find(ic[idx].second)==check.end())
			{
				check[ic[idx].second]=true;
				result.insert(ic[idx].second);
			}
		}
	}
	for (set<string>::iterator idx=result.begin();idx!=result.end();)
	{
		if (isasr(*idx)) idx++;
		else
		{
			idx=result.erase(idx);
		}
	}
	return result;
}

vector<pblock::PBLOCK> pblock::split(vector<gen::quartral_element> ic)
{
	int last=0;
	PBLOCK tmp=PBLOCK();
	vector<pblock::PBLOCK> result;
	set<string> tmpdef;
	set<string> tmpuse;
	for (size_t i=0;i<ic.size();i++)
	{
		if (isgap(ic[i])||ic[i+1].op==gen::LABEL)
		{
			tmp.begin=last;tmp.end=i+1;
			last=i+1;
			result.push_back(tmp);
			tmpdef.clear();
			tmpuse.clear();
		}	
	}
	for (size_t i=0;i<result.size();i++)
	{
		if (ic[result[i].begin].op==gen::FUN||(result[i].begin+1<ic.size()&&ic[result[i].begin+1].op==gen::FUN))
		{
			string tmps = ic[result[i].end-1].target;
			for (size_t j=0;j<result.size();j++)
			{
				for (size_t k=result[j].begin;k<result[j].end;k++)
				{
					if (ic[k].target==tmps&&ic[k].op==gen::LABEL)
					{
						result[j].valid=true;
					}
				}
			}					
		}
	}
	for (size_t i=0;i<result.size();i++)
	{
		if (result[i].valid==true)
		{
			cout<<i<<' ';
		}	
	}
	cout<<endl;
	return result;
}
vector<pblock::PBLOCK> pblock::resplit(vector<gen::quartral_element> ic)
{
	int last=0;
	PBLOCK tmp=PBLOCK();
	vector<pblock::PBLOCK> result;
	set<string> tmpdef;
	set<string> tmpuse;
	for (size_t i=0;i<ic.size();i++)
	{
		if ((isgap(ic[i])&&ic[i].op!=gen::CALL&&ic[i].op!=gen::CALLFUN)||ic[i+1].op==gen::LABEL)
		{
			tmp.begin=last;tmp.end=i+1;tmp.valid=false;
			last=i+1;
			result.push_back(tmp);
			tmpdef.clear();
			tmpuse.clear();
		}	
	}
	for (size_t i=0;i<result.size();i++)
	{
		if (ic[result[i].begin].op==gen::FUN||(result[i].begin+1<ic.size()&&ic[result[i].begin+1].op==gen::FUN))
		{
			string tmps = ic[result[i].end-1].target;
			string fn = ic[result[i].begin+1].target;
			result[i].functionname=fn;
			for (size_t j=0;j<result.size();j++)
			{
				for (size_t k=result[j].begin;k<result[j].end;k++)
				{
					if (ic[k].target==tmps&&ic[k].op==gen::LABEL)
					{
						result[j].valid=true;
						for (size_t l=result[i].begin;l!=result[i].end;l++)
						{
							if (ic[l].op==gen::CREATE)
							{
								result[j].var.push_back(ic[l].first);
							}
						}
						if (i!=0)
						{
							result[j].level=helper::type2type::string2int(ic[result[i].begin+1].first);
						}
						else
						{
							result[j].level=0;
						}
						result[j].functionname=fn;
					}
				}
			}					
		}
	}
	for (size_t i=0;i<result.size();i++)
	{
		if (result[i].valid==true)
		{
			cout<<i<<' ';
		}	
	}
	cout<<endl;
	return result;
}

bool pblock::isalloc(string s)
{
	if (s.find("l@")!=string::npos)
	{
		return false;
	}
	if (s.find("i@")!=string::npos)
	{
		return false;
	}
	return true;
}
bool pblock::isasr(string s)
{
	if (!isalloc(s)) return false;
	if (s.find("t@")!=string::npos) return false;
	return true;
}