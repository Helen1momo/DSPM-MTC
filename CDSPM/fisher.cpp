#include "cdspm.h"
#include <iostream>
#include "cmath"

double cdspm::getlog(int n)
{
	double res = 0.0;
	if (n == 0)
		return log(1.0);
	if (n <= p_map.size())
		res = p_map[n];
	else{
		if (p_map.size() != 0)
			res = p_map[p_map.size()];
		for (int i = p_map.size(); i < n; i++)
		{
			res = res + log(1.0 * (i+1));
			p_map[i + 1] = res;
		}
	}
	return res;
}

double cdspm::getfisher(int a, int b, int c, int d)
{
	double res = 0.0;
	res = getlog(a + b) + getlog(c + d) + getlog(a + c) + getlog(b + d) - getlog(a) - getlog(b) - getlog(c) - getlog(d) - getlog(a + b + c + d);

	res = exp(res);

	double q = (1.0*c*b) / ((a+1.0)*(d+1.0));

	int min = (c > b) ? b : c;
	res = res *((1.0 - pow(q*1.0, min*1.0+1.0)) / (1.0 - q*1.0));

	return res;
}