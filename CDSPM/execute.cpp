#include "cdspm.h"
#include <iostream>
#include <ctime>
using namespace std;

void cdspm::execute()
{
	clock_t start, finish;
	start = clock();
	int k = 1;
	bool has_next = true;
	while (has_next && k <= max_pattern_length) 
	{
		cout << k << "  /  "<<max_pattern_length<< endl;
		has_next = generate_patterns(k);
		++k;
		//check_full(pattern_candidates, mfs.cdsp_output_path, false, true);
		/*
		check_full(pattern_cuts, mfs.cut_output_path, false, false);
		check_full(pattern_imps, mfs.imp_output_path, false, false);
		if (sig_l < 1.09)
		{
			check_full(pattern_candidates, mfs.red_output_path, true, false);
		}
		*/
	}
	sort_pattern();
	check_full(pattern_fdr, mfs.cdsp_output_path, false, true);
	check_full(pattern_fwer, mfs.fwer_output_path, false, true);
	/*
	pattern_fdr.clear();
	pattern_fwer.clear();
	*/
	
	//check_full(pattern_candidates, mfs.cdsp_output_path, true, true);
	/*
	check_full(pattern_cuts, mfs.cut_output_path, true, false);
	check_full(pattern_imps, mfs.imp_output_path, true, false);
	if (sig_l < 1.09)
	{
		check_full(pattern_candidates, mfs.red_output_path, true, false);
	}
	*/
	// filter_cuts();

	finish = clock();
}

