
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;


class genome_matrix
{

private:
	vector<vector<char> > Matrix;

	int Individuals;
	int Sites;
public:
	genome_matrix();
	~genome_matrix();

	vector<vector<char> > Information;

	void initialize(int individuals, int sites);
	
	void testprint();

	void print();

	void set(int individual, int site, char order);
	char get(int individual, int site);

	int individuals();
	int sites();

};

genome_matrix::genome_matrix()
{

}

genome_matrix::~genome_matrix()
{

}

void genome_matrix::set(int individual, int site, char order)
{

	this->Matrix[individual][site] = order;
}

char genome_matrix::get(int individual, int site)
{

	return this->Matrix[individual][site];
}

void genome_matrix::initialize(int individuals, int sites)
{

	this->Individuals = individuals;
	this->Sites = sites;

	for (int i = 0; i < individuals + 1; i++)
	{

		vector<char> row;

		for (int j = 0; j < sites; j++)
		{

			row.push_back('0');			
		}

		this->Matrix.push_back(row);
	}

	for (int i = 0; i < individuals + 1; i++)
	{

		vector<char> info;
		this->Information.push_back(info);
	}

}

void genome_matrix::testprint()
{

	for (int i = 0; i < 50; i++)
	{

		for (int j = 0; j < 70; j++)

		{

			cerr << this->Matrix[i][j] << ' ';
		}

		cerr << endl;
	}
}

void genome_matrix::print()
{

	for (int i = 1; i < this->Matrix.size(); i++)
	{

		for (int j = 0; j < this->Information[i].size(); j++)
		{

			cout << Information[i][j];
		}

		cout << ' ';

//		for (int j = 0; j < 70; j++)
		for (int j = 0; j < this->Matrix[i].size(); j++)
		
		{

			switch (this->Matrix[i][j])
			{

				case '0':
					cout << '1' << ' ' << '1' << ' ';
					break;

				case '1':
					cout << '1' << ' ' << '2' << ' ';
					break;

				case '2':
					cout << '2' << ' ' << '1' << ' ';
					break;

				case '3':
					cout << '2' << ' ' << '2' << ' ';
					break;
				default:
					cout << '1' << ' ' << '2' << ' ';
					break;
			}
		}

		cout << endl;
	}

}

int genome_matrix::individuals()
{
	return this->Individuals;
}

int genome_matrix::sites()
{

	return this->Sites;
}
	
