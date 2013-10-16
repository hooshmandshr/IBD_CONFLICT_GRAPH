
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

ibd_seg::ibd_seg(int start, int end)
{
	this->Start = start;
	this->End = end;
}

ibd_seg::~ibd_seg()
{

}

int ibd_seg::start()
{
	return this->Start;
}

int ibd_seg::end()
{
	return this->End;
}


bool ibd_seg::operator<(const ibd_seg& Operand) const
{
	//TODO: change this	
	//if (this->end < Operand.start)
	if (this->End < Operand.End)
	{
		return true;
	}
	else
	{
		return false;
	}	
}


bool ibd_seg::operator<=(const ibd_seg& Operand) const
{
	if (this->Start <= Operand.Start)
	{
		return true;	
	}
	else
	{
		return false;
	}
}


bool ibd_seg::operator>(const ibd_seg& Operand) const
{
	if (this->Start > Operand.End)
	{
		return true;
	}
	else
	{
		return false;
	}	
}


bool ibd_seg::operator>=(const ibd_seg& Operand) const
{
	if (this->Start >= Operand.Start)
	{
		return true;
	}	
	else
	{
		return false;
	}

}

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
	ibd_segment(int individual, int segmentStart, int segmentEnd);
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


ibd_segment::ibd_segment(int individualOne, int individualTwo, int segmentStart, int segmentEnd) : ibd_seg(segmentStart, segmentEnd)
{
	//this->ibd_seg(segmentStart, segmentEnd);
	this->Individuals = new unsigned short [2];
	this->Individuals[0] = individualOne;
	this->Individuals[1] = individualTwo;
	//this->Start = segmentStart;
	//this->End = segmentEnd;
	this->SingleIndividual = false;	
}

ibd_segment::ibd_segment(int individual, int segmentStart, int segmentEnd) : ibd_seg(segmentStart, segmentEnd)
{
	//this->ibd_seg(segmentStart, segmentEnd);
	this->Individual = individual;
	//this->Start = segmentStart;
	//this->End = segmentEnd;
	this->SingleIndividual = true;
}

ibd_segment::~ibd_segment()
{

	if (!(this->SingleIndividual))
	{
		delete [] Individuals;
	}
}

int ibd_segment::individual()
{
	if (this->SingleIndividual)
	{	
		return this->Individual;
	}
	else
	{
		return this->Individuals[0];
	}

}

int ibd_segment::individual_one()
{
	if (this->SingleIndividual)
	{
		return this->Individual;
	}
	else
	{
		return this->Individuals[0];
	}
}

int ibd_segment::individual_two()
{
	if (this->SingleIndividual)
	{
		return this->Individual;
	}
	else
	{
		return this->Individuals[1];
	}
}

bool ibd_segment::single_individual()
{

	return this->SingleIndividual; 
}

