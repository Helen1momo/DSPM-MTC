#include "cdspm.h"
#include <iostream>
using namespace std;

double min(double a, double b)
{
	return a < b ? a : b;
}

void cdspm::filter_cdsps()
{
	for (int i = 0; i < pattern_candidates.size(); i++)
	{
		pattern_candidates[i].build_next_key();
		
		// improvements
		/*bool cur_impro = get_impro(cdsp_root, pattern_candidates[i], 0);
		if (cur_impro)
		{
			pattern_imps.push_back(pattern_candidates[i]);
		}
		*/

		bool cur_sig_l = get_sig_l(pdsp_root, pattern_candidates[i], 0);
		
		// cdsps
		if (cur_sig_l == false)
		{
			pattern_p_map.erase(pattern_candidates[i]);
			
			vector<Pattern>::iterator iter = find(patterns.begin(), patterns.end(),pattern_candidates[i]);
			if (iter != patterns.end())
				patterns.erase(iter);

			//pattern_cdsps.push_back(pattern_candidates[i]);
			//insert_tree(cdsp_root, pattern_candidates[i], 0);
		}
		else
		{
			insert_tree(cdsp_root, pattern_candidates[i], 0);
		}

		// cuts
		/*
		if (cur_sig_l > (1 << 28))
		{
			pattern_cuts.push_back(pattern_candidates[i]);
		}
		*/

		pattern_candidates[i].next_key.clear();
	}
	
	//¶¼ÊÇÎ±pattern
	for (int i = 0; i < pattern_candidates.size(); i++)
	{
		insert_tree(pdsp_root, pattern_candidates[i], 0);
	}
}

bool cdspm::get_sig_l(Node * root, Pattern &p, int k)
{
	if (root == NULL || root->children.size() == 0 || k >= p.eles.size())
	{
		return 1 << 29;
	}
	double ret = 1 << 29;
	bool tag = true;

	for (hash_map<int, Node*>::iterator it = root->children.begin(); it != root->children.end(); it++)
	{
		int key = it->first;
		Node* child = it->second;

		if (p.next_key[k].count(key) != 0)
		{
			if (child->tail)
			{
				/*
				double f_frequency = 1.0*p.pos / child->pos;
				double b_frequency = 0;
				if (child->neg == 0)
				{
					b_frequency = 1;
					ret = f_frequency / b_frequency;
				}
				else if (p.neg != 0)
				{
					b_frequency = 1.0*p.neg / child->neg;	
				}
				*/
				
				double fisher = getfisher(child->pos, p_sequences.size() - child->pos, child->neg, e_sequences.size() - child->neg);

				if (fisher <= pattern_p_map[p])
					tag = false;

				//ret = min(ret, 1.0*f_frequency/b_frequency);
				/*
				if (ret < sig_l - 1e-6)
				{
					return ret;
				}
				*/
			}
			int skip = p.next_key[k][key];
			/*
			ret = min(ret, get_sig_l(child, p, skip + 1));
			if (ret < sig_l - 1e-6)
			{
				return ret;
			}*/
		}
	}
	return tag;
}


bool cdspm::get_impro(Node * root, Pattern &p, int k)
{
	if (root == NULL || root->children.size() == 0 || k >= p.eles.size())
	{
		return true;
	}

	for (hash_map<int, Node*>::iterator it = root->children.begin(); it != root->children.end(); it++)
	{
		int key = it->first;
		Node* child = it->second;
		if (p.next_key[k].count(key) != 0)
		{
			if (child->tail)
			{
				double f_frequency = 1.0*p.pos / child->pos;
				double b_frequency = 1e-8;
				if (child->neg == 0)
				{
					b_frequency = 1;
				}
				else if (p.neg != 0)
				{
					b_frequency = 1.0*p.neg / child->neg;
				}

				double value1 = f_frequency / b_frequency;
				if (value1 < 1 - 1e-6)
				{
					return false;
				}
			}

			int skip = p.next_key[k][key];

			if (!get_impro(child, p, skip + 1))
			{
				return false;
			}
		}
	}
	return true;
}