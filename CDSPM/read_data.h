#pragma once
#include "common.h"
#include "parameters.h"
#include "cdspm.h"

void read_data(string file, vector<Sequence> & sequeue, int gap);


void read_data(string file, vector<Sequence> & sequences);

void read_file(vector<Sequence> &p_sequeues, vector<Sequence>& e_sequeues, int k, MakeFolds mfs);

