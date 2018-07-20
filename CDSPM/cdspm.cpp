#include "cdspm.h"
#include <iostream>
#include <hash_map>
#include <fstream>
#include <hash_set>
using namespace std;


cdspm::~cdspm()
{
	delete_tree(pdsp_root);
	delete_tree(candidates_root);
	delete_tree(cdsp_root);
}

void cdspm::init_folds()
{	
	init_files();

	ofstream fout(mfs.cdsp_output_path);
	fout.close();
	fout.open(mfs.cut_output_path);
	fout.close();
	fout.open(mfs.imp_output_path);
	fout.close();
	fout.open(mfs.red_output_path);
	fout.close();
}

void cdspm::init_files()
{
	mfs.cdsp_output_path = mfs.source_cv_nu + "\\cdsp.txt";
	mfs.cut_output_path = mfs.source_cv_nu + "\\cut.txt";
	mfs.imp_output_path = mfs.source_cv_nu + "\\imp.txt";
	mfs.red_output_path = mfs.source_cv_nu + "\\red.txt";
	mfs.fwer_output_path = mfs.source_cv_nu + "\\fwer.txt";
}

void cdspm::delete_tree(Node *root)
{
	if (root == NULL) return;
	for (hash_map<int, Node*>::iterator it = root->children.begin(); it != root->children.end(); it++)
	{
		delete_tree(it->second);
	}
	delete root;
	root = NULL;
}

void cdspm::insert_tree(Node* rt, Pattern & pat_can, int k)
{
	if (k >= pat_can.eles.size())
	{
		return;
	}
	int value = pat_can.eles[k];
	if (rt->children.count(value) == 0)
	{
		Node *temp = new Node(value);
		rt->children[value] = temp;
	}

	Node *cur = rt->children[value];
	if (k == pat_can.eles.size() - 1)
	{
		cur->pos = pat_can.pos;
		cur->neg = pat_can.neg;
		cur->tail = true;
	}
	insert_tree(cur, pat_can, k + 1);
}

void cdspm::check_full(vector<Pattern> & patterns,string path, bool end, bool is_cdsp)
{
	//if (patterns.size() >= 10000 || end)
	//{
		ofstream fout(path, ios::app);
		for (int i = 0; i < patterns.size(); i++)
		{
			if (p_vs_n)
			{
				fout << "0";
			}
			else
			{
				fout << "1";
			}

			//fout << " " << i+1;//µ±Ç°patternÅÅÐò
			
			for (int j = 0; j<patterns[i].eles.size(); j++)
			{
				fout << " " << patterns[i].eles[j];
			}
			fout << endl;
		}
		fout.close();
		patterns.clear();
	//}
}