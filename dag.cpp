#include "dag.h"
hash_map<string,int> dag::string2node;
vector<dag::node> dag::vqn;
void dag::dag_symplify(vector<quartral_element> &vqe,vector<pblock::PBLOCK> vqb)
{
	for (size_t i=0;i<vqb.size();i++)
	{
		string2node.clear();
		vqn.clear();
		for (size_t j=vqb[i].begin;j<vqb[i].end;j++)
		{
			int l,r,mid;
			bool isnod=false;
			if (vqe[j].op==ADDRSHIFT)
			{
				string tmpstr=vqe[j].second;
				vqe[j].second=vqe[j].target;
				vqe[j].target=tmpstr;
			}
			if (string2node.find(vqe[j].first)!=string2node.end())
			{l=string2node[vqe[j].first];}			
			else 
			{
				node newnode;
				newnode.op=NOD;
				newnode.entity=vqe[j].first;
				vqn.push_back(newnode);
				string2node[vqe[j].first]=int(vqn.size()-1);
				l = int(vqn.size()-1);
			}
			if (string2node.find(vqe[j].second)!=string2node.end())
			{r=string2node[vqe[j].second];}
			else 
			{
				node newnode;
				newnode.entity=vqe[j].second;
				vqn.push_back(newnode);
				string2node[vqe[j].second]=int(vqn.size()-1);
				r=int(vqn.size()-1);
			}
			if (pblock::iscal(vqe[j]))	
			{
				mid=-1;
				for (int k=0;k<vqn.size();k++)
				{
					if (vqn[k].left==l&&vqn[k].right==r&&vqn[k].op==vqe[j].op)
					{
						mid=k;
						dag::string2node[vqe[j].target]=k;
						break;
					}
				}
				isnod=true;
				if (mid==-1)
				{
					isnod=false;
					node newnode;
					newnode.left=l;
					newnode.right=r;
					newnode.op=vqe[j].op;
					newnode.entity=vqe[j].target;
					vqn.push_back(newnode);
					string2node[vqe[j].target]=int(vqn.size()-1);
					mid=int(vqn.size()-1);
				}
				vqe[j].target=vqn[string2node[vqe[j].target]].entity;
				if (isnod&&vqe[j].op!=gen::PAR)
				{
					vqe[j].op=NOD;
				}
			}
			if (vqe[j].op==gen::PAR)
			{
				if (string2node.find(vqe[j].target)!=string2node.end())
				{
					vqe[j].target=vqn[string2node[vqe[j].target]].entity;
				}
				
			}
			if (pblock::isbr(vqe[j])||pblock::iscal(vqe[j]))
			{
				if (string2node.find(vqe[j].first)!=string2node.end())
				{
					vqe[j].first=vqn[string2node[vqe[j].first]].entity;
				}
				if (string2node.find(vqe[j].second)!=string2node.end())
				{
					vqe[j].second=vqn[string2node[vqe[j].second]].entity;
				}
			}
			if (vqe[j].op==ADDRSHIFT)
			{
				string tmpstr=vqe[j].second;
				vqe[j].second=vqe[j].target;
				vqe[j].target=tmpstr;
			}
		}
	}
	for (vector<quartral_element>::iterator idx= vqe.begin();idx!=vqe.end();)
	{
		if (idx->op==NOD)
		{
			idx=vqe.erase(idx);
		}
		else
		{
			idx++;
		}
	}
}