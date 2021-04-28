#include <iostream>
#include "SimpleElections.h"

namespace Elections
{
	Simple_Elections::Simple_Elections(int d, int m, int y, int repsNum) : elections(d, m, y)
	{
		if (repsNum < 0)
		{
			throw invalid_argument("invalid representitives number.");
		}
		district* dis = new district(" ", 0, DIVIDED, repsNum);
		districts.insert(districts.end(), dis);
		const list<citizen*>& list = this->Citizens();
		districts[0]->citizensDis = list;
	}

	void Simple_Elections::addCitizenToList(const string& newName, int id, int birthYear, int DisNum)
	{
		citizen* c1 = searchByID(id);
		if (c1 != nullptr)
			throw invalid_argument("Citizen is already exist! ");

		c1 = new citizen(newName, id, birthYear, Date.getYear(), *(districts[0]));

		citizens.push_back(c1); //add the new citizen to list
		districts[0]->getRes()->updateCitizensNum(); //add citizen to number of citiznes in result
	}
}
