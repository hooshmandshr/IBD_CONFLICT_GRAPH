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


ibd_map::ibd_map(int individuals)
{
	this->individual_count = individuals;
	start_matrix = new vector<unsigned int> **[individuals+1];
	end_matrix = new vector<unsigned int> **[individuals+1];
	
	for (int i = 0; i < individuals+1; i++)
	{
		start_matrix[i] = new vector<unsigned int> *[individuals+1];
		end_matrix[i] = new vector<unsigned int> *[individuals+1];

		for (int j = 0; j < individuals+1; j++)
		{
			start_matrix[i][j] = new vector<unsigned int>();
			end_matrix[i][j] = new vector<unsigned int>();	
		}
	}
}

ibd_map::~ibd_map()
{
	for (int i = 0; i <= this->individual_count; i++)
	{
		for (int j = 0; j <= this->individual_count; j++)
		{
			delete this->start_matrix[i][j];
			delete this->end_matrix[i][j];
		}
		delete [] this->start_matrix[i];
		delete [] this->end_matrix[i];
	}
	delete [] this->start_matrix;
	delete [] this->end_matrix;	
}

void ibd_map::add(ibd_segment * segment)
{	

	if (segment == NULL)
	{
		return;
	}
	this->start_matrix[segment->individual_one()][segment->individual_two()]->push_back(segment->start());
	this->end_matrix[segment->individual_one()][segment->individual_two()]->push_back(segment->end());

	this->start_matrix[segment->individual_two()][segment->individual_one()]->push_back(segment->start());
	this->end_matrix[segment->individual_two()][segment->individual_one()]->push_back(segment->end());


}

vector<unsigned int> ***ibd_map::start_map()
{
	return this->start_matrix;
}

vector<unsigned int> ***ibd_map::end_map()
{
	return this->end_matrix;
}
