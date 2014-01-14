#include <iostream>
#include <set>
#include <vector>
#include <queue>
#include <map>

#include "genome_matrix.hpp"
#include "ibd_map.hpp"
#include "graph.hpp"

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

mytuple::mytuple()
{

}

mytuple::~mytuple()
{

}

bool mytuple::operator<(const mytuple& operand) const
{

	return (this->Site < operand.Site);
}

void mytuple::set(int site, int individual, int index, int vertex)
{

	this->Site = site;
	this->Individual = individual;
	this->Index = index;
	this->Vertex = vertex;
}



class conflict_graph
{

private:
	vector<vector<vector<char> > > Label;
	genome_matrix * GenomeMatrix;
	ibd_map * ibdMap;
public:
	conflict_graph(genome_matrix * GM, ibd_map * IM);
	~conflict_graph();

	void optimize_individual_graph(int I);

	void resolve_individual_graph(int I, vector<int> * cut);

	void resolve_iteration();
};


conflict_graph::conflict_graph(genome_matrix * GM, ibd_map * IM)
{

	this->GenomeMatrix = GM;
	this->ibdMap = IM;

	for (int i = 0; i < IM->First.size(); i++)
	{

		vector<vector<char> > individual;
		for (int j = 0; j < IM->First[i].size(); j++)
		{

			vector<char> row;
			for (int k = 0; k < IM->First[i][j].size(); k++)
			{
				row.push_back(0);
			}	
			individual.push_back(row);
		}
		
		this->Label.push_back(individual);
	}
}

conflict_graph::~conflict_graph()
{

}

void conflict_graph::optimize_individual_graph(int I)
{

	vector<int> active_index;

	priority_queue<mytuple> fringe;
	priority_queue<mytuple> active;

	set<mytuple> active_set;

	vector<map<int, int> > adjList;

	int vertex = 0;
	for (int i = 0; i < this->ibdMap->First[I].size(); i++)
	{

		mytuple t;
		if (this->ibdMap->First[I][i].size() > 0)
		{	
			t.set(- this->ibdMap->First[I][i][0], i, 0, vertex);
			fringe.push(t);
			map<int, int> v;
			adjList.push_back(v);
			vertex += 1;
		}
	}


	mytuple top = fringe.top();
	fringe.pop();

	mytuple topact;
	topact.set(2, -1, -1, -1);

	int site = 0;

	int count = 0;

	while (fringe.size() > 0 || active.size() > 0)
	{

		while (fringe.size() > 0 && - fringe.top().Site == site)
		{

			if ( this->ibdMap->First[I][fringe.top().Individual].size() > 1 + fringe.top().Index )
			{

				mytuple r;
				r.set(- this->ibdMap->First[I][fringe.top().Individual][fringe.top().Index + 1], fringe.top().Individual, fringe.top().Index + 1, vertex);
				
				map<int, int> v;
				adjList.push_back(v);	
				vertex += 1;
				
				fringe.push(r);
			}
			mytuple t;
			t.set(- this->ibdMap->Last[I][fringe.top().Individual][fringe.top().Index], fringe.top().Individual, fringe.top().Index, fringe.top().Vertex);

			active.push(t);
			active_set.insert(t);
			fringe.pop();
	
		}

		while (active.size() > 0 && - active.top().Site == site - 1)
		{
		
			active_set.erase(active.top());	
			active.pop();	
		}

		vector<int> allele_one;
		vector<int> allele_two;

		for (set<mytuple>::iterator it = active_set.begin(); it != active_set.end(); ++it)
		{

			


			int i = (*it).Individual;
			
			int allele = this->GenomeMatrix->get(i, site);
	
			if (allele == '-')
			{
				continue;
			}

			allele -= 48;
			if (allele > 3 || allele < 0)
			{
				this->GenomeMatrix->set(i, site, '-');
				//cerr << allele << endl;
			}

			int chromosome = this->Label[i][I][(*it).Index];

			switch (chromosome * 2 + allele * 3)
			{

				case 0:
				case 2:
				case 3:
				case 8:
					allele_one.push_back((*it).Vertex);
					break;
				case 5:
				case 6:
				case 9:
				case 11:
					allele_two.push_back((*it).Vertex);	
					break;

			}
				
		}

		
		for (int u = 0; u < allele_one.size(); u++)
		{

			for (int v = 0; v < allele_two.size(); v++)
			{

				if (adjList[u].find(v) == adjList[u].end())
				{
					adjList[allele_one[u]][allele_two[v]] = 1;
					adjList[allele_two[v]][allele_one[u]] = 1;
	
				}
				else
				{
					adjList[allele_one[u]][allele_two[v]] += 1;
					adjList[allele_two[v]][allele_one[u]] += 1;
				}
			}
		}	
	
		site += 1;	
	}

	graph G;
	G.initialize(&adjList);
	G.sweep_search(NULL, 1000);
	int cost = G.dc_local_search(NULL, 0, 0, 0);
	this->resolve_individual_graph(I, &(G.Cut));
}


