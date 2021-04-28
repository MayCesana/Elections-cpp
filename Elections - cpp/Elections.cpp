#include "Elections.h"
#include "NormalElections.h"
#include "SimpleElections.h"
#include <typeinfo>

namespace Elections
{
	void elections::add_Party(const string& name, citizen* prime)
	{
		if (prime == nullptr)
			throw invalid_argument("There is no such citizen ");

		party* p = new party(name, *prime, parties.size(), districts.size());
		parties.insert(parties.end(), p);

		for (int i = 0; i < districts.size(); ++i)
		{
			p->updateNumOfReps(i, districts[i]->getRepNum());
			districts[i]->getRes()->setParty(p);
		}
	}

	bool party::updateNumOfReps(int disNum, int repsNum)
	{
		(*reps)[disNum].numberOfReps = repsNum;
		return true;
	}

	void elections::printParties()
	{
		if (parties.size() <= 0)
			throw out_of_range("There are no parties to show..");
		for (auto itr = parties.begin(); itr != parties.end(); ++itr)
		{
			cout << *(*itr) << endl;
			try {
				(*itr)->printReps();
			}
			catch (std::exception& ex) {
				cout << ex.what() << endl;
			}
			cout << endl;
		}
	}

	void elections::printDistricts()
	{
		if (districts.size() <= 0)
			throw out_of_range("There are no districts to show");
		districts.print();
	}

	void elections::addRep(int ID, int partyNum, int districtNum)
	{
		citizen* newRep = searchByID(ID);
		if (newRep == nullptr)
			throw invalid_argument("There is no such citizen ");
		else if (newRep->citizenIsRep())
		{
			throw invalid_argument ("This citizen is already a representative");
		}
		
		parties[partyNum]->getReps()[districtNum].repList->push_back(newRep);
		parties[partyNum]->getReps()[districtNum].currReps++;
		newRep->setRep();
	}

	bool elections::vote(citizen& voter, int partyNum)
	{
		voter.setVote();
		int disNum = voter.getDistrictNum();
		districts[disNum]->getRes()->addVoter(partyNum); //add vote to results array

		return true;
	}

	void elections::preResults()
	{
		if (parties.empty())
			throw invalid_argument("There are no parties to vote for!");

		//check if the electors lists are full
		for (int p = 0; p < parties.size(); ++p)
		{
			for (int d = 0; d < parties[p]->getRepsSize(); ++d)
			{
				if (!(parties[p]->enoughReps(d)))
				{
					throw invalid_argument("Electors List for party " + std::to_string(p) + " to district " + std::to_string(d) + " is not full.");
				}
			}
		}

		//else - we can calculates the results
		setPre();
		setRepsToParties();
		if (typeid(*this) == typeid(Normal_Elections))
			dynamic_cast<Normal_Elections*>(this)->setWinToDistricts();
	}

	void elections::setPre()
	{
		for (auto itr = districts.begin(); itr != districts.end(); ++itr)
		{
			(*itr)->getRes()->setPercentage();
		}
	}

	void elections::setRepsToParties()
	{
		for (auto itr = districts.begin(); itr != districts.end(); itr++)
		{
			(*itr)->getRes()->setNumReps((*itr)->getRepNum()); //set the repsNum to res in order to calculates the number of reps after votes
		}
		for (int p = 0; p < parties.size(); ++p)
		{
			for (int d = 0; d < districts.size(); ++d)
			{
				int repsNum = districts[d]->getRes()->getRepsNum(p);
				districts[d]->getRes()->getDisResults()[p].repsList->clear();
				list<citizen*>& lst = *(parties[p]->getReps()[d].repList);
				for (auto listItr =lst.begin() ; listItr != lst.end(); ++listItr)
				{
					citizen& rep = *(*listItr);
					districts[d]->getRes()->addRepToList(p, &rep);
				}
			}
		}
	}

