#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
#include "ibd_segment.hpp" 
#include "ibd_map.hpp"
#include "genome_matrix.hpp"

using namespace std;

ibd_segment * parse_line(string line)
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

	if (null_flag)
	{
		return NULL;
	}
	ibd_segment * segment = new ibd_segment(individualOne, individualTwo, start, end);
	return segment;
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

ibd_map * parse_match(const char * filename)
{
	string line;
	int line_count = length(filename);	
	int count = 0;
	ibd_map * match_map = new ibd_map(500); 
	ifstream match_file(filename);

	cerr << "Parsing " << filename << " ..." << endl;	
	if (match_file.is_open())
	{
		int count = 0;	
		cerr << count << "0" << " complete...";	
		while (match_file.good())
		{
			getline(match_file, line);
			ibd_segment * segment = parse_line(line);
			match_map->add(segment);
			delete segment;
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

	vector<unsigned int> *** start_matrix = match_map->end_map(); 
	vector<unsigned int> *** end_matrix = match_map->start_map();	
	for (int i = 1; i <= 500; i++)
	{
		for (int j = 1; j <= 500; j++)
		{
			sort(start_matrix[i][j]->begin(), start_matrix[i][j]->end());	
			sort(end_matrix[i][j]->begin(), end_matrix[i][j]->end());		
		}
	}	 
	
	return match_map;
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
	for (int i = 0; i < line.size(); i++)
	{
		if (isspace(line[i]))
		{
			spaceCount++;
			continue;	
		}

		else if (spaceCount > 5)
		{	
			//cerr << site << ":" << site/2  << endl;	
			GM->set_allele(individual, site%2, site/2, (int(line[i])+1)%2);
			site++;
		}	
	}
}


genome_matrix * parse_ped(const char * filename)
{
	int individual_count;
	int snp_count;	
	ped_parameters(filename, &individual_count, &snp_count);

	genome_matrix * G = new genome_matrix(individual_count, snp_count);
	ifstream File(filename);
	string line;

	if (File.is_open())
	{
		cerr << '\r' << "0" << "%" << " complete...";		
		for (int i = 1; i < individual_count+1; i++)
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

	return G;
}


