
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <stdint.h>
#include <cstdlib>
#include "IBD.h"

using namespace std;

class graph{

private:

	int individual;
	int V;
	map<int, int> * AdjList;
	int * offset;
	int * CUT;
	vector<IBD> **IBDmatrix;
	vector<vector<uint8_t> > * GenomeMatrix;
	vector<uint8_t> * haplotype;

public:

	graph(vector<IBD> **, vector<vector<uint8_t> > *, int);
	~graph();

	int getSize();
	map<int, int> * getAdjList();
	void maxCut();
	void maxCut2();
	void phase();
	int findIBD(int , int);
	vector<uint8_t> * getHaplotype();

};



graph::graph(vector<IBD> **IBDMatrix, vector<vector<uint8_t> > *GENOMEMatrix, int Individual){

	this->IBDmatrix = IBDMatrix;
	this->individual = Individual;

	haplotype = new vector<uint8_t>;

	GenomeMatrix = GENOMEMatrix;

	//cerr << "Entering constructor" << endl;

	int S = GenomeMatrix->at(individual).size();
	int N = GenomeMatrix->size();

	//cerr << "Initializing offset" << endl;

	offset = new int [N];

	for (int i = 0; i < N; i++){

		offset[i] = 0;
	}

	for (int i = 1; i < N; i++){

		offset[i] = offset[i - 1] + IBDmatrix[individual][i - 1].size();
	}

	V = offset[N - 1] + IBDmatrix[individual][N-1].size();

	CUT = new int [V];

	//cerr << V << endl;

	//cerr << "Initializing Adjacency List" << endl;

	AdjList = new map<int, int> [V];

	//TODO:
	//	Debug purpuse
	//cerr << "Initialized with no problem!" << endl;

	//points to current IBD shared with the i'th person that we're moving on
	int * pointer = new int[N];
	for(int i = 0; i < N; i++){
		pointer[i] = 0;
	}

	vector<int> zeros;
	vector<int> twos;

	for (int s = 0; s < S; s++){

		if (GenomeMatrix->at(individual)[s] != '1'){
			continue;
		}

		zeros.clear();
		twos.clear();

		for(int i2= 0; i2 < N; i2++){

			if (i2 == individual){
				continue;
			}

			IBD ibd = IBDmatrix[individual][i2][pointer[i2]];

			while(ibd.endSNP < s){
				pointer[i2]++;
				ibd = IBDmatrix[individual][i2][pointer[i2]];
			}

			if (ibd.startSNP > s){
				continue;
			}

			if (GenomeMatrix->at(i2)[s] == '0'){
				zeros.push_back(offset[i2] + pointer[i2]);
			}
			else if(GenomeMatrix->at(i2)[s] == '2'){
				twos.push_back(offset[i2] + pointer[i2]);
			}
		}

		for (int u = 0; u < zeros.size(); u++){
			for (int v = 0; v < twos.size(); v++){
				/*
				if (AdjList[u][v] == NULL){
					AdjList[u][v] = 0;
				}
				else{
					AdjList[u][v]++;
				}
				*/
				AdjList[u][v]++;
			}
		}
	}

}

