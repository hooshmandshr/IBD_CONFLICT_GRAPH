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

	void initialize(vector<map<int, int> >  * adjList);
};

graph::graph(int size, double density)
{

	this->generate_graph(size, density);				
}

graph::graph()
{


}

graph::~graph()
{

}

void graph::initialize(vector<map<int, int> > * adjList)
{

	for (int i = 0; i < adjList->size(); i++)
	{

		vector<int> edgeList;
		vector<double> weightList;

		for (map<int, int>::iterator it = (*adjList)[i].begin(); it != (*adjList)[i].end(); ++it)
		{

			edgeList.push_back(it->first);
			weightList.push_back(it->second);
		}

		this->Edge.push_back(edgeList);
		this->Weight.push_back(weightList);
	}	
}

int graph::size()
{

	return this->Edge.size();
}

void graph::setup(int size)
{

	for (int i = 0; i < size; i++)
	{

		vector<int> edge;
		vector<double> weight;

		this->Edge.push_back(edge);
		this->Weight.push_back(weight);

		this->V = size;
	}	
}

void graph::add_edge(int u, int v, int w)
{

	this->Edge[u].push_back(v);
	this->Weight[u].push_back(w);
	this->E += 1;
}

void graph::generate_graph(int size, double density)
{

	for (int i = 0; i < size; i++)
	{
		vector<int> edge;
		vector<double> weight;
		this->Edge.push_back(edge);
		this->Weight.push_back(weight);	
	}

	for (int i = 0; i < size; i++)
	{

		for (int j = 0; j < i; j++)
		{

			if ((rand()%1000)/1000 < density)
			{

				double weight = rand()%100;

				this->Edge[i].push_back(j);
				this->Weight[i].push_back(weight);	
				
				this->Edge[j].push_back(i);
				this->Weight[j].push_back(weight);	
			}
		}
	}		
}

double graph::cut_cost(vector<int> * cut)
{

	double cost = 0;
	for (int i = 0; i < this->Edge.size(); i++)
	{

		int partition = (*cut)[i];
		for (int j = 0; j < this->Edge[i].size(); j++)
		{

			if (partition != (*cut)[this->Edge[i][j]])
			{
				cost += this->Weight[i][j];
			}
		}
	}

	return cost/2;
}

double graph::flip_cost(vector<int> * cut, int vertex)
{

	double cost = 0;
	int partition = (*cut)[vertex];
	for (int u = 0; u < this->Edge[vertex].size(); u++)
	{
		
		if (partition == (*cut)[this->Edge[vertex][u]])
		{
			cost += this->Weight[vertex][u];	
		}
		else
		{
			cost -= this->Weight[vertex][u];
		}	
	}
return cost;	
}

double graph::dc_local_search(vector<int> * cut, double cost, int begin, int end)
{

	if (cut == NULL)
	{
	
		for (int i = 0; i < this->Edge.size(); i++)
		{

			this->Cut.push_back(rand()%2 * 2 - 1);	
		}

		cut = &(this->Cut);	
		
		cost = cut_cost(cut);

		return this->dc_local_search(cut, cost, 0, this->Edge.size());
	}

	else if (end - begin < 80)
	{

		int count = 0;
		while(true)
		{
			count++;
			int flip = -1;
			double gain = 0;
		
			for (int i = begin; i < end; i++)
			{
				gain = flip_cost(cut, i);
				//cerr << gain << ' ';

				if (gain > 0)
				{

					flip = i;
					break;				
				}
			}	

			if (flip == -1 or count > 1000)
			{
				break;
			}
			else
			{
				(*cut)[flip] = (*cut)[flip] * -1;
				cost += gain;
			}

			//cerr << flip << '\t' << cost << endl;	
		}	
	}
	else
	{

		int middle = begin + int((end-begin)/2);
		int quarter = int((middle-begin)/2);
		//cerr << begin << '\t' << middle << '\t' << end << endl;
		double c1 = this->dc_local_search(cut, cost, begin, middle);
		double c2 = this->dc_local_search(cut, c1, middle - quarter, middle + quarter);
		double c3 = this->dc_local_search(cut, c2, middle, end);
		double c4 = this->dc_local_search(cut, c3, middle - quarter, middle + quarter);
		double c5 = this->dc_local_search(cut, c4, begin, middle);

		cost = c5;	
	}
	
	return cost;
}

