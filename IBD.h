#include <vector>
#include <stdint.h>

using namespace std;

#ifndef IBD_H
#define IBD_H

struct IBD{
	int startSNP;
	int endSNP;
};

vector<IBD> ** PIBD(vector< vector<uint8_t> >);
void sortByStart(vector<IBD>, int, int);

#endif

