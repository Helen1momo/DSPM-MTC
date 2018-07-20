#include "common.h"
#include <iostream>
#include <fstream>
using namespace std;

void Sequence::build_next_key()
{
	hash_map<int, int> next;
	for (int i = eles.size() - 1; i >= 0; i--)
	{
		int key = eles[i];
		next[key] = i;
		next_key.push_back(next);
	}
	int n = eles.size();
	for (int i = 0; i < n / 2; i++)
	{
		swap(next_key[i], next_key[n - 1 - i]);
	}
}


void MakeFolds::mkdir(string sys_cmd)
{
	char cmd[200] = "md ";
	int len = strlen(cmd);
	for (int i = 0; i < sys_cmd.length(); i++)
	{
		cmd[len++] = sys_cmd[i];
	}
	cmd[len++] = '\n';
	cout << cmd << endl;
	system(cmd);
}

void MakeFolds::mk_profold()
{
	profold = kind + "profold";
	mkdir(profold);
}

void MakeFolds::mk_input()
{
	input = kind + "\\" + "input\\split";
	mkdir(input);
}

void MakeFolds::split(string file_p, string file_e)
{
	ifstream fin[2];
	fin[0].open(file_p);
	fin[1].open(file_e);

	ofstream fout[2][cross];
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < cross; j++)
		{
			if (i == 0) fout[i][j].open(input + "\\p_" + char(j + '0') + ".txt");
			else fout[i][j].open(input + "\\e_" + char(j + '0') + ".txt");
		}
	}

	for (int i = 0; i < 2; i++)
	{
		string str;
		while (getline(fin[i], str))
		{
			int index = rand() % cross;
			fout[i][index] << str << endl;
		}
		fin[i].close();
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < cross; j++)
			fout[i][j].close();
	}
}

void MakeFolds::mk_sup_g(double sup, double sig_g)
{
	stringstream ss;
	ss.precision(2);
	ss << showpoint << sup << "__" << sig_g;
	string parameter = ss.str();
	ss.clear();
	sup_g = profold + "\\" + parameter;
	mkdir(sup_g);

}

void MakeFolds::mk_times(int time)
{
	char temp[100];
	_itoa_s(time, temp, 10);
	times = sup_g + "\\";
	times += temp;
	mkdir(times);
}

void MakeFolds::mk_source()
{
	source = times + "\\source";
	mkdir(source);
}

void MakeFolds::mk_source_cv_nu(int nu)
{
	char temp[100];
	_itoa_s(nu, temp, 10);
	source_cv_nu = source + "\\" + temp;
	mkdir(source_cv_nu);

	train_file = source_cv_nu + "\\" + "train.txt";
	test_file = source_cv_nu + "\\" + "test.txt";
}


void MakeFolds::mk_para_cv_nu(double sup, double sig_g, double sig_l, int cv_nu)
{
	stringstream ss;
	ss.precision(2);
	ss << showpoint << sup << "__" << sig_g<<"__"<<sig_l;
	string parameter = ss.str();
	ss.clear();
	para = times + "\\" + parameter;
	
	char temp[100];
	_itoa_s(cv_nu, temp, 10);

	para_cv_nu = para + "\\" + temp;
	mkdir(para_cv_nu);

	cdsp_pra_path = para_cv_nu + "\\cdsp.txt";
	cut_pra_path = para_cv_nu + "\\cut.txt";
	imp_pra_path = para_cv_nu + "\\imp.txt";
	red_pra_path = para_cv_nu + "\\red.txt";
	fwer_pra_path = para_cv_nu + "\\fwer.txt";

	train_red = para_cv_nu + "\\" + "train_red.txt";
	train_cdsp = para_cv_nu + "\\" + "train_cdsp.txt";
	train_cut = para_cv_nu + "\\" + "train_cut.txt";
	train_imp = para_cv_nu + "\\" + "train_imp.txt";
	train_fwer = para_cv_nu + "\\" + "train_fwer.txt";

	test_red = para_cv_nu + "\\" + "test_red.txt";
	test_cdsp = para_cv_nu + "\\" + "test_cdsp.txt";
	test_cut = para_cv_nu + "\\" + "test_cut.txt";
	test_imp = para_cv_nu + "\\" + "test_imp.txt";
	test_fwer = para_cv_nu + "\\" + "test_fwer.txt";
}