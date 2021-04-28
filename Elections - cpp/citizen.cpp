#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "citizen.h"
#include "district.h"

namespace Elections
{
	citizen::citizen(const string& newName, int id, int birthYear, int electionsYear, const district& dis) : name(newName), ID(id), birthYear(birthYear),
		dis(const_cast<district&>(dis)), voted(false), isRep(false)
	{
		if ((electionsYear -birthYear < 18)) // if person is not 18+ 
			throw  invalid_argument("Person is under 18.");
		if ((id / 100000000) == 0 || (id / 100000000) > 9)
			throw  invalid_argument("Person's ID is not 9 figures.");
	}

	citizen::citizen(const citizen& person) : name(person.name), ID(person.ID), birthYear(person.birthYear),
		dis(const_cast<district&>(dis)), voted(person.voted) {}

	int citizen::getDistrictNum() const
	{
		return (dis.getSerial());
	}

	ostream& operator<<(ostream& os, const citizen& person)
	{
		os << person.getName() << ", ID: " << person.ID << ", year of birth: " << person.birthYear << ", district number: "
			<< person.getDistrictNum();
		return os;
	}

	void citizen::operator=(const citizen& other)
	{
		ID = other.ID;
		name = other.name;
		birthYear = other.birthYear;
		dis = other.dis;
		voted = other.voted;
		isRep = other.isRep;
	}

	void citizen::save(ofstream& file) const
	{
		int nameSize = name.length();
		file.write(rcastcc(&nameSize), sizeof(int)); //number of bytes for name
		file.write(rcastcc(name.data()), nameSize);	//citizen's name
		file.write(rcastcc(&birthYear), sizeof(int));
		file.write(rcastcc(&ID), sizeof(int));

		int districtNum = dis.getSerial();
		file.write(rcastcc(&districtNum), sizeof(int));

		int isVote = static_cast<int>(voted);
		int is_Rep = static_cast<int>(isRep);
		file.write(rcastcc(&isVote), sizeof(int));
		file.write(rcastcc(&is_Rep), sizeof(int));
	}
}