
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

#include "./dynamic_bitset.hpp"

using namespace std;
using namespace boost;


class genome_matrix{

private:
	dynamic_bitset<unsigned short int> *** Matrix;

	int Individuals;
	int SNP;
public:
	genome_matrix(int, int);

	~genome_matrix();

	int get_allele(int, int, int);
	void set_allele(int, int, int, int);
	void push_allele(int, int, int);
};


genome_matrix::genome_matrix(int individuals, int SNPs)
{
	int byte_number = SNPs/16 + 1;

	Individuals = individuals;
	SNP = SNPs;
	//dynamic_bitset<unsigned short> ** Matrix;
	
	Matrix = new dynamic_bitset<unsigned short int> ** [individuals+1];

	for (int i = 0; i < individuals+1; i++)
	{
		Matrix[i] = new dynamic_bitset<unsigned short int> * [2];
		
		Matrix[i][0] = new dynamic_bitset<unsigned short int>(byte_number, 8);
		Matrix[i][1] = new dynamic_bitset<unsigned short int>(byte_number, 8);
	}
 
}


genome_matrix::~genome_matrix()
{
/*
	for (int i = 0; i < Individuals+1; i++)
	{	
		Matrix[i][0]->clear();
		Matrix[i][1]->clear();
	}
*/
	delete [] Matrix;
}

int genome_matrix::get_allele(int individual, int chromosome, int site)
{
	return (*Matrix[individual][chromosome])[site];
}

void genome_matrix::set_allele(int individual, int chromosome, int site, int allele)
{
	(*Matrix[individual][chromosome])[site] = allele % 2;
}

void genome_matrix::push_allele(int individual, int chromosome, int allele)
{
	Matrix[individual][chromosome]->push_back(allele);
}