void graph::maxCut(){

	long int objective = 0;
	long int nextObjective;
	int * nextCut;
	int **c;
	int status = 0;

	for (int i = 0; i < V; i++){

		CUT[i] = 0;
	}

	for (int i = 0; i < V; i++){

		CUT[i] = rand()%2;
		if (CUT[i] == 1){

			for (map<int, int>::iterator it = AdjList[i].begin(); it != AdjList[i].end(); ++it){
				if (CUT[it->first] == 0){
					objective += it->second;
				}
			}
		}
	}

	int step = 0;

	while(step < 1000000){

		step++;

		nextObjective = objective;

		int u = (rand()*32760+rand())%V;

		for (map<int, int>::iterator it = AdjList[u].begin(); it != AdjList[u].end(); ++it){

			if(CUT[it->first == CUT[u]]){
				nextObjective += it->second;
			}
			else{
				nextObjective -= it->second;
			}
		}

		if (nextObjective > objective){

			step = 0;

			CUT[u] *= -1;
			CUT[u] += 1;

			objective = nextObjective;
			cerr << objective << endl;
		}
	}

	int N = GenomeMatrix->size();
	int S = GenomeMatrix->at(individual).size();
	vector<int> zeros;
	vector<int> twos;

	cerr << "entering phase 2" << endl;

	step = 0;
	try{
	while (step < 100000){

		step++;

		c = new int *;
		nextCut = *c;
		nextCut = new int[V];
		for (int i = 0; i < V; i++){

			nextCut[i] = CUT[i];
		}
		nextObjective = objective;

		zeros.clear();
		twos.clear();

		int snp = (rand()*32760+rand()) % S;

		for (int i = 0; i < N; i++){

			int u = findIBD(i, snp);
			if (u != -1){
				switch (GenomeMatrix->at(i)[snp]){
				case '0':
					zeros.push_back(u);
					break;
				case '2':
					twos.push_back(u);
					break;
				}
			}
		}

		for (int i = 0; i < zeros.size(); i++){
			int u = zeros[i];
			if (nextCut[u] == 1){
				for (map<int, int>::iterator it = AdjList[u].begin(); it != AdjList[u].end(); ++it){
					if (nextCut[it->first] == 1){
						nextObjective += it->second;
					}
					else{
						nextObjective -= it->second;
					}
				}
				nextCut[u] = 0;
			}
		}

		for (int i = 0; i < twos.size(); i++){
			int u = twos[i];
			if (nextCut[u] == 0){
				for (map<int, int>::iterator it = AdjList[u].begin(); it != AdjList[u].end(); ++it){
					if (nextCut[it->first] == 1){
						nextObjective += it->second;
					}
					else{
						nextObjective -= it->second;
					}
				}
				nextCut[u] = 1;
			}
		}

		if (nextObjective > objective){
			objective = nextObjective;
			delete [] CUT;
			CUT = nextCut;
			step = 0;
			cerr << objective << endl;
			continue;
		}

		delete c;
		c = new int *;
		nextCut = *c;
		nextCut = new int[V];
		for (int i = 0; i < V; i++){

			nextCut[i] = CUT[i];
		}
		nextObjective = objective;

		for (int i = 0; i < zeros.size(); i++){
			int u = zeros[i];
			if (nextCut[u] == 0){
				for (map<int, int>::iterator it = AdjList[u].begin(); it != AdjList[u].end(); ++it){
					if (nextCut[it->first] == 0){
						nextObjective += it->second;
					}
					else{
						nextObjective -= it->second;
					}
				}
				nextCut[u] = 1;
			}
		}

		for (int i = 0; i < twos.size(); i++){
			int u = twos[i];
			if (nextCut[u] == 1){
				for (map<int, int>::iterator it = AdjList[u].begin(); it != AdjList[u].end(); ++it){
					if (nextCut[it->first] == 1){
						nextObjective += it->second;
					}
					else{
						nextObjective -= it->second;
					}
				}
				nextCut[u] = 0;
			}
		}

		if (nextObjective > objective){
			objective = nextObjective;
			delete [] CUT;
			CUT = nextCut;
			step = 0;
			cerr << objective << endl;
			continue;
		}

	}
	}
	catch(bad_alloc& ba){
		cerr << "exception: " << ba.what() << endl;
	}

	delete c;

	delete [] nextCut;

}

