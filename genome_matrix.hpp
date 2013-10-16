

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

#include "./dynamic_bitset.hpp"

using namespace std;
using namespace boost;


#ifndef GENOME_H
#define GENOME_H

using namespace std;
using namespace boost; 

class genome_matrix{

private:
	dynamic_bitset<unsigned short> *** Matrix;
	int Individuals;
	int SNP;
public:

	genome_matrix(int, int);

	~genome_matrix();

	int get_allele(int, int, int);
	void set_allele(int, int, int, int);	
	void push_allele(int, int, int);
};


#endif
