#include "read_data.h"
#include "cdspm.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

void read_data(string file, vector<Sequence> & sequences)
{
	cout << file << endl;
	ifstream fin(file);
	string str;
	while (getline(fin, str))
	{
		Sequence sequence;
		stringstream ss(str);
		int int_temp;
		ss >> sequence.lable;
		while (ss >> int_temp)
		{
			sequence.eles.push_back(int_temp);
		}

		sequence.build_next_key();

		sequences.push_back(sequence);
	}
}

void cdspm::read_patterns(string source_file, vector<Pattern> &patterns, bool is_cdsp, bool is_fwer, bool is_train, int pos, int neg)
{
	ifstream fin(source_file);
	string str;

	int po = 0;
	int ne = 0;
	set<Pattern> setp;
	while (getline(fin, str))
	{
		Pattern pattern;
		stringstream ss(str);
		int int_temp;
		ss >> int_temp;

		pattern.pos = int_temp;
		if (int_temp == 0)
			po+=1;
		else
			ne+=1;


		if (ne > neg)//负极都找完了
			break;
		if (po > pos && ne == 0)//刚好找够正集但负集还没找，跳过
			continue;

		while (ss >> int_temp)
		{
			pattern.eles.push_back(int_temp);
		}

		if ((po <= pos) || (ne <= neg && ne != 0))//正集开始找或负极开始找
		{
			if (setp.empty() == true){ //集合为空直接加入
				setp.insert(pattern);
				patterns.push_back(pattern);
				ss.clear();
			}
			else{//集合不为空先判断有没有存在
				set<Pattern>::iterator ite1 = setp.begin();
				ite1 = setp.find(pattern);
				if (ite1 == setp.end()){ //不存在就加入
					setp.insert(pattern);
					patterns.push_back(pattern);
					ss.clear();
				}
			}
		}
	};
	fin.close();

	if (is_cdsp && is_train)
	{
		ofstream fout(mfs.cdsp_pra_path, ios::app);
		for (int i = 0; i < patterns.size(); i++)
		{
			fout << patterns[i].pos;
			for (int j = 0; j<patterns[i].eles.size(); j++)
			{
				fout << " " << patterns[i].eles[j];
			}
			fout << endl;
		}
		fout.close();
	}

	if (is_fwer && is_train)
	{
		ofstream fout(mfs.fwer_pra_path, ios::app);
		for (int i = 0; i < patterns.size(); i++)
		{
			fout << patterns[i].pos;
			for (int j = 0; j<patterns[i].eles.size(); j++)
			{
				fout << " " << patterns[i].eles[j];
			}
			fout << endl;
		}
		fout.close();
	}
}

void read_file(vector<Sequence> &p_sequeues, vector<Sequence>& e_sequeues, int k, MakeFolds mfs)
{
	ifstream fin[2][cross];
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < cross; j++)
		{
			if (i == 0) fin[i][j].open(mfs.input + "\\p_" + char(j + '0') + ".txt");
			else fin[i][j].open(mfs.input + "\\e_" + char(j + '0') + ".txt");
		}
	}

	ofstream fout[2];
	fout[0].open(mfs.train_file);
	fout[1].open(mfs.test_file);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < cross; j++)
		{
			string str;
			while (getline(fin[i][j], str))
			{
				Sequence sequence;
				stringstream ss(str);
				int int_temp;
				while (ss >> int_temp)
				{
					sequence.eles.push_back(int_temp);
				}

				sequence.build_next_key(); //建立序列的next指针

				if (j != k) // 这是训练数据
				{
					if (i == 0)
					{
						p_sequeues.push_back(sequence);
					}
					else e_sequeues.push_back(sequence);
					fout[0] << i << " " << str << endl;
				}
				else if (j == k) //这是测试数据
				{
					fout[1] << i << " " << str << endl;
				}
			}
			fin[i][j].close();
		}
	}
	fout[0].close();
	fout[1].close();
}

void cdspm::data_for_classify(double sup, double sig_g,  int cv_nu, double p_start, double p_end, double p_step, map<int, list<int>> map1)
{
	vector<Sequence> train_sequences;
	vector<Sequence> test_sequences;

	read_data(mfs.train_file, train_sequences); //将文件中的内容放到序列集合中
	read_data(mfs.test_file, test_sequences);
	
	for (double p = p_start; p <= p_end; p += p_step)//转成训练和测试的0/1文件
	{
	    //sig_l = 0;
		mfs.mk_para_cv_nu(sup, sig_g, p, cv_nu);

		int q = p * 100;
		//map<int, list<int>>::iterator iter = map1.find(p);
		list<int> list_p = map1[q];

		int neg_fdr = list_p.back();
		list_p.pop_back();
		int neg_fwer = list_p.back();
		list_p.pop_back();
		int pos_fdr = list_p.back();
		list_p.pop_back();
		int pos_fwer = list_p.back();
		list_p.pop_back();

		//FDR
		trans_data_for_classify(train_sequences, mfs.cdsp_output_path, mfs.train_cdsp, true, false, true, pos_fdr, neg_fdr);
		trans_data_for_classify(test_sequences, mfs.cdsp_output_path, mfs.test_cdsp, true, false, false, pos_fdr, neg_fdr);

		//FWER
		trans_data_for_classify(train_sequences, mfs.fwer_output_path, mfs.train_fwer, false, true, true, pos_fwer, neg_fwer);
		trans_data_for_classify(test_sequences, mfs.fwer_output_path, mfs.test_fwer, false, true, false, pos_fwer, neg_fwer);
		/*
		if (sig_l < 1.09)
		{
			trans_data_for_classify(train_sequences, mfs.red_output_path, mfs.train_red);
			trans_data_for_classify(train_sequences, mfs.imp_output_path, mfs.train_imp);
			trans_data_for_classify(train_sequences, mfs.cut_output_path, mfs.train_cut);

			trans_data_for_classify(test_sequences, mfs.red_output_path, mfs.test_red);
			trans_data_for_classify(test_sequences, mfs.imp_output_path, mfs.test_imp);
			trans_data_for_classify(test_sequences, mfs.cut_output_path, mfs.test_cut);
		}
		*/
	}
}

void cdspm::trans_data_for_classify(vector<Sequence>&  sequences, string source_file, string target_file, bool is_cdsp, bool is_fwer, bool is_train, int pos, int neg)
{
	vector<Pattern>  patterns; 
	read_patterns(source_file, patterns, is_cdsp, is_fwer, is_train, pos, neg);

	ofstream fout(target_file,ios::trunc);
	string str;
	for (int i = 0; i < sequences.size(); i++)
	{
		int label = sequences[i].lable;
		vector<int> temp;
		
		for (int j = 0; j < patterns.size(); j++)
		{
			int start = 0;
			int last = -1;
			for (int k = 0; k < patterns[j].eles.size(); k++)
			{
				int val = patterns[j].eles[k];
				if (start>=sequences[i].eles.size() || sequences[i].next_key[start].count(val) == 0)
				{
					break;
				}
				start = sequences[i].next_key[start][val] + 1;
				last = k;
			}
			if (last == patterns[j].eles.size() - 1)
			{
				temp.push_back(j);
			}
		}
		fout << label;
		for (int j = 0; j < temp.size(); j++)
		{
			fout << " " << temp[j];
		}
		fout << endl;
	}
	fout.close();
}