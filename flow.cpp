#include "flow.h"
map<string,int> flow::color;
void flow::active_variable(vector<pblock::PBLOCK>& vqb,vector<gen::quartral_element>& vqe)
{
	int pro_start,pro_end;
	for (size_t j=0;j<vqe.size();j++)
	{
		if (vqe[j].op==gen::ADDRSHIFT)
		{
			string tmpstr=vqe[j].second;
			vqe[j].second=vqe[j].target;
			vqe[j].target=tmpstr;
		}
	}
	for (size_t i=0;i<vqb.size();i++)
	{
		vqb[i].def=pblock::isdef(vqe,vqb[i].begin,vqb[i].end);
		vqb[i].use=pblock::isuse(vqe,vqb[i].begin,vqb[i].end);		
	}
	for (size_t i=0;i<vqb.size();i++)
	{
		if (vqb[i].valid)
		{			
			pro_start=i;
		}
		if (vqe[vqb[i].end-1].op==gen::RETURN)
		{
			pro_end=i;
			for (int j=pro_start;j<=pro_end;j++)
			{
				for (int k=pro_start;k<=pro_end;k++)
				{
					if (k==j+1&&vqe[vqb[j].end-1].op!=gen::BR)
					{
						vqb[j].next.push_back(k);
						continue;
					}
					else
					{
						if (vqe[vqb[j].end-1].target==vqe[vqb[k].begin].target&&(pblock::isbr(vqe[vqb[j].end-1])||vqe[vqb[j].end-1].op==gen::BR))
						{
							vqb[j].next.push_back(k);
						}
					}
				}
			}
			bool change;
			set<string> addset;
			do
			{
				change=false;
				for (int j=pro_end;j>=pro_start;j--)
				{
					for (vector<int>::iterator idx_next=vqb[j].next.begin();idx_next!=vqb[j].next.end();idx_next++)
					{
						for (set<string>::iterator idx_in=vqb[*idx_next].in.begin();idx_in!=vqb[*idx_next].in.end();idx_in++)
						{
							if (vqb[j].out.find(*idx_in)==vqb[j].out.end())
							{
								vqb[j].out.insert(*idx_in);
								change=true;
							}
						}						
					}
					addset.clear();
					if (!vqb[j].out.empty())
					{
						for (set<string>::iterator idx_out=vqb[j].out.begin();idx_out!=vqb[j].out.end();idx_out++)
						{
							if (vqb[j].def.find(*idx_out)==vqb[j].def.end())
							{
								addset.insert(*idx_out);
							}
						}				
					}
					if (!addset.empty())
					{
						for (set<string>::iterator idx_addset=addset.begin();idx_addset!=addset.end();idx_addset++)
						{
							if (vqb[j].in.find(*idx_addset)==vqb[j].in.end())
							{
								vqb[j].in.insert(*idx_addset);	
								change=true;
							}
						}
					}
					if (!vqb[j].use.empty())
					{
						for (set<string>::iterator idx_use=vqb[j].use.begin();idx_use!=vqb[j].use.end();idx_use++)
						{
							if (vqb[j].in.find(*idx_use)==vqb[j].in.end())
							{
								vqb[j].in.insert(*idx_use);	
								change=true;
							}
						}
					}
				}
			}
			while (change);
		}
	}	
	for (size_t j=0;j<vqe.size();j++)
	{
		if (vqe[j].op==gen::ADDRSHIFT)
		{
			string tmpstr=vqe[j].second;
			vqe[j].second=vqe[j].target;
			vqe[j].target=tmpstr;
		}
	}
	bool isall;
	int now_level=0;
	vector<string> elements;
	for (size_t i=0;i<vqb.size();i++)
	{
		if (vqb[i].valid)
		{
			isall=false;
			std::cout<<"start: var: ";
			pro_start=i;
			if (i!=0)
			{
				now_level=vqb[i].level;
			}
			for (size_t j=0;j<vqb[i].var.size();j++)
			{
				std::cout<<vqb[i].var[j]<<' ';
				color[vqb[i].var[j]]=j;
			}
			elements=vqb[i].var;
			std::cout<<endl;
		}
		std::cout<<i<<"---------------------"<<endl;
		for (size_t j=vqb[i].begin;j<vqb[i].end;j++)
		{
			if (vqe[j].op==gen::CALL||vqe[j].op==gen::CALLFUN)
			{
				for (size_t k=0;k<vqe.size();k++)
				{
					if (vqe[k].op==gen::LABEL&&vqe[k].target=="l@"+vqe[j].second)
					{
						if (helper::type2type::string2int(vqe[k+1].first)==now_level+1)
						{
							isall=true;

						}					
					}
				}
			}
			std::cout<<gen::I2string[vqe[j].op]<<' '<<vqe[j].first<<' '<<vqe[j].second<<' '<<vqe[j].target<<endl;
		}
		std::cout<<">next:";
		for (size_t j=0;j<vqb[i].next.size();j++)
		{
			std::cout<<vqb[i].next[j]<<' ';
		}
		std::cout<<endl;
		std::cout<<">use:";
		for (set<string>::iterator j=vqb[i].use.begin();j!=vqb[i].use.end();j++)
		{
			std::cout<<*j<<' ';
		}
		std::cout<<endl;
		std::cout<<">def:";
		for (set<string>::iterator j=vqb[i].def.begin();j!=vqb[i].def.end();j++)
		{
			std::cout<<*j<<' ';
		}
		std::cout<<endl;
		std::cout<<">in:";
		for (set<string>::iterator j=vqb[i].in.begin();j!=vqb[i].in.end();j++)
		{
			std::cout<<*j<<' ';
		}
		std::cout<<endl;
		std::cout<<">out:";
		for (set<string>::iterator j=vqb[i].out.begin();j!=vqb[i].out.end();j++)
		{
			std::cout<<*j<<' ';
		}
		std::cout<<endl;
	}
}