void graph::maxCut2(){

	int N = GenomeMatrix->size();
	int S = GenomeMatrix->at(0).size();


	int * cut = new int[V];
	int * nextCut = new int[V];
	int * bestCut = new int[V];
	int current, next, best = 0;

	int LIMIT = 100000;

	for (int i = 0; i < V; i++){

		cut[i] = 0;
		nextCut[i] = 0;
		bestCut[i] = 0;
	}

	int step = 0;
	time_t startTime = time(NULL);

	int isnp = 0;
	while(isnp < S){

		step++;

		vector<int> group1;
		vector<int> group2;

		for (int i = 0; i < N; i++){

			int v;

			switch(GenomeMatrix->at(i)[isnp]){
			case '2':
				v = findIBD(i, isnp);
				if (v != -1){
					group1.push_back(v);
				}
				break;
			case '0':
				v = findIBD(i, isnp);
				if (v != -1){
					group2.push_back(v);
				}
				break;
			}
		}

		//TODO: debug purpose
		//cout << "here!" << endl;

		next = current;
		for (int i = 0; i < V; i++){

			nextCut[i] = cut[i];
		}

		//group1 -> 0, group2 -> 1
		for (int i = 0; i < group1.size(); i++){

			int u = group1[i];
			if (cut[u] == 0){
				continue;
			}
			else{
				nextCut[u] = 0;
				for (map<int, int>::iterator it = AdjList[u].begin(); it != AdjList[u].end(); it++){
					switch(cut[it->first]){
					case 0:
						next -= it->second;
						break;
					case 1:
						next += it->second;
						break;
					}
				}
			}
		}
		//group2 -> 1
		for (int i = 0; i < group2.size(); i++){

			int u = group2[i];
			if (cut[u] == 1){
				continue;
			}
			else{
				nextCut[u] = 1;
				for (map<int, int>::iterator it = AdjList[u].begin(); it != AdjList[u].end(); it++){
					switch(cut[it->first]){
					case 0:
						next += it->second;
						break;
					case 1:
						next -= it->second;
						break;
					}
				}
			}
		}
		//compare next and current
		if (next > current){
			//step = 0;
			current = next;
			for (int i = 0; i < V; i++){

				cut[i] = nextCut[i];
			}
			if (current > best){
				best = current;
				for (int i = 0; i < V; i++){
					bestCut[i] = cut[i];
				}
			}

			//cerr << next << " in " << step << " steps" << endl;
			step = 0;
		}


		//
		//
		//group1 -> 1 and group2 -> 0

		next = current;
		for (int i = 0; i < V; i++){

			nextCut[i] = cut[i];
		}

		//group1 -> 1, group2 -> 0
		for (int i = 0; i < group1.size(); i++){

			int u = group1[i];
			if (cut[u] == 1){
				continue;
			}
			else{
				nextCut[u] = 1;
				for (map<int, int>::iterator it = AdjList[u].begin(); it != AdjList[u].end(); it++){
					switch(cut[it->first]){
					case 0:
						next += it->second;
						break;
					case 1:
						next -= it->second;
						break;
					}
				}
			}
		}
		//group2 -> 1
		for (int i = 0; i < group2.size(); i++){

			int u = group2[i];
			if (cut[u] == 0){
				continue;
			}
			else{
				nextCut[u] = 0;
				for (map<int, int>::iterator it = AdjList[u].begin(); it != AdjList[u].end(); it++){
					switch(cut[it->first]){
					case 0:
						next -= it->second;
						break;
					case 1:
						next += it->second;
						break;
					}
				}
			}
		}
		//compare next and current
		if (next > current){
			//step = 0;
			current = next;
			for (int i = 0; i < V; i++){

				cut[i] = nextCut[i];
			}
			if (current > best){
				best = current;
				for (int i = 0; i < V; i++){
					bestCut[i] = cut[i];
				}
			}

			//cerr << next << " in " << step << " steps" << endl;

			step = 0;
		}

		isnp++;

	}


	while(step < LIMIT && (time(NULL) - startTime) < 100){

		step++;

		int snp = (rand()*32768 + rand())%S;
		if (snp < 0){
			snp *= -1;
		}

		vector<int> group1;
		vector<int> group2;

		for (int i = 0; i < N; i++){

			int v;

			switch(GenomeMatrix->at(i)[snp]){
			case '2':
				v = findIBD(i, snp);
				if (v != -1){
					group1.push_back(v);
				}
				break;
			case '0':
				v = findIBD(i, snp);
				if (v != -1){
					group2.push_back(v);
				}
				break;
			}
		}

		//TODO: debug purpose
		//cout << "here!" << endl;

		next = current;
		for (int i = 0; i < V; i++){

			nextCut[i] = cut[i];
		}

		//group1 -> 0, group2 -> 1
		for (int i = 0; i < group1.size(); i++){

			int u = group1[i];
			if (cut[u] == 0){
				continue;
			}
			else{
				nextCut[u] = 0;
				for (map<int, int>::iterator it = AdjList[u].begin(); it != AdjList[u].end(); it++){
					switch(cut[it->first]){
					case 0:
						next -= it->second;
						break;
					case 1:
						next += it->second;
						break;
					}
				}
			}
		}
		//group2 -> 1
		for (int i = 0; i < group2.size(); i++){

			int u = group2[i];
			if (cut[u] == 1){
				continue;
			}
			else{
				nextCut[u] = 1;
				for (map<int, int>::iterator it = AdjList[u].begin(); it != AdjList[u].end(); it++){
					switch(cut[it->first]){
					case 0:
						next += it->second;
						break;
					case 1:
						next -= it->second;
						break;
					}
				}
			}
		}
		//compare next and current
		if (next > current){
			//step = 0;
			current = next;
			for (int i = 0; i < V; i++){

				cut[i] = nextCut[i];
			}
			if (current > best){
				best = current;
				for (int i = 0; i < V; i++){
					bestCut[i] = cut[i];
				}
			}

			//cerr << next << " in " << step << " steps" << endl;
			step = 0;
		}


		//
		//
		//group1 -> 1 and group2 -> 0

		next = current;
		for (int i = 0; i < V; i++){

			nextCut[i] = cut[i];
		}

		//group1 -> 1, group2 -> 0
		for (int i = 0; i < group1.size(); i++){

			int u = group1[i];
			if (cut[u] == 1){
				continue;
			}
			else{
				nextCut[u] = 1;
				for (map<int, int>::iterator it = AdjList[u].begin(); it != AdjList[u].end(); it++){
					switch(cut[it->first]){
					case 0:
						next += it->second;
						break;
					case 1:
						next -= it->second;
						break;
					}
				}
			}
		}
		//group2 -> 1
		for (int i = 0; i < group2.size(); i++){

			int u = group2[i];
			if (cut[u] == 0){
				continue;
			}
			else{
				nextCut[u] = 0;
				for (map<int, int>::iterator it = AdjList[u].begin(); it != AdjList[u].end(); it++){
					switch(cut[it->first]){
					case 0:
						next -= it->second;
						break;
					case 1:
						next += it->second;
						break;
					}
				}
			}
		}
		//compare next and current
		if (next > current){
			//step = 0;
			current = next;
			for (int i = 0; i < V; i++){

				cut[i] = nextCut[i];
			}
			if (current > best){
				best = current;
				for (int i = 0; i < V; i++){
					bestCut[i] = cut[i];
				}
			}

			//cerr << next << " in " << step << " steps" << endl;

			step = 0;
		}

	}

	for (int i = 0; i < V; i++){

		CUT[i] = cut[i];
	}

	delete [] cut;
	delete [] nextCut;
	delete [] bestCut;
}


