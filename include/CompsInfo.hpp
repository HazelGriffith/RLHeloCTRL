#ifndef __COMPS_INFO_HPP__
#define __COMPS_INFO_HPP__

#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

struct CompsInfo{
	
	CompsInfo() {}
	
	CompsInfo(vector<int> i_comps)
				:comps(i_comps){}
				  
	vector<int> comps;
};

ostream& operator << (ostream &out, const CompsInfo &e);

istream& operator >> (istream &in, CompsInfo &e);

#endif //__COMPS_INFO_HPP__