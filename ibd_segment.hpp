
#ifndef IBD_H
#define IBD_H

class ibd_seg
{
private:
	unsigned long int Start;
	unsigned long int End;
	
public:
	ibd_seg(int start, int end);
	~ibd_seg();

	bool operator<(const ibd_seg&) const;
	bool operator<=(const ibd_seg&) const;
	bool operator>(const ibd_seg&) const;
	bool operator>=(const ibd_seg&) const;

	int start();
	int end();
};


class ibd_segment : public ibd_seg
{

private:
	unsigned short int * Individuals;
	unsigned short int Individual;
	//unsigned long int Start;
	//unsigned long int End;
	bool SingleIndividual;
public:
	ibd_segment(int individualOne, int individualTwo, int segmentStart, int segmentEnd);
	ibd_segment(int individual, int segment_start, int segment_end);
	~ibd_segment();

	//bool operator<(const ibd_segment&) const;
	//bool operator<=(const ibd_segment&) const;
	//bool operator>(const ibd_segment&) const;
	//bool operator>=(const ibd_segment&) const;

	int individual();
	//int start();
	//int end();
	int individual_one();
	int individual_two();	
	bool single_individual();

};

#endif
