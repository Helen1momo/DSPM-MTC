#include "cdspm.h"
#include <iostream>
#include <algorithm>
using namespace std;

bool cdspm::generate_patterns(int k)
{
	bool has_next = false;

	if (k == 1)
	{
		has_next = find_frequent_item();
	}
	else
	{
		has_next = create_candidates(k);//生成新的模式，计算得出p值，并且放进map中
	}

	
	if (has_next)
	{
		filter_cdsps();
		cout << patterns.size() << "  " << pattern_candidates.size() << "  " << pattern_p_map.size() << endl;
	}
	
	//cout << patterns.size() << "  " << pattern_candidates.size() << endl;

	/*
	typedef pair<Pattern, double> PAIR;

	struct CmpByValue {
		bool operator()(const PAIR& lhs, const PAIR& rhs) {
			return lhs.second < rhs.second;
		}
	};

	vector<PAIR> pattern_p_vec(pattern_p_map.begin(), pattern_p_map.end());
	sort(pattern_p_vec.begin(), pattern_p_vec.end(), CmpByValue());
	for (int i = 0; i != pattern_p_vec.size(); ++i) {
		cout << pattern_p_vec[i].second << " ";
		for (int j = 0; j < pattern_p_vec[i].first.eles.size(); j++){
			cout << pattern_p_vec[i].first.eles[j] << " ";
		}
		cout<< endl;
	}
	*/

	return has_next;
}