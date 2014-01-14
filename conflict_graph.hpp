#ifndef CONFLICT_H
#define CONFLICT_H

#include <iostream>
#include <set>
#include <vector>
#include <queue>

#include "genome_matrix.hpp"
#include "ibd_map.hpp"

using namespace std;


class mytuple
{

public:
	int Site;
	int Individual;
	int Index;
	int Vertex;

	mytuple();
	~mytuple();
		
	bool operator<(const mytuple& operand) const;

	void set(int site, int individual, int index, int vertex);
	
};

class conflict_graph
{

private:
	vector<vector<vector<char> > > Label;
	genome_matrix * GenomeMatrix;
	ibd_map * ibdMap;
public:
	conflict_graph(genome_matrix * GM, ibd_map * IM);
	~conflict_graph();

	void resolve_individual_graph(int I, vector<int> * cut);

	void optimize_individual_graph(int I);

	void resolve_iteration();
};


#endif

