#include "cdspm.h"
#include <iostream>
#include <algorithm>
#include <list>
using namespace std;

void cdspm::sort_pattern()
{
	typedef pair<Pattern, double> PAIR;

	struct CmpByValue {
		bool operator()(const PAIR& lhs, const PAIR& rhs) {
			return lhs.second < rhs.second;
		}
	};

	vector<PAIR> pattern_p_vec(pattern_p_map.begin(), pattern_p_map.end());
	sort(pattern_p_vec.begin(), pattern_p_vec.end(), CmpByValue());

	/*for (int i = 0; i != pattern_p_vec.size(); ++i) {
		cout << pattern_p_vec[i].second << " ";
		
		for (int j = 0; j < pattern_p_vec[i].first.eles.size(); j++){
			cout << pattern_p_vec[i].first.eles[j] << " ";
		}
		cout << " ||||  " << pattern_p_vec[i].first.pos << "-----" << pattern_p_vec[i].first.neg << endl;
		
	}*/

	int max = 0;
	int fwer = 0;
	cout << "pattern : " << pattern_p_vec.size() << endl;


	//确认各个p值的对应标号
	int FWER = 0;
	int FDR = 0;
	for (double p = p_start; p <= p_end; p += p_step)
	{
		for (int i = 0; i < pattern_p_vec.size(); i++) {
			if (pattern_p_vec[i].second <= p / pattern_p_vec.size())//FWER
			{
				FWER++;
			}
			if (pattern_p_vec[i].second <= ((i + 1)*p) / pattern_p_vec.size() && i + 1 > FDR)
				FDR = i + 1;  //fdr最大下标
			if (pattern_p_vec[i].second > p)
				break;
		}
		if (p_vs_n)
		{
			// "0";
			list<int> list;
			list.push_back(FWER);
			list.push_back(FDR);

			//p_num_map[p] = list;
			p_num_map.insert(make_pair(p*100, list));
		}
		else
		{
			// "1";
			p_num_map[p*100].push_back(FWER);
			p_num_map[p*100].push_back(FDR);
		}
		FWER = 0;
		FDR = 0;
	}

	//找p为最大的时候对应的集合
	for (int i = 0; i < pattern_p_vec.size(); i++) {
		if (pattern_p_vec[i].second <= p_end / pattern_p_vec.size())//FWER
		{
			//if (pattern_set.FWER.empty() == true){ //集合为空直接加入
				pattern_fwer.push_back(pattern_p_vec[i].first);
				pattern_set.FWER.insert(pattern_p_vec[i].first);
				fwer++;
			//}
			/*else{//集合不为空先判断有没有存在
				set<Pattern>::iterator ite1 = pattern_set.FWER.begin();
				ite1 = pattern_set.FWER.find(pattern_p_vec[i].first);
				if (ite1 == pattern_set.FWER.end()){ //不存在就加入
					pattern_fwer.push_back(pattern_p_vec[i].first);
					pattern_set.FWER.insert(pattern_p_vec[i].first);
					fwer++;
				}
			}*/
			if (pattern_fwer.size() != 0 && pattern_fwer.size() % 10000 == 0)//每到一万个就写一遍文件
			{
				check_full(pattern_fwer, mfs.fwer_output_path, false, true);
			}
		}
		if (pattern_p_vec[i].second <= ((i+1)*p_end) / pattern_p_vec.size() && i+1 > max)
			max = i+1;  //fdr最大下标
		if (pattern_p_vec[i].second > p_end)
			break;
	}

	cout << "FWER : " << fwer << endl;
	cout << "FDR : " << max << endl;

	for (int i = 0; i < max; i++) {

		//if (pattern_set.FDR.empty() == true){ //集合为空直接加入
			pattern_fdr.push_back(pattern_p_vec[i].first);
			pattern_set.FDR.insert(pattern_p_vec[i].first);
		//}
		/*else{//集合不为空先判断有没有存在
			set<Pattern>::iterator ite1 = pattern_set.FDR.begin();
			ite1 = pattern_set.FDR.find(pattern_p_vec[i].first);
			if (ite1 == pattern_set.FDR.end()){ //不存在就加入
				pattern_fdr.push_back(pattern_p_vec[i].first);
				pattern_set.FDR.insert(pattern_p_vec[i].first);
			}
		}*/

		//pattern_fdr.push_back(pattern_p_vec[i].first);
		if (pattern_fdr.size() != 0 && pattern_fdr.size() % 10000 == 0)
		{
			check_full(pattern_fdr, mfs.cdsp_output_path, false, true);
		}
	}

	


	/*
	pattern_p_map.clear();
	pattern_p_vec.clear();
	*/
}