#pragma once
#include "district.h"
#include "party.h"
#include "Elections.h"
#include "final.h"

namespace Elections
{
	class Normal_Elections : public elections
	{
	private:
		DynamicArray<Final> finalResults; //array of results. any cell is a party

	public:
		Normal_Elections(int d, int m, int y);
		Normal_Elections(const Normal_Elections& e) = delete;
		~Normal_Elections() {}

		//getter
		const DynamicArray<Final>& getFinalArr() const { return finalResults; }

		bool addDistrict(const string& Name, int repnum, int type);
		virtual void addCitizenToList(const string& newName, int id, int birthYear, int DisNum = 0) override;
		virtual const list<citizen*>& Citizens() const { return citizens; }

		void setWinToDistricts(); //set party win only to uniun districts
		void makeFinalResults(); //Calculats the final results
		void showFinalResult();
		void SortFinalResults();

		//file
		void save(ofstream& file) const;
		void readDistricts(ifstream& file); //read any district from file
	};
}


