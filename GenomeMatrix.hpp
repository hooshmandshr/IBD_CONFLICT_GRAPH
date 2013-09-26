

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

class GenomeMatrix{

private:
	dynamic_bitset<unsigned short> *** Matrix;
	int Individuals;
	int SNP;
public:

	GenomeMatrix(int, int);

	~GenomeMatrix();

	int getAllele(int, int, int);
	void setAllele(int, int, int, int);	
};


#endif
