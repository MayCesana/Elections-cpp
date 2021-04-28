#pragma once
#include<iostream>
#include"Elections.h"

namespace Elections
{
	class Final //keep the results by parties
	{
	private:
		party* par = nullptr;
		int numOfElects = 0;
		int totalVotes = 0;
	public:
		Final() : par(nullptr) {}
		~Final() = default;

		int getNumOfElects() { return numOfElects; }
		int getTotalVotes() { return totalVotes; }
		void setParty(party* p) { par = p; }
		party* getParty() const { return par; }

		void Sort(int size); //sorts the parties by number of electors - high to low 
		friend ostream& operator<<(ostream& os, const Final& c);
		void operator=(const Final& other);
	};
}


