/*
 * IBD.cpp
 *
 *  Created on: Apr 22, 2013
 *      Author: hooshmand
 */

#include <iostream>
#include <vector>
#include <stdint.h>
#include <algorithm>
#include <cmath>

using namespace std;


struct IBD{

	//int individual;
	int startSNP;
	int endSNP;

};

//sort
int * sortByStart(vector<IBD> list, int first, int last){

	int n = last - first;
	int * sortedLabel = new int [n];

	if (n < 2){

		sortedLabel[0] = first;

		return sortedLabel;
	}

	int middle = (last - first) / 2;

	cerr << middle << endl;

	int * firstHalf = sortByStart(list, first, middle);
	int * secondHalf = sortByStart(list, middle, last);

	int n1 = middle - first;
	int n2 = last - middle;
	int j1, j2 = 0;

	for (int i = 0; i < n; i++){
		if (firstHalf[j1] > secondHalf[j2] && j2 < n2) {
			sortedLabel[i] = secondHalf[j2];
			j2++;
		}
		else if(j1 < n1){
			sortedLabel[i] = firstHalf[j1];
			j1++;
		}
	}

	delete [] firstHalf;
	delete [] secondHalf;

	return sortedLabel;

}

vector<IBD> ** PIBD(vector< vector<uint8_t> > GenomeMatrix){

	int IBDcount = 0;

	//number of individuals
	int N = GenomeMatrix.size();

	//number of SNP's
	int S = GenomeMatrix[0].size();

	vector<IBD> ** IBDmatrix;

	IBDmatrix = new vector<IBD> * [N];

	for (int i = 0; i < N; i++){
		IBDmatrix[i] = new vector<IBD> [N];
	}

	for (int i1 = 0; i1 < N; i1++){

		//currentIBD.clear();

		for (int i2 = i1 + 1; i2 < N; i2++){

			int startPoint = 0;
			int endPoint;

			for (int s = 0; s < S; s++){

				uint8_t g1 = GenomeMatrix[i1][s];
				uint8_t g2 = GenomeMatrix[i2][s];

				if ( (g1 == 48 && g2 == 50) || (g1 == 50 && g2 == 48) ){

					if( s - startPoint > 30 ){

						IBDcount ++;

						IBD newIBD;
						//newIBD.individual = i2;
						newIBD.endSNP = s;
						newIBD.startSNP = startPoint;

						//currentIBD.push_back(newIBD);
						IBDmatrix[i1][i2].push_back(newIBD);
						IBDmatrix[i2][i1].push_back(newIBD);
					}

					startPoint = s + 1;
				}
			}
		}

		//int * sortedlist = sortByStart(currentIBD, 0, currentIBD.size());
		//delete [] sortedlist;



		//cerr << "Individual " << i1 << " done!" << endl;
		//cerr << IBDcount << " IBD found!" << endl;

		if (i1 == 0){
			cerr << (double(i1+1)/N)*100 << "% completed...";
		}
		else{

			cerr << '\r' << (double(i1+1)/N)*100 << "% completed...";
		}

		IBDcount = 0;
	}

	cerr << endl;

	return IBDmatrix;
}







