#pragma once
#include <list>
#include <iostream>
#include <fstream>
#include "DynamicArray.h"
#include "citizen.h"
#include "functions.h"

using namespace std;
namespace Elections
{
	class party
	{
	private:
		friend class elections;
		struct repToDistrict
		{
			int currReps = 0;
			int numberOfReps;
			list<citizen*>* repList; // list of the party representatives to this district
		};

		int serial;
		string partyName;
		const citizen& PrimeCandidate; //pointer to citizen candidates to be the Prime Minister
		DynamicArray<repToDistrict>* reps; // pointer to array of the party representatives, any cell is a different district

	public:
		party(const string& name, const citizen& prime, int serial, int numOfDistricts);
		party(const party& p) = delete;
		~party() {}

		//getters
		const string& getPartyName() const { return partyName; }
		const int getSerialNum() const { return serial; }
		const citizen& getPrimeCandidate() const { return PrimeCandidate; }
		DynamicArray<repToDistrict>& getReps() { return *reps; }
		int getRepsSize() const { return (*reps).size(); }

		bool updateNumOfReps(int disNum, int repsNum); //when we add a new party
		bool updateDistrictToParty(int disNum, int repsNum); //when we add a new district
		void insertDistrictToPartyRes(int repsNum); 
		bool enoughReps(int index) const;
		void printReps();
		friend ostream& operator<<(ostream& os, const party& p);
		bool operator==(const party& other)const;

		//file
		void save(ofstream& file) const;
	};
}


