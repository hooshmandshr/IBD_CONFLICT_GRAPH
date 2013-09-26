

#ifndef PARSER_H
#define PARSER_H


#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "GenomeMatrix.hpp"
using namespace std;


class Parser
{

private:
	int PEDindividuals;
	int PEDsnps;

	void parsePEDLine(string, GenomeMatrix *, int);
	void setPEDProperties(const char *);
public:
	Parser();
	Parser(const char *);
	
	~Parser();

	void parsePED(const char *);	
};

#endif
