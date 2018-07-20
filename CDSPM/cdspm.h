#pragma once

#include "parameters.h"
#include "common.h"
#include<map>

class cdspm 
{
public:
	cdspm(vector<Sequence> p, vector<Sequence> e, Parameters par, bool flag)
	{
		sup = par.sup;
		sig_g = par.sig_g;
		sig_l = par.sig_l;
		max_pattern_length = par.max_pattern_length;
		gap = par.gap;
		p_start = par.p_start;
		p_end = par.p_end;
		p_step = par.p_step;

		p_vs_n = flag;
		p_sequences = p;
		e_sequences = e;
		pos = p.size();
		neg = e.size();

		candidates_root = NULL;
		pdsp_root = new Node();
		cdsp_root = new Node();
	}

	~cdspm();
	void init_folds();
	void init_files();

	void execute();
	bool generate_patterns(int k);
	bool find_frequent_item();
	bool create_candidates(int k);
	void check_full(vector<Pattern> & patterns, string path, bool end, bool is_cdsp);

	bool check_pattern(Pattern & pattern_a, Pattern & pattern_b);
	Pattern generate_two(Pattern & pattern_a, Pattern & pattern_b);
	void naive_create();
	void improved_create();
	bool find_pattern(Node* root, Pattern & p, int k);

	void count_candidates_with_hash_tree();
	
	void delete_tree(Node *rt);
	void insert_tree(Node* root, Pattern & pat_can, int k);

	void count_candidates(Node*root, Sequence& seq, int k, bool is_pos);
	void filter_candidates_with_hash_tree(Node *root, Pattern &p, int cnt_pos, int cnt_neg);

	void filter_cdsps();
	bool get_sig_l(Node * root, Pattern &p, int k);
	bool get_impro(Node * root, Pattern &p, int k);

	/*
		compare with other algorithm.
	*/
	void filter_cuts();
	void find_cuts(Node *root, Pattern & p);

	/*
		write data for classify.
	*/
	void data_for_classify(double sup, double sig_g, int cv_nu, double sig_l_start, double sig_l_end, double sig_l_step,map<int, list<int>> map);
	void trans_data_for_classify(vector<Sequence>&  sequences, string source_file, string target_file, bool is_cdsp, bool is_fwer, bool is_train ,int posnum,int negnum);
	void read_patterns(string source_file, vector<Pattern> &patterns, bool is_cdsp, bool is_fwer, bool is_train, int posnum, int negnum);

	int fac(int n);
	double getfisher(int a , int b , int c , int d);
	void sort_pattern();
	double getlog(int n);
public:
	vector<Sequence> p_sequences, e_sequences;
	vector<int> freq_items;
	vector<Pattern> patterns;            // last frequent patterns
	vector<Pattern> pattern_candidates;  // last global discriminitive patterns
	vector<Pattern> pattern_cdsps;       // last conditional discriminitive patterns.
	vector<Pattern> pattern_fdr;
	vector<Pattern> pattern_fwer;
	map<int, double> p_map;

	map<Pattern, double> pattern_p_map;  
	/*
		TBD
		vector<Pattern> pattern_improves;
		vector<Pattern> pattern_tbd;
	*/
	vector<Pattern> pattern_cuts;
	vector<Pattern> pattern_imps;
 
	Node * candidates_root;
	Node * pdsp_root; // pseudo discriminative sequence pattern
	Node * cdsp_root;

	bool p_vs_n;
	double sup;
	double sig_l;
	double sig_g;
	int max_pattern_length;
	int gap;
	int pos;
	int neg;
	double p_start;
	double p_end;
	double p_step;


	string log_path;
	MakeFolds mfs;

	Pattern_set pattern_set;
	map<int, list<int>> p_num_map;
};