void conflict_graph::resolve_individual_graph(int I, vector<int> * cut)
{
	
	vector<int> active_index;

	priority_queue<mytuple> fringe;
	priority_queue<mytuple> active;

	set<mytuple> active_set;	

	int vertex = 0;
	for (int i = 0; i < this->ibdMap->First[I].size(); i++)
	{

		mytuple t;
		if (this->ibdMap->First[I][i].size() > 0)
		{	
			t.set(- this->ibdMap->First[I][i][0], i, 0, vertex);
			fringe.push(t);
			this->Label[I][fringe.top().Individual][fringe.top().Index] = ((*cut)[vertex] + 1)/2;	
			vertex += 1;
		}
	}

	//cerr << cut->size() << " size" << endl;

	mytuple top = fringe.top();
	fringe.pop();

	mytuple topact;
	topact.set(2, -1, -1, -1);

	int site = 0;

	int count = 0;
	

	int heterozygote = 0;
	int resolved = 0; 
	while (fringe.size() > 0 || active.size() > 0)
	{

		while (fringe.size() > 0 && - fringe.top().Site == site)
		{

			if ( this->ibdMap->First[I][fringe.top().Individual].size() > 1 + fringe.top().Index )
			{

				

				mytuple r;
				r.set(- this->ibdMap->First[I][fringe.top().Individual][fringe.top().Index + 1], fringe.top().Individual, fringe.top().Index + 1, vertex);

				fringe.push(r);	
	
				this->Label[I][fringe.top().Individual][fringe.top().Index] = ((*cut)[vertex] + 1)/2;


				vertex += 1;

			
				
			}
			mytuple t;
			t.set(- this->ibdMap->Last[I][fringe.top().Individual][fringe.top().Index], fringe.top().Individual, fringe.top().Index, fringe.top().Vertex);
	
			active.push(t);
			active_set.insert(t);
			fringe.pop();
			
		}

		while (active.size() > 0 && - active.top().Site == site - 1)
		{
			
			active_set.erase(active.top());	
			active.pop();
			
		}



		if (this->GenomeMatrix->get(I, site) == '3' || this->GenomeMatrix->get(I, site) == '0')
		{
		
			site += 1;
			continue;
		}
		else
		{
			heterozygote += 1; 
		}

		map<char, int> vote;
		vote['1'] = 0;
		vote['2'] = 0;
	
		for (set<mytuple>::iterator it = active_set.begin(); it != active_set.end(); ++it)
		{

			int i = (*it).Individual;

			if (this->GenomeMatrix->get(i, site) == '-')
			{
				continue;
			}

			int chromosome_one = this->Label[I][i][(*it).Index];
			int chromosome_two = this->Label[i][I][(*it).Index];

			int allele = this->GenomeMatrix->get(i, site);

			allele -= 48;
			
			switch (chromosome_one * 25 + chromosome_two * 5 + allele * 1)
			{

				case 0:
				case 1:
				case 5:
				case 7:
				case 27:
				case 28:
				case 31:
				case 33:
						vote['1'] += 1;	
						break;

				default:
						vote['2'] += 1;
						break; 

			}

		}

		if (vote['1'] > vote['2'])
		{

			resolved += 1; 
			this->GenomeMatrix->set(I, site, '1');
		}
		else if (vote['2'] > vote['1'])
		{
	
			resolved += 1; 
			this->GenomeMatrix->set(I, site, '2');
		}

		site += 1;		
	
	}	

	cerr << heterozygote << " heterozygote sites, " << resolved << " resolved." << endl;  

}

void conflict_graph::resolve_iteration()
{

	for (int i = 1; i < this->GenomeMatrix->individuals() + 1; i++)
	{

		cerr << "Resolving individual " << i << ":" << endl;
		this->optimize_individual_graph(i);
	}
 
}

