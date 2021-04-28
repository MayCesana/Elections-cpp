#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include "party.h"

using namespace std;

namespace Elections
{
	party::party(const string& name, const citizen& prime, int serial, int numOfDistricts) : serial(serial),
		PrimeCandidate(prime), partyName(name)
	{
		if (numOfDistricts < 0)
			throw invalid_argument("invalid Rep Size.");

		reps = new DynamicArray<repToDistrict>(numOfDistricts); //creates dynamicArray 

		for (int i = 0; i < numOfDistricts; ++i)
		{
			insertDistrictToPartyRes(0);
		}
	}

	bool party::updateDistrictToParty(int disNum, int repsNum)
	{
		if ((*reps).size() <= disNum)
		{
			insertDistrictToPartyRes(repsNum);
		}
		return true;
	}

	void party::insertDistrictToPartyRes(int repsNum)
	{
		repToDistrict* rtd = new repToDistrict;
		rtd->numberOfReps = repsNum;
		rtd->repList = new list<citizen*>();
		(*reps).insert((*reps).end(), *rtd);
	}

	bool party::enoughReps(int index) const
	{
		if ((*reps)[index].numberOfReps == (*reps)[index].currReps)
			return true;
		else
			return false;
	}

	ostream& operator<<(ostream& os, const party& p)
	{
		os << "PARTY " << p.serial + 1 << endl << "Party's serial number: " << p.serial << ", Party's name: " << p.partyName << endl
			<< "Party's prime minister candidate: " << p.PrimeCandidate << endl;
		return os;
	}

	bool party::operator==(const party& other)const
	{
		if ((partyName == other.partyName) && (serial == other.serial))
			return true;
		else
			return false;
	}

	void party::printReps()
	{
		cout << "REPRESENTATIVES LIST:" << endl;
		for (int i = 0; i < (*reps).size(); ++i)
		{
			cout << "District number: " << i << endl;
			cout << "Representatives: " << endl;
			list<citizen*>& lst = *((*reps)[i].repList);
			if (lst.empty())
				throw invalid_argument("There are no citizens.");
			else
				printList(lst);
			cout << endl;
		}
		cout << endl;
	}

	void party::save(ofstream& file) const
	{
		int nameSize = partyName.length();
		file.write(rcastcc(&nameSize), sizeof(int));//number of bytes for name
		file.write(rcastcc(partyName.data()), nameSize);
		file.write(rcastcc(&serial), sizeof(int)); //party serial number 

		int primeID = PrimeCandidate.getID();
		file.write(rcastcc(&primeID), sizeof(int));

		int repsSize = reps->size();
		file.write(rcastcc(&repsSize), sizeof(int)); //districts number (to array)
		for (int i = 0; i < repsSize; ++i)
		{
			file.write(rcastcc(&((*reps)[i].numberOfReps)), sizeof(int));
			for (auto itr = (*reps)[i].repList->begin(); itr != (*reps)[i].repList->end(); ++itr)
			{
				int repID = (*(*itr)).getID();
				file.write(rcastcc(&repID), sizeof(int));
			}
		}
	}
}