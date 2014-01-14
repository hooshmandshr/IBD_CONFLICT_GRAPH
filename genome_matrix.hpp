
#ifndef GENOME_H
#define GENOME_H

#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;


class genome_matrix
{

private:
	vector<vector<char> > Matrix;	
	int Individuals;
	int Sites;
public:
	genome_matrix();
	~genome_matrix();

	vector<vector<char> > Information;

	void initialize(int individuals, int sites);

	void print();
	void testprint();

	void set(int individual, int site, char order);
	char get(int individual, int site);

	int individuals();
	int sites();
};

#endif
