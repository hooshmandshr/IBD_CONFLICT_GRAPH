
#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>

#include "genome_matrix.hpp"

using namespace std;

ibd_segment * parse_line(string line);
int length(const char * filename);
ibd_map * parse_match(const char * filename);

void ped_parameters(const char * filename, int * individual_count, int * snp_count);
void parse_ped_line(string line, genome_matrix * GM, int individual);
genome_matrix * parse_ped(const char * filename);

#endif
