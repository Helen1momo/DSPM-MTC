#include "cdspm.h"
#include <iostream>

void cdspm::filter_cuts()
{
	pattern_cuts.clear();
	Pattern p;
	find_cuts(pdsp_root, p);
	cout << pattern_cuts.size() << "  "<<"***"<<endl;
	check_full(pattern_cuts, mfs.cut_output_path, true, false);
}

void cdspm::find_cuts(Node *root, Pattern & p)
{
	if (root == NULL || root->tail)
	{
		return;
	}
	for (hash_map<int, Node*>::iterator it = root->children.begin(); it != root->children.end(); it++)
	{
		int key = it->first;
		Node *val = it->second;
		p.eles.push_back(key);
		if (val->tail)
		{
			pattern_cuts.push_back(p);
		}
		find_cuts(val, p);
		p.eles.pop_back();
	}
}