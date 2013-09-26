/*
 * IBDgraph.h
 *
 *  Created on: Apr 28, 2013
 *      Author: hooshmand
 */

#include <map>
#include <vector>
#include <stdint.h>
#include "IBD.h"

using namespace std;

#ifndef IBDGRAPH_H
#define IBDGRAPH_H

class graph{

private:
	int individual;
	int V;
	map<int, int> AdjList;
	int * offset;
	int * CUT;
	vector<IBD> **IBDmatrix;
	vector<uint8_t> * haplotype;

public:

	graph(vector<IBD> **, vector<vector<uint8_t> > *, int);
	~graph();

	int getSize();
	map<int, int> * getAdjList();
	void maxCut();
	void maxCut2();
	void phase();
	int findIBD(int , int);
	vector<uint8_t> * getHaplotype();

};


#endif
