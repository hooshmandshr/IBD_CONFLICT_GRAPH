
#include <iostream>
#include <cstdio>
#include <fstream>
#include <cctype>
#include "GenomeMatrix.hpp"

using namespace std;


class Parser{

private:
	int PEDindividuals;
	int PEDsnps;
	
	void parsePEDLine(string, GenomeMatrix *, int);			
	void setPEDProperties(const char *);		
public:
	Parser();	

	~Parser();

	void parsePED(const char * );	
};

////////////////////////////////////////////
//                                        //
////////////////////////////////////////////

Parser::Parser()
{	
}

Parser::~Parser()
{
}

void Parser::parsePED(const char * FileName)
{
	this->setPEDProperties(FileName);

	GenomeMatrix * G = new GenomeMatrix(this->PEDindividuals, this->PEDsnps);
	ifstream File(FileName);
	string line;

	if (File.is_open())
	{
		for (int i = 0; i < this->PEDindividuals; i++)
		{
			getline(File, line);
			parsePEDLine(line, G, i);
		}	
		File.close();
	}
	else
	{
		cerr << "Could not open file." << endl;
	}
}

void Parser::parsePEDLine(string line, GenomeMatrix * GM, int individual)
{
	string word;
	word.clear();

	int site = 0; 
	int spaceCount = 0;	
	for (int i = 0; i < line.size(); i++)
	{
		if (isspace(line[i]))
		{
			spaceCount++;
			continue;	
		}

		else if (spaceCount > 5)
		{
			site++;
			cerr << '\r' << site;
			GM->setAllele(individual, site%2, site, line[i]%2);	
		}	
	}
	//cerr << endl;	
}

void Parser::setPEDProperties(const char * FileName)
{

	string line;
	ifstream File(FileName);

	int count = 0;

	int length = 0;	
	bool isFirstLine = true;	
	if (File.is_open())
	{
		while(File.good())
		{
			getline(File, line);
			count ++;
				
			if (isFirstLine)
			{	
				int spaceCount = 0;
				for (int i = 0; i < line.size(); i++)
				{
					if (isspace(line[i]))
					{	
						spaceCount++;		
					}
				}
				isFirstLine = false;
				length = (spaceCount - 5)/2;
			}
		}

		count --;
		File.close();
	}
	else
	{
		cerr << "Could not open file." << endl;
	}
	

	PEDindividuals = count;
	PEDsnps = length;	
	cerr << count << " individuals read." << endl;
	cerr << length << " SNP's per individual." << endl;

}
