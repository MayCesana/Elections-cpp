#pragma once
#include <iostream>

using namespace std;

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

namespace Elections
{
	class district;
	class citizen
	{
	private:
		string name;
		int ID;
		int birthYear;
		district& dis;
		bool voted;
		bool isRep;
	public:
		citizen(const string& newName, int id, int birthYear, int electionsYear, const district& dis);
		citizen(const citizen& person);
		~citizen() {}
		
		//getters
		const string& getName() const { return name; }
		int getID() const { return ID; }
		int getDistrictNum() const;
		bool isVoted()const { return voted; }
		bool citizenIsRep()const { return isRep; }
		const district& getDis() { return dis; }

		//setters
		void setVote() { voted = true; }
		void setRep() { isRep = true; }

		void operator=(const citizen& other);
		friend ostream& operator<<(ostream& os, const citizen& c);

		//file
		void save(ofstream& file) const;
	};
}

