#include <iostream>
#include <vector>

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

ibd_map::ibd_map()
{

}

ibd_map::~ibd_map()
{

}

void ibd_map::setup(int individuals)
{

	for (int i = 0; i < individuals + 1; i++)
	{

		vector<vector<int> > individual_first;
		vector<vector<int> > individual_last;
		for (int j = 0; j < individuals + 1; j++)
		{

			vector<int> f;
			vector<int> l;

			individual_first.push_back(f);
			individual_last.push_back(l);

		}

		this->First.push_back(individual_first);
		this->Last.push_back(individual_last);
	}
}


void ibd_map::add(int one, int two, int start, int last)
{

	this->First[one][two].push_back(start);
	this->Last[one][two].push_back(last);
}



