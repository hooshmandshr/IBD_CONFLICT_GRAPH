#include <iostream>
#include <set>
#include <ctime>

#include "parser.hpp"
#include "genome_matrix.hpp"
#include "ibd_map.hpp"
#include "conflict_graph.hpp"
#include "graph.hpp"

using namespace std;


int main(int argc, char **argv)
{

	const char * ped_file = argv[1];
	const char * match_file = argv[2];
	
	genome_matrix gm;
	
	parse_ped_genotype(ped_file, &gm);

	ibd_map im;
	im.setup(gm.individuals());	
	
	parse_match(match_file, &im);

	conflict_graph cg(&gm, &im);

	cg.resolve_iteration();

	gm.print();

	return 0;

	time_t now = time(NULL);
	cg.optimize_individual_graph(1);
	cerr << time(NULL) - now << " seconds." << endl;

	now = time(NULL);
	cg.optimize_individual_graph(2);
	cerr << time(NULL) - now << " seconds." << endl;

	now = time(NULL);
	cg.optimize_individual_graph(500);
	cerr << time(NULL) - now << " seconds." << endl;

	cerr << "end" << endl;

	now = time(NULL);

	gm.print();

	cerr << time(NULL) - now << " seconds." << endl;

	return 0;

}