double graph::sweep_search(vector<int> * cut, int window)
{

	double cost;
	if (cut == NULL)
	{
		this->Cut.clear();	
		for (int i = 0; i < this->Edge.size(); i++)
		{

			this->Cut.push_back(rand()%2 * 2 - 1);	
		}

		cut = &(this->Cut);	
		
		cost = cut_cost(cut);	
	}
	else
	{
		cut = &(this->Cut);
		cost = cut_cost(cut);	
	}

	for (int i = 0; i < this->size(); i += int(window/2))
	{

		int limit = i + window;
		if (limit > this->size())
		{
			limit = this->size();
		}	
		cost = dc_local_search(cut, cost, i, limit);
	
	}
}

double graph::local_search()
{

	vector<int> cut;
	for (int i = 0; i < this->Edge.size(); i++)
	{

		cut.push_back(rand()%2 * 2 - 1);	
	}

	double cost = cut_cost(&cut);
	//cerr << cost << endl;

	int count = 0;

	for (int offset = 0; offset < 500; offset += 250)
	{
		int sweep_length = 500;	
		for (int sweep = offset; sweep < this->Edge.size(); sweep += sweep_length)
		{

			int limit = sweep + sweep_length;
			if (limit > this->Edge.size())
			{
		
				limit = this->Edge.size();
			}

			count = 0;
	
			while(true)
			{
				count++;
				int flip = -1;
				double gain = 0;
		
				for (int i = sweep; i < limit; i++)
				{
					gain = flip_cost(&cut, i);
					//cerr << gain << ' ';

					if (gain > 0)
					{

						flip = i;
						break;				
					}
				}
				cerr << endl;

				if (flip == -1 or count > 1000)
				{
					break;
				}
				else
				{
					cut[flip] = cut[flip] * -1;
					cost += gain;
				}

				cerr << flip << '\t' << cost << endl;	
			}
		}	
	}	
/*
	while(true)
	{
		count++;
		int flip = -1;
		double gain = 0;
		
		for (int i = sweep; i < this->Edge.size(); i++)
		{
			gain = flip_cost(&cut, i);
			//cerr << gain << ' ';

			if (gain > 0)
			{

				flip = i;
				break;				
			}
		}
		cerr << endl;

		if (flip == -1 or count > 1000)
		{
			break;
		}
		else
		{
			cut[flip] = cut[flip] * -1;
			cost += gain;
		}

		cerr << flip << '\t' << cost << endl;	
	}

*/

	return cost;
}

//   <Backtrack Optimal Solution>
//
double graph::optimal_solution()
{

	vector<int> cut;
	for(int i = 0; i < this->Edge.size(); i++)
	{
		cut.push_back(-1);	
	}

	double max = 0;

	while(true)
	{
		double cost = cut_cost(&cut);
		if (max < cost)
		{
			max = cost;
		}

		int status = this->next_cut(&cut, 0);
		if (status)
		{
			break;
		}
	}
return max;	
}

int graph::next_cut(vector<int> * cut, int vertex)
{

	if (vertex >= this->Edge.size())
	{
		return 1;
	}	
	else if ((*cut)[vertex] == -1)
	{
		(*cut)[vertex] = 1;
		return 0;
	}
	else
	{
		(*cut)[vertex] = -1;
		return this->next_cut(cut, vertex+1);
	
	}

}

//   </Backtrack Optimal Solution>
//


/*
 
int main(int argc, char **argv)
{


	//srand(time(NULL));
	time_t start = time(NULL);
	int size = atoi(argv[1]);

	graph s(size, 0.05);

	srand(time(NULL));

	double cost = 0;

	//cost = s.dc_local_search(NULL, 0, 0, 0); 

	cost = s.sweep_search(NULL, 1000);
 	
	cerr << cost << endl; 

	time_t end = time(NULL);	
	cerr << "IT took " << end - start << " seconds." << endl;


	return 0;


	int N = atoi(argv[2]);
	double *  solution= new double[N];

	solution[0] = s.local_search();
	for (int i = 1; i < N; i++)
	{
		solution[i] = s.local_search();
	}

	//double optimum = s.optimal_solution();
 
	for (int i = 0; i < N; i++)
	{
		//cerr << solution[i]/optimum*100 << '\t';
		cerr << solution[i] << '\t'; 
	}
	cerr << endl;


	
	//cerr << "Optimal solutions: " <<  optimum << endl;

	time_t end = time(NULL);	

	cerr << "IT took " << end - start << " seconds." << endl;

		
	return 0;

}

*/

 
