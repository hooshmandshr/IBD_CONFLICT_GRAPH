#include <vector>
#include <iostream>
#include "ibd_segment.hpp"

using namespace std;

class ibd_map
{
private:
	vector<unsigned int> ***start_matrix;
	vector<unsigned int> ***end_matrix;
	int individual_count;
public:
	ibd_map(int individuals);
	~ibd_map();

	void add(ibd_segment * segment);

	vector<unsigned int> ***start_map();
	vector<unsigned int> ***end_map();
};


