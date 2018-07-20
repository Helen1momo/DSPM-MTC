#include "common.h"
#include "parameters.h"
#include "read_data.h"
#include "cdspm.h"
#include <ctime>
#include <iostream>
#include <fstream>
using namespace std;


cdspm * signal_process(vector<Sequence>& p_sequeues, vector<Sequence> & e_sequeues, Parameters pra, MakeFolds mfs, Pattern_set pattern_set, map<int, list<int>> p_num_map)
{
	cdspm *cdsp = new cdspm(p_sequeues, e_sequeues, pra, true);
	cdsp->mfs = mfs;
	cdsp->pattern_set = pattern_set;
	cdsp->p_num_map = p_num_map;
	cdsp->init_folds();
	cdsp->init_files();
	cdsp->execute();
	
	pattern_set = cdsp->pattern_set;
	p_num_map = cdsp->p_num_map;
	delete cdsp;
	

	cdsp = new cdspm(e_sequeues, p_sequeues, pra, false);
	cdsp->mfs = mfs;
	cdsp->pattern_set = pattern_set;
	cdsp->p_num_map = p_num_map;
	cdsp->init_files();
	cdsp->execute();//生成pattern
	p_num_map = cdsp->p_num_map;
	return cdsp;
}
void process(double sup, double sig_g, int max_pattern_length = 1 << 29,
	int gap = 1 << 29)
{
	double p_start = 0.01;
	double p_end = 0.1;
	double p_step = 0.02;
	

	time_t tt = time(NULL);                       //这句返回的只是一个时间cuo
	tm tmp1;
	tm *t1 = &tmp1;
	localtime_s(t1, &tt);


	MakeFolds mfs;
	mfs.mk_profold();
	mfs.mk_input();                                  //每次对应的输入，包含五个文件
	
	mfs.mk_sup_g(sup, sig_g);
	for (int times = 0; times < 5; times++)
	{
		mfs.split(p_file, e_file);
		mfs.mk_times(times);
		mfs.mk_source();
		for (int i = 0; i < cross; i++)
		{
			mfs.mk_source_cv_nu(i);
			Parameters pra(sup, sig_g, 1, p_start, p_end, p_step, max_pattern_length, gap);
			vector<Sequence> p_sequeues, e_sequeues;
			read_file(p_sequeues, e_sequeues, i, mfs);

			Pattern_set pattern_set;
			map<int, list<int>> p_num_map;

			cdspm *cdsp = signal_process(p_sequeues, e_sequeues, pra, mfs, pattern_set, p_num_map);
			//cdsp->sort_pattern();
			pattern_set.clean();
			p_num_map = cdsp->p_num_map;

			cdsp->data_for_classify(sup, sig_g, i, p_start, p_end, p_step, p_num_map);
			p_num_map.clear();
			delete cdsp;
		}
	}

	tm tmp2;
	tt = time(NULL);
	tm *t2 = &tmp2;
	localtime_s(t2, &tt);

	ofstream fout(kind + "log.txt", ios::app);
	fout.precision(2);

	for (double p = p_start; p <= p_end; p += p_step) 
	{
		cout << "Sup = " << sup << "     Sig_G = " << sig_g << "     p = " << p << endl;
		fout << showpoint << sup << ' ' << sig_g << ' ' << p << " ";
		cout << t1->tm_mday << "__" << t1->tm_hour << ":" << t1->tm_min << ":" << t1->tm_sec << endl;
		fout << t1->tm_mday << "__" << t1->tm_hour << ":" << t1->tm_min << ":" << t1->tm_sec << " ";
		cout << t2->tm_mday << "__" << t2->tm_hour << ":" << t2->tm_min << ":" << t2->tm_sec << endl << endl << endl;
		fout << t2->tm_mday << "__" << t2->tm_hour << ":" << t2->tm_min << ":" << t2->tm_sec << endl;
	}
	fout.close();

}


int main()
{
	/* 
		step 1: read_data
		step 2: set parameters
		step 3: execute
		step 4: write result.
	*/ 
	double sup = 0.1;
	double sig_g = 0.15; 
	//double sig_l = 3.0;
	int max_pattern_length = 5; 
	int gap = 5;
	int times = 5;
	int cross = 5;  
	//double p = 0.05;
	//for (double sup = 0.1; sup >= 0.02; sup -= 0.02)
	//{
		//for (double sig_g = 2.5; sig_g < 3.1; sig_g += 0.5)
		//{
			process(sup, sig_g, max_pattern_length);
		//}
	//}
	return 0;
}
