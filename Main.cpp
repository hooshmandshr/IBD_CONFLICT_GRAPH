#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "../boost/dynamic_bitset.hpp"


using namespace std;
using namespace boost;

int main()
{
	dynamic_bitset<unsigned short> * b = new dynamic_bitset<unsigned short>(10, 8);

	for (int i = 0; i < b->size(); i++)
	{
		(*b)[i] = rand()%2;
	}

	for (int i = 0; i < b->size(); i++)
	{
		cout << (*b)[i];
	}
	cout << endl;
	return 0;
} 
