#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "district.h"
#include "results.h"
#include "UnionResults.h"

using namespace std;
namespace Elections
{
	district::district(string Name, int serial, int type, int repnum) : name(Name), serial(serial), type(type),
		repNum(repnum)
	{
		if (serial < 0)
		{
			throw invalid_argument("invalid serial number.");
		}
		if (type != UNION && type != DIVIDED)
		{
			throw invalid_argument("invalid District type.");
		}
		if (repnum < 0)
			throw invalid_argument("invalid representitives number.");
		else if (type == UNION)
			res = new Union_Results();
		else
			res = new results();
	}

	district::district(const district& dis) : name(dis.name), serial(dis.serial), type(dis.type),
		repNum(dis.repNum)
	{
		delete res;
		res = dis.res;
	}

	district::district(ifstream& file)
	{
		int nameSize;
		file.read(rcastc(&nameSize), sizeof(int)); //number of bytes for name
		name.resize(nameSize);
		file.read((char*)(name.data()), nameSize);	//district name
		
		file.read(rcastc(&serial), sizeof(int)); //district serial number 
		file.read(rcastc(&type), sizeof(int)); //district type (1=union, 2=diveded)
		file.read(rcastc(&repNum), sizeof(int)); //reps number

		if (serial < 0)
		{
			throw invalid_argument("invalid serial number.");
		}
		if (type != UNION && type != DIVIDED)
		{
			throw invalid_argument("invalid District type.");
		}
		else if (type == UNION)
			res = new Union_Results();
		else
			res = new results();
	}

	district::~district()
	{
		delete res;
	}

	ostream& operator<<(ostream& os, const district& d)
	{
		os << "District's serial number: " << d.getSerial() << ", District's name: " << d.getName() <<
			"district type: ";
		if (d.getType() == UNION)
			os << "Union , ";
		else
			os << "Divided , ";
		os << "District's representative number:" << d.getRepNum() << endl;

		return os;
	}

	void district::operator=(const district& other)
	{
		name = other.name;
		serial = other.serial;
		type= other.type; 
		repNum = other.repNum;
		res = other.res; 
		Percentage = other.Percentage;
		citizensDis = other.citizensDis;
	}

	void district::save(ofstream& file)const
	{
		int nameSize = name.length();
		file.write(rcastcc(&nameSize), sizeof(int)); //number of bytes for name
		file.write(rcastcc(name.data()), nameSize);	//district name
		file.write(rcastcc(&serial), sizeof(int)); //district serial number 
		file.write(rcastcc(&type), sizeof(int)); //district type (1=union, 2=diveded)
		file.write(rcastcc(&repNum), sizeof(int)); //reps number
	}
}