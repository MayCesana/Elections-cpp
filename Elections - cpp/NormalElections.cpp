#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "NormalElections.h"
#include <string.h>
#include <typeinfo>
#include "UnionResults.h"
#include "functions.h"

namespace Elections
{
	Normal_Elections::Normal_Elections(int d, int m, int y) : elections(d, m, y) {}

	bool Normal_Elections::addDistrict(const string& Name, int repnum, int type)
	{
		int size = districts.size();
		district* d = new district(Name, size, type, repnum);
		districts.insert(districts.end(), d);
		
		for (int i = 0; i < parties.size(); ++i)
		{
			parties[i]->updateDistrictToParty(size, districts[size]->getRepNum());
		}
		return true;
	}

	void Normal_Elections::addCitizenToList(const string& newName, int id, int birthYear, int DisNum)
	{
		if (DisNum < 0 || DisNum > districts.size())
			throw invalid_argument("There is no such district");

		citizen* c1 = searchByID(id);
		if (c1 != nullptr)
			throw invalid_argument("citizen is already exist! ");

		district* dis = districts[DisNum];
		c1 = new citizen(newName, id, birthYear, Date.getYear(), *dis);
		
		citizens.push_back(c1);
		districts[DisNum]->citizensDis.push_back(c1);
		districts[DisNum]->getRes()->updateCitizensNum();
	}

	void Normal_Elections::setWinToDistricts()
	{
		for (auto itr = districts.begin(); itr != districts.end(); ++itr)
		{
			results* disRes = (*itr)->getRes();
			if ((*itr)->getType() == UNION)
			{
				Union_Results* disURes = dynamic_cast<Union_Results*>(disRes);
				disURes->setWinParty();
			}
		}
	}

	void Normal_Elections::makeFinalResults()
	{
		if (!finalResults.empty()) //if we need to make a new one
		{
			finalResults.clear();
		}

		int numOfPar = parties.size();
		
		for (int i = 0; i < numOfPar; i++)
		{
			Final* ref = new Final(); //pointer to some final
			ref->setParty(parties[i]); 
			finalResults.insert(finalResults.end(), *ref); //add this final to array

			for (auto itr=districts.begin(); itr != districts.end(); ++itr)
			{
				results* DisRes = (*itr)->getRes();
				int n = DisRes->getDisResults()[i].numberOfVotes;
				updateByOne(finalResults[i].getTotalVotes(), PlusOne());
				
				int numOfelects;
				if ((*itr)->getType() == DIVIDED)
				{
					numOfelects = DisRes->getDisResults()[i].numberOfReps;
				}
				else if (*(parties[i]) == dynamic_cast<Union_Results*>(DisRes)->getWinParty());
				{
					numOfelects = (*itr)->getRepNum();
				}
				updateByOne(finalResults[i].getNumOfElects(), PlusOne());
			}
		}

		SortFinalResults();
	}

	void Normal_Elections::SortFinalResults()
	{
		for (int i = 0; i < finalResults.size() - 1; i++)
		{
			for (int j = 0; j < finalResults.size() - i - 1; i++)
			{
				if (finalResults[j].getNumOfElects() < finalResults[j + 1].getNumOfElects())
				{
					Swap(finalResults[j], finalResults[j + 1]);
				}
			}
		}
	}

	void Normal_Elections::showFinalResult()
	{
		cout << endl << "FINAL RESULTS:   " << endl << endl;
		for (auto itr = finalResults.begin(); itr != finalResults.end(); ++itr)
		{
			cout << (*itr) << endl << endl;
		}
	}

	void Normal_Elections::save(ofstream& file) const
	{
		int disSize = districts.size();
		file.write(rcastcc(&disSize), sizeof(int)); //number of districts
		if (disSize > 0)
		{
			for (int i = 0; i < disSize; i++)
			{
				districts[i]->save(file);
			}
		}
	}

	void Normal_Elections::readDistricts(ifstream& file)//read any district from file
	{
		int disSize;
		file.read(rcastc(&disSize), sizeof(int)); //number of districts

		for (int i = 0; i < disSize; i++)
		{
			district* d = new district(file);
			districts.insert(districts.end(), d);
		}
			
	}
}

