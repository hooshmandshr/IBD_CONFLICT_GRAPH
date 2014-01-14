#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>

#include "genome_matrix.hpp"
#include "ibd_map.hpp"

using namespace std;

void parse_line(string line, int * tuple)
{
	int individualOne;
	int individualTwo;
	int start;
	int end;

	bool null_flag = true;	
	string element = "";	
	int count = 0;	
	for (int index=0; index < line.size(); index++)
	{	
		null_flag = false;
		if (isspace(line.at(index)))
		{
			switch (count)
			{
				case 1:
					individualOne = atoi(element.c_str());
					break;
				case 3:
					individualTwo = atoi(element.c_str());
					break;
				case 7:
					start = atoi(element.c_str());
					break;
				case 8:
					end = atoi(element.c_str());
					break;
				default:
					break;	
			}
			count++;	
			element.clear();	
		}
		else
		{
			element += line.at(index);	
		}
			
	}


	tuple[0] = individualOne;
	tuple[1] = individualTwo;
	tuple[2] = start;
	tuple[3] = end;
		
	if (null_flag)
	{
		tuple[0] = 0;
		tuple[1] = 0;
		tuple[2] = 0;
		tuple[3] = 0;
	
	}
	//ibd_segment * segment = new ibd_segment(individualOne, individualTwo, start, end);
	
}


int length(const char * filename)
{
	// Determining the lenght of the match file
	int line_count = 0;
	string line;
	ifstream _match_file(filename);
	if (_match_file.is_open())
	{		
		while (_match_file.good())
		{
			getline(_match_file, line);	
			line.clear();
			line_count++;
		}	
		_match_file.close();
	}
	return line_count;
}

void parse_match(const char * filename, ibd_map * match_map)
{

	//int individual_count;
	//int snp_count;	
	//ped_parameters(ped_file, &individual_count, &snp_count);

	string line;
	int line_count = length(filename);	
	int count = 0;
	
	//ibd_map match_map;
	//match_map.setup(individual_count);

	ifstream match_file(filename);

	int segment[4];
	cerr << "Parsing " << filename << " ..." << endl;	
	if (match_file.is_open())
	{
		int count = 0;	
		cerr << count << "0" << " complete...";	
		while (match_file.good())
		{
			getline(match_file, line);
			parse_line(line, segment);
			match_map->add(segment[0], segment[1], segment[2], segment[3]);	
			match_map->add(segment[1], segment[0], segment[2], segment[3]);
			line.clear();
			count++;
			if (count %10000 == 0)
			{
				cerr << '\r' << int(count/double(line_count)*100) << "%" << " complete...";
			}	
		}
		cerr << '\r' << "100%" << " complete." << endl;
		match_file.close();
	}	
	cerr << "Match file fully read." << endl;	

}

//
//
void ped_parameters(const char * filename, int *individual_count, int *snp_count) 
{

	string line;
	ifstream File(filename);

	int count = 0;

	cerr << "Parsing PED file " << filename << " ..." << endl;
	int length = 0;	
	bool is_first= true;	
	if (File.is_open())
	{
		while(File.good())
		{
			getline(File, line);
			count ++;
				
			if (is_first)
			{	
				int spaceCount = 0;
				for (int i = 0; i < line.size(); i++)
				{
					if (isspace(line[i]))
					{	
						spaceCount++;		
					}
				}
				is_first = false;
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
	

	*individual_count = count;
	*snp_count = length;	
	cerr << count << " individuals read." << endl;
	cerr << length << " SNP's per individual." << endl;

}


void parse_ped_line(string line, genome_matrix * GM, int individual)
{
	string word;
	word.clear();

	int site = 0; 
	int spaceCount = 0;	
	int genotype;
	for (int i = 0; i < line.size(); i++)
	{
		if (isspace(line[i]))
		{
			spaceCount++;
			continue;
		}

		else if (spaceCount > 5)
		{

			int allele = 0;
			if (line[i] == '1')
			{
				allele = 0;	
			}
			else if (line[i] == '2')
			{
				allele = 1;
			}

			if (site%2 == 0)
			{

				genotype = allele;		
			}
			else
			{

				genotype += (allele * 2);
				GM->set(individual, site/2, '0' + genotype);
			}
			//cerr << site << ":" << site/2  << endl;
			//
			site += 1;	
		}	
	}
}

void parse_ped(const char * filename, genome_matrix * G)
{
	int individual_count;
	int snp_count;	
	ped_parameters(filename, &individual_count, &snp_count);
	G->initialize(individual_count, snp_count);

	ifstream File(filename);
	string line;

	if (File.is_open())
	{
		cerr << '\r' << "0" << "%" << " complete...";		
		for (int i = 1; i < individual_count - 1; i++)
		{
			cerr << '\r' << int(i*100/(double(individual_count))) << "%" << " complete...";				
			getline(File, line);
			parse_ped_line(line, G, i);
	
		}

		cerr << '\r' << "100" << "%" << " complete..." << endl;
		cerr << "PED file fully read." << endl;
		File.close();
	}
	else
	{
		cerr << "Could not open file." << endl;
	}

}

void parse_ped_genotype_line(string line, genome_matrix * GM, int individual)
{
	string word;
	word.clear();

	int site = 0; 
	int spaceCount = 0;	
	int genotype;
	for (int i = 0; i < line.size(); i++)
	{
		if (isspace(line[i]))
		{

			if (spaceCount <= 5)
			{

				GM->Information[individual].push_back(line[i]);
			}
			spaceCount++;
			continue;	
		}

		else if (spaceCount > 5)
		{

			int allele = 0;
			if (line[i] == '1')
			{
				allele = 0;	
			}
			else if (line[i] == '2')
			{
				allele = 1;
			}

			if (site%2 == 0)
			{

				genotype = allele;		
			}
			else
			{

				genotype += (allele * 2);
				if (genotype == 1 || genotype == 2)
				{
					GM->set(individual, site/2, '-');
				}
				else
				{
					GM->set(individual, site/2, '0' + genotype);
				}
			}
			//cerr << site << ":" << site/2  << endl;
			//
			site += 1;	
		}

		else
		{
			GM->Information[individual].push_back(line[i]);
		}
		
	}
	
}

void parse_ped_genotype(const char * filename, genome_matrix * G)
{
	int individual_count;
	int snp_count;	
	ped_parameters(filename, &individual_count, &snp_count);
	G->initialize(individual_count, snp_count);

	//genome_matrix * G = new genome_matrix(individual_count, snp_count);
	
	//genome_matrix G;
	//G.initialize(individual_count, snp_count);
	
	ifstream File(filename);
	string line;

	if (File.is_open())
	{
		cerr << '\r' << "0" << "%" << " complete...";		
		for (int i = 1; i < individual_count + 1; i++)
		{
			cerr << '\r' << int(i*100/(double(individual_count))) << "%" << " complete...";			
			getline(File, line);
			parse_ped_genotype_line(line, G, i);
		}

		cerr << '\r' << "100" << "%" << " complete..." << endl;
		cerr << "PED file fully read." << endl;
		File.close();
	}
	else
	{
		cerr << "Could not open file." << endl;
	}

}


