#pragma once
#include <iostream>
#include "party.h"
#include "functions.h"

using namespace std;
namespace Elections
{
	class party;

	class results //results for any diveded district. 
	{
	protected:
		int citizensNumber;
		int votersNumber;

		struct partyRes
		{
			party* party = nullptr; //pointer to party
			int numberOfVotes = 0; //vote to this party
			float preVoters; //percentage of citizens vote to this party
			int numberOfReps; //after percentages's calculation
			list<citizen*>* repsList = nullptr; //after percentages's calculation
		};

		DynamicArray<partyRes> DisResults; //array of partyRes

	public:
		results() : citizensNumber(0), votersNumber(0) {}
		results(const results& r) = delete;
		virtual ~results() {}

		//getters
		int getRepsNum(int partyNum);
		float getPercentage() const; //returns the district's voters percentages 
		const DynamicArray<partyRes>& getDisResults() const { return DisResults; }

		//setters
		bool updateCitizensNum() { citizensNumber++; return true; }
		bool updateVotersNum() { votersNumber++; return true; }
		bool setParty(party* p); //when the user adds party - adds this party to DisResults array
		void addVoter(int partyNum);
		bool setPercentage(); //Calculats the percentages for any party in the district
		void setNumReps(int repsNum);
		void addRepToList(int partyNum, citizen* rep);

		friend ostream& operator<<(ostream& os, const results& res);
		virtual void toOs(ostream& os) const;
		void Sort(int size); //sorts the district results (if the district is diveded)

		//file
		void save(ofstream& out_file) const;
		void load(ifstream& out_file);
	};
}