graph::~graph(){

	delete [] offset;
	for (int i = 0; i < V; i++){

		AdjList[i].clear();
	}
	delete [] AdjList;
	haplotype->clear();
}

int graph::getSize(){

	return this->V;
}

int graph::findIBD(int ind2, int snp){
	int label = -1;

	int start = 0;
	int end = IBDmatrix[individual][ind2].size();

	if (end == 0){
		return label;
	}

	while(start < end){

		int middle = (start + end)/2;

		int s = IBDmatrix[individual][ind2][middle].startSNP;
		int e = IBDmatrix[individual][ind2][middle].endSNP;

		if ( snp >= s && snp < e){
			return offset[ind2] + middle;
		}
		else if (snp < s){

			end = middle;
		}
		else{
			start = middle + 1;
		}
	}

	return label;
}

void graph::phase(){

	this->maxCut2();

	int N = GenomeMatrix->size();
	int S = GenomeMatrix->at(individual).size();

	int * currentSharedIBD = new int [N];
	for (int i = 0; i < N; i++){
		currentSharedIBD[i] = offset[i];
	}

	for (int snp = 0; snp < S; snp++){

		//vector<int> maternalCutVotes;
		//vector<int> paternalCutVotes;
		int maternalVotes[2];
		int paternalVotes[2];
		maternalVotes[0] = 0;
		maternalVotes[1] = 0;
		paternalVotes[0] = 0;
		paternalVotes[1] = 0;

		switch(GenomeMatrix->at(individual)[snp]){
		case '0':
			haplotype->push_back(0);
			break;
		case '2':
			haplotype->push_back(3);
			break;
		case '1':
			for (int ind = 0; ind < N; ind++){

				int u = findIBD(ind, snp);
				if (u == -1){
					continue;
				}

				int allele;

				switch(GenomeMatrix->at(ind)[snp]){
				case '2':
					allele = 1;
					break;
				case '0':
					allele = 0;
					break;
				case '1':
					allele = -1;
					break;
				}

				if (allele == -1){
					continue;
				}

				switch(CUT[u]){
				//maternal
				case 0:
					maternalVotes[allele]++;
					break;
				//paternal
				case 1:
					paternalVotes[allele]++;
					break;
				}

			}

			if (maternalVotes[0] > maternalVotes[1] && paternalVotes[1] > paternalVotes[0]){
				haplotype->push_back(1);
			}
			else if(maternalVotes[1] > maternalVotes[0] && paternalVotes[0] > paternalVotes[1]){
				haplotype->push_back(2);
			}
			else{
				haplotype->push_back(4);
			}

			break;
		default:
			haplotype->push_back(4);
			break;
		}
	}
}

map<int, int> * graph::getAdjList(){

	return this->AdjList;
}

vector<uint8_t> * graph::getHaplotype(){

	return haplotype;
}