	void elections::showResults()
	{
		cout << "RESULTS FOR " << Date << "ELECTIONS:" << endl << endl;
		if (typeid(*this) == typeid(Normal_Elections))
		{
			cout << "DISTRICTS RESULTS:" << endl << endl;
			for (int i = 0; i < districts.size(); ++i)
			{
				cout << districts[i]->getName();
				if (districts[i]->getType() == UNION)
					cout << " (Union) ";

				else
				{
					districts[i]->getRes()->Sort(parties.size());
					cout << " (Diveded) ";
				}
				cout << "district results: " << endl;
				cout << *(districts[i]->getRes());
			}
		}
		else
		{
			cout << endl << "FINAL RESULTS:   " << endl << endl;
			districts[0]->getRes()->Sort(parties.size());
			cout << *(districts[0]->getRes());
		}
	}

	void elections::readparty(ifstream& file)
	{
		int nameSize, srl, repsize;
		file.read(rcastc(&nameSize), sizeof(int)); //number of bytes for name
		char* name = new char[nameSize + 1];
		file.read(name, nameSize);	//party name
		name[nameSize] = '\0';
		file.read(rcastc(&srl), sizeof(int)); //party serial number 

		int primeID;
		file.read(rcastc(&primeID), sizeof(int));
		citizen* PrimeCandidate = searchByID(primeID);
		file.read(rcastc(&repsize), sizeof(int)); //districts number (to array)


		add_Party(name, PrimeCandidate);

		for (int i = 0; i < repsize; i++) //for any district
		{
			file.read(rcastc(&(parties[srl]->getReps()[i].numberOfReps)), sizeof(int));
			parties[srl]->getReps()[i].currReps = parties[srl]->getReps()[i].numberOfReps;

			for (int j = 0; j < parties[srl]->getReps()[i].numberOfReps; j++)
			{
				int repID;
				file.read(rcastc(&repID), sizeof(int));
				citizen* p = searchByID(repID);
				parties[srl]->getReps()[i].repList->push_back(p);
			}
		}
	}

	void elections::readCitizen(ifstream& file)
	{
		int namelen, brth, id, disnum;
		char* name;
		file.read(rcastc(&namelen), sizeof(int));
		name = new char[namelen + 1];
		file.read(name, namelen);
		name[namelen] = '\0';
		file.read(rcastc(&brth), sizeof(int));
		file.read(rcastc(&id), sizeof(int));
		file.read(rcastc(&disnum), sizeof(int));

		addCitizenToList(name, id, brth, disnum);
		citizen* p = searchByID(id);

		int isVote;
		int is_Rep;
		file.read(rcastc(&isVote), sizeof(int));
		file.read(rcastc(&is_Rep), sizeof(int));

		if (isVote)
			p->setVote();
		if (is_Rep)
			p->setRep();
	}

	void elections::readElections(ifstream& file)
	{
		int districtType, TotalPar, TotalCitizens, number_of_districts;

		if (typeid(*this) == typeid(Normal_Elections))
		{
			dynamic_cast<Normal_Elections*>(this)->readDistricts(file);
		}

		file.read(rcastc(&TotalCitizens), sizeof(int));
		if (!file.eof())
		{
			for (int i = 0; i < TotalCitizens; i++) //load citizens
				readCitizen(file);
		}

		file.read(rcastc(&TotalPar), sizeof(int));
		if (!file.eof())
		{
			for (int i = 0; i < TotalPar; i++) //load parties
				readparty(file);
		}

		if (!file.eof())
		{
			file.read(rcastc(&number_of_districts), sizeof(int));
			for (int i = 0; i < number_of_districts; i++)
			{
				districts[i]->getRes()->load(file); //load votes per district
			}
		}
	}

	citizen* elections::searchByID(int ID)
	{
		for (auto itr = citizens.begin(); itr != citizens.end(); ++itr)
		{
			if ((*itr)->getID() == ID)
				return (*itr);
		}

		return nullptr;
	}
}
