
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>

#include <vector>
#include <map>


using namespace std;

class graph 
{

private:

	vector<vector<int> > Edge;
	vector<vector<double> > Weight;
	int V;
	int E;

	void generate_graph(int size, double density);
	int next_cut(vector<int> * cut, int vertex);
public:
	graph(int size, double density);
	graph();
	~graph();

	vector<int> Cut;

	void setup(int size);

	void add_edge(int u, int v, int w);
	
	int size();

	double cut_cost(vector<int> * cut);

	double flip_cost(vector<int> * cut, int vertex);
	double local_search();

	double dc_local_search(vector<int> * cut, double cost, int begin, int end);

	double optimal_solution();

	double sweep_search(vector<int> * cut, int window);

	void initialize(vector<map<int, int> > * adjList);

};


#endif

