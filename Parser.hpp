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
#include "ibd_map.hpp"

using namespace std;

void parse_line(string line, int * tuple);

int length(const char * filename);

void parse_match(const char * filename, ibd_map * match_map);

void ped_parameters(const char * filename, int *individual_count, int *snp_count);

void parse_ped_line(string line, genome_matrix * GM, int individual);

void parse_ped(const char * filename, genome_matrix * G);

void parse_ped_genotype_line(string line, genome_matrix * GM, int individual);

void parse_ped_genotype(const char * filename, genome_matrix * G);

#endif
