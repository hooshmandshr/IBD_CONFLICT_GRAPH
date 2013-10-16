#include <iostream>
#include <cstdlib>

#include "ibd_map.hpp"
#include "parser.hpp"
#include "genome_matrix.hpp"
#include "dynamic_bitset.hpp"

using namespace std;


int main(int argc, char ** argv)
{
	const char *ped_file = argv[1];
	const char *match_file = argv[2];
	
	genome_matrix * genome_map = parse_ped(ped_file);
	ibd_map * match_map = parse_match(match_file);

	return 0;	
		
	srand(1110);
	genome_matrix g(100, 1000);

	for (int i = 1; i <=100; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			g.set_allele(i, 0, j, rand()%2);
			g.set_allele(i, 1, j, rand()%2);
		}
	}

	for (int i = 1; i<=100; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			cout << g.get_allele(i, 0, j) << ' ' << g.get_allele(i, 1, j) << '\t';
		}
		cout << endl;
	}

	return 0;

}

