#include "cdspm.h"
#include <iostream>
using namespace std;

void cdspm::count_candidates_with_hash_tree()
{
	delete_tree(candidates_root);
	candidates_root = new Node();
	for (int i = 0; i < pattern_candidates.size(); i++)
	{
		insert_tree(candidates_root, pattern_candidates[i], 0);
	}

	for (int i = 0; i < p_sequences.size(); i++)
	{
		count_candidates(candidates_root, p_sequences[i], 0, true);
	}
	for (int i = 0; i < e_sequences.size(); i++)
	{
		count_candidates(candidates_root, e_sequences[i], 0, false);
	}
	
	patterns.clear();
	pattern_candidates.clear();
	int cnt_pos = p_sequences.size();
	int cnt_neg = e_sequences.size();
	Pattern p;
	filter_candidates_with_hash_tree(candidates_root, p, cnt_pos, cnt_neg);
}



void cdspm::count_candidates(Node*root, Sequence& seq, int k, bool is_pos)
{
	if (root == NULL || root->children.size() == 0 || k>=seq.eles.size() )
	{
		return;
	}
	for (hash_map<int, Node*>::iterator it = root->children.begin(); it != root->children.end(); it++)
	{
		int key = it->first;
		Node* child = it->second;
		if (seq.next_key[k].count(key) != 0)
		{
			if (child->tail)
			{
				if (is_pos)
				{
					child->pos++;
				}
				else
				{
					child->neg++;
				}
			}
			int skip = seq.next_key[k][key];
			count_candidates(child, seq, skip+1, is_pos);
		}
	}
}

int cdspm:: fac(int n)
{
	if (n<0) return 0;
	if (n == 0 || n == 1)return 1;
	if (n>1)
	{
		return n*fac(n - 1);
	}
}

void cdspm::filter_candidates_with_hash_tree(Node *root, Pattern &p, int cnt_pos, int cnt_neg)
{
	
	if (root == NULL) return;
	for (hash_map<int, Node*>::iterator it = root->children.begin(); it != root->children.end(); it++)
	{
		double fisher = 0.0;
		int key = it->first;
		Node* child = it->second;
		p.eles.push_back(key);
		if (child->tail)
		{
			p.pos = child->pos;
			p.neg = child->neg;

			if (1.0*child->pos / cnt_pos >= sup)//计算support,得到频繁模式
			{
				patterns.push_back(p);
				
				fisher = getfisher(child->pos, cnt_pos - child->pos, child->neg, cnt_neg - child->neg);

				pattern_p_map[p] = fisher;
				pattern_candidates.push_back(p);
			}
		}
		filter_candidates_with_hash_tree(child, p, cnt_pos, cnt_neg);
		p.eles.pop_back();
	}
}