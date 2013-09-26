
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

#include "./dynamic_bitset.hpp"

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


GenomeMatrix::GenomeMatrix(int individuals, int SNPs)
{
	int byte_number = SNPs/16 + 1;

	Individuals = individuals;
	SNP = SNPs;
	//dynamic_bitset<unsigned short> ** Matrix;
	
	Matrix = new dynamic_bitset<unsigned short> ** [individuals];

	for (int i = 0; i < individuals; i++)
	{
		Matrix[i] = new dynamic_bitset<unsigned short> * [2];
		
		Matrix[i][0] = new dynamic_bitset<unsigned short>(byte_number, 8);
		Matrix[i][1] = new dynamic_bitset<unsigned short>(byte_number, 8);
	} 
}


GenomeMatrix::~GenomeMatrix()
{
	for (int i = 0; i < Individuals; i++)
	{
		delete [] Matrix[i][0];
		delete [] Matrix[i][1];
	}

	delete [] Matrix;
}

int GenomeMatrix::getAllele(int individual, int chromosome, int site)
{
	return (*Matrix)[individual][chromosome][site];
}

void GenomeMatrix::setAllele(int individual, int chromosome, int site, int allele)
{
	(*Matrix)[individual][chromosome][site] = allele % 2;
}
