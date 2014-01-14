#ifndef IBDMAP_H
#define IBDMAP_H

#include <iostream>

using namespace std;

class ibd_map
{

public:
	vector<vector<vector<int> > > First;
	vector<vector<vector<int> > > Last;


	ibd_map();
	~ibd_map();

	void setup(int individuals);
	void add(int one, int two, int start, int last);
};

#endif
