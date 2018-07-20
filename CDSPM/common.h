#pragma once
#include "parameters.h"
#include <set>
#include <vector>
#include <hash_map>
#include <sstream>
using namespace std;

struct Parameters
{
	double sup;
	double sig_g;
	double sig_l;
	double p_start;	
	double p_end;
	double p_step;

	int max_pattern_length;
	int gap;
	
	Parameters(double a, double b, double c, double f, double g, double h, int d = 1 << 29, int e = 1 << 29)
	{
		sup = a;
		sig_g = b;
		sig_l = c;
		
		p_start = f;
		p_end = g;
		p_step = h;

		max_pattern_length = d;
		gap = e;
	}
};

class Sequence
{
public:
	vector<int>eles;
	vector< hash_map<int, int> > next_key;
	void build_next_key();
	int gap;
	int lable;
};

class Pattern :public Sequence
{
public:
	int pos;
	int neg;
	double cur_sig_l;
	Pattern()
	{
		pos = 0;
		neg = 0;
		cur_sig_l = -1;
	}

	bool operator < (const Pattern &p) const
	{
		if ((p.eles.size() == 0 && eles.size() == 0) || (p.eles.size() != 0 && eles.size() == 0))
			return 0;
		if (p.eles.size() == 0 && eles.size() != 0)
			return 1;

		int i = 0;
		int j = 0;
		while ( i < eles.size() && j < p.eles.size() && eles[i] == p.eles[j]){
			++i;
			++j;
		}
		
		if ((i <= eles.size() && j == p.eles.size()) || (i == eles.size() && j == p.eles.size()))
			return 0;
		if ((i == eles.size() && j < p.eles.size()) || eles[i] < p.eles[j])
			return 1;
		else
			return 0;
	}

	bool operator==(const Pattern& p) const  //重载“==”操作符
	{
		if (p.eles.size() == 0 && eles.size() == 0)
			return 1;
		if (p.eles.size() != eles.size())
			return 0;
		int i = 0;
		int j = 0;
		while (i < eles.size() && j < p.eles.size() && eles[i] == p.eles[j]){
			++i;
			++j;
		}
		if (i == j)
			return 1;
		else
			return 0;//具体匹配条件自己设定，可以设定多个
	}

};

class Pattern_set : public Pattern
{
public:
	set<Pattern> FDR;
	set<Pattern> FWER;

	void clean()
	{
		FDR.clear();
		FWER.clear();
	}
};


struct Node
{
	int key;
	hash_map<int, Node*> children;
	int pos;
	int neg;
	bool tail;

	Node(int value = -1)
	{
		key = value;
		pos = 0;
		neg = 0;
		tail = false;
	}

};

class MakeFolds
{
public:
	// source
	string cdsp_output_path;
	string cut_output_path;
	string imp_output_path;
	string red_output_path;
	string fwer_output_path;
	//nu_cv
	string cdsp_pra_path;
	string cut_pra_path;
	string imp_pra_path;
	string red_pra_path;
	string fwer_pra_path;

	string train_file;
	string test_file;
	
	string train_red;
	string train_cdsp;
	string train_cut;
	string train_imp;
	string train_fwer;
	string test_red;
	string test_cdsp;
	string test_cut;
	string test_imp;
	string test_fwer;

	string profold;
	string input;
	string output;
	string sup_g;
	string times;
	string source;
	string source_cv_nu;

	string para;
	string para_cv_nu;
	
	
	
	
	string cv_nu;

	void mkdir(string cmd);

	void mk_profold();
	void mk_input();
	void split(string file_p, string file_e);

	void mk_sup_g(double sup, double sig_g);
	void mk_times(int time);
	void mk_source();
	void mk_source_cv_nu(int nu);

	void mk_para_cv_nu(double sup, double sig_g, double sig_l, int cv_nu);
	void mk_parafold(double sup, double sig_l, double sig_g);
	
	
	void mk_output();

	
	
	void mk_cv_nu(int nu);
};