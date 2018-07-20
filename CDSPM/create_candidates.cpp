#include "cdspm.h"
#include <iostream>
#include <hash_map>
#include <hash_set>
using namespace std;


bool cdspm::find_frequent_item()
{
	if (patterns.size() != 0)
	{
		patterns.clear();
	}
	
	if (!pattern_candidates.empty())
	{
		pattern_candidates.clear();
	}

	if (freq_items.size() != 0)
	{
		freq_items.clear();
	}

	hash_map<int, int>p_cnt, e_cnt;

	for (int i = 0; i < p_sequences.size(); i++)
	{
		hash_set<int> s;
		for (int j = 0; j < p_sequences[i].eles.size(); j++)
		{
			s.insert(p_sequences[i].eles[j]);// 把一条序列全部放进set中
		}
		for (hash_set<int>::iterator it = s.begin(); it != s.end(); it++)
		{
			if (p_cnt.count(*it) == 0)
			{
				p_cnt[*it] = 0;
			}
			p_cnt[*it] += 1;
		}
	}

	for (int i = 0; i < e_sequences.size(); i++)
	{
		hash_set<int> s;
		for (int j = 0; j < e_sequences[i].eles.size(); j++)
		{
			s.insert(e_sequences[i].eles[j]);
		}
		for (hash_set<int>::iterator it = s.begin(); it != s.end(); it++)
		{
			if (e_cnt.count(*it) == 0)
			{
				e_cnt[*it] = 0;
			}
			e_cnt[*it] += 1;
		}
	}

	for (hash_map<int, int>::iterator it = p_cnt.begin(); it != p_cnt.end(); it++)
	{
		if (1.0 * it->second / p_sequences.size() < sup)
		{
			continue;
		}

		Pattern temp;
		temp.eles.push_back(it->first);
		temp.pos = it->second;
		if (e_cnt.count(it->first) == 0)
		{
			e_cnt[it->first] = 0;
		}
		temp.neg = e_cnt[it->first];

		patterns.push_back(temp);


		double fisher = getfisher(temp.pos, p_sequences.size() - temp.pos, temp.neg, e_sequences.size() - temp.neg);
		/*if (1.0*temp.pos / pos / (1.0 * temp.neg / neg ) > sig_g - 1e-6 )
		{
			pattern_candidates.push_back(temp);
		}*/

		//fdr
		pattern_p_map.insert(make_pair(temp, fisher));
		pattern_candidates.push_back(temp);


		freq_items.push_back(it->first);
	}

	if (patterns.size() == 0)
	{
		return false;
	}
	return true;
}

bool cdspm::create_candidates(int k)
{
	if (!pattern_candidates.empty())
	{
		pattern_candidates.clear();
	}
	
	if (k == 2)
	{
		for (int i = 0; i < patterns.size(); i++)
		{
			for (int j = 0; j < patterns.size(); j++)
			{
				Pattern temp;
				temp.eles.push_back(patterns[i].eles[0]);
				temp.eles.push_back(patterns[j].eles[0]);
				pattern_candidates.push_back(temp);
			}
		}
	}
	else
	{
		if (freq_items.size() > patterns.size())
		{
			naive_create();
		}
		else
		{
			improved_create();
		}
	}
	
	if (!pattern_candidates.empty())
	{
		count_candidates_with_hash_tree();//用hash树计数,得到正反集频次，计算p值
	}
	else
	{
		return false;
	}

	if (patterns.size() == 0)
	{
		return false;
	}
	return true;
}


bool cdspm::check_pattern(Pattern & pattern_a, Pattern & pattern_b)
{
	if (pattern_a.eles.size() != pattern_b.eles.size())
	{
		return false;
	}

	vector<int>::iterator it_a = pattern_a.eles.begin();
	vector<int>::iterator it_b = pattern_b.eles.begin();
	it_a++;

	while (it_a != pattern_a.eles.end() && it_b != pattern_b.eles.end())
	{
		if (*it_a != *it_b)
		{
			return false;
		}
		it_a++;
		it_b++;
	}
	return true;
}

Pattern cdspm::generate_two(Pattern & pattern_a, Pattern & pattern_b)
{
	Pattern ret;
	for (int i = 0; i < pattern_a.eles.size(); i++)
	{
		ret.eles.push_back(pattern_a.eles[i]);
	}
	int length = pattern_b.eles.size();
	ret.eles.push_back(pattern_b.eles[length - 1]);
	ret.neg = ret.pos = 0;
	return ret;
}

void cdspm::naive_create()
{
	for (int i = 0; i < patterns.size(); i++)
	{
		for (int j = 0; j < patterns.size(); j++)
		{
			if (!check_pattern(patterns[i], patterns[j]))
			{
				continue;
			}

			Pattern temp = generate_two(patterns[i], patterns[j]);
			pattern_candidates.push_back(temp);
		}
	}
}

void cdspm::improved_create()
{
	Node *root = new Node();
	for (int i = 0; i < patterns.size(); i++)
	{
		Pattern temp;
		for (int j = patterns[i].eles.size() - 1; j>0; --j)
		{
			temp.eles.push_back(patterns[i].eles[j]);
		}
		insert_tree(root, temp, 0);
	}

	for (int i = 0; i < patterns.size(); i++)
	{
		Pattern temp = patterns[i];
		temp.pos = temp.neg = 0;
		for (int j = 0; j < freq_items.size(); j++)
		{
			temp.eles.push_back(freq_items[j]);
			if (find_pattern(root, temp, temp.eles.size() - 1));
			{
				pattern_candidates.push_back(temp);
			}
			temp.eles.pop_back();
		}
	}

	delete_tree(root);
}

bool cdspm::find_pattern(Node* root, Pattern & p, int k)
{
	if (k == 0)
	{
		return true;
	}
	int key = p.eles[k];
	if (root->children.count(key) > 0)
	{
		return find_pattern(root->children[key], p, k - 1);
	}
	return false;
}