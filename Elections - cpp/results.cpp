#include   <iostream>
#include "results.h"
#include "UnionResults.h"

using namespace std;
namespace Elections
{
	bool results::setParty(party* p)
	{
		partyRes* r = new partyRes;
		r->party = p;
		r->repsList = new list<citizen*>();

		DisResults.insert(DisResults.end(), *r);

		return true;
	}

	void results::addVoter(int partyNum)
	{
		if (partyNum >= DisResults.size())
			throw invalid_argument("No such party");

		DisResults[partyNum].numberOfVotes++;
		votersNumber++;
	}

	bool results::setPercentage() //set voters percentages for any party in the district
	{
		for (auto itr = DisResults.begin(); itr != DisResults.end(); ++itr)
		{
			(*itr).preVoters = (static_cast<float>((*itr).numberOfVotes / static_cast<float>(votersNumber)) * 100);
		}
		return true;
	}

	float results::getPercentage() const //total percentage for the district
	{
		float res = (static_cast<float>(votersNumber) / static_cast<float>(citizensNumber)) * 100;
		return res;
	}

	int results::getRepsNum(int partyNum)
	{
		return (DisResults[partyNum].numberOfReps);
	}
	void results::setNumReps(int repsNum)
	{
		int i;
		for (auto itr = DisResults.begin(); itr != DisResults.end() ; ++itr)
		{
			float TempNumOfReps = (((*itr).preVoters) * repsNum) / 100;
			int numOfReps = static_cast<int>(TempNumOfReps);
			if ((TempNumOfReps - numOfReps) >= 0.5)
				numOfReps += 1; //if the number after point is big - add 1 to repsNum

			(*itr).numberOfReps = numOfReps;
		}
	}

	void results::addRepToList(int partyNum, citizen* rep)
	{
		DisResults[partyNum].repsList->push_back(rep);
	}

	ostream& operator<<(ostream& os, const results& res)
	{
		os << "precentage of voters: " << res.getPercentage() << "%" << endl << endl;
		res.toOs(os);

		for (int i = 0; i < res.getDisResults().size(); ++i)
		{
			os << "party number : " << i << endl << endl
				<< "number of voters: " << res.DisResults[i].numberOfVotes << endl
				<< "precentage of voters: " << res.DisResults[i].preVoters << "%" << endl
				<< "Representatives list:" << endl;
			printList(*(res.DisResults[i].repsList));
		}

		return os;
	}

	void results::Sort(int size)
	{
		for (int i = 0; i < size - 1; i++)
		{
			for (int j = 0; j < size - i - 1; i++)
			{
				if (DisResults[i].numberOfReps < DisResults[i + 1].numberOfReps)
				{
					Swap(DisResults[i], DisResults[i + 1]);
				}
			}
		}
	}

	void results::toOs(ostream& os) const
	{
		os << "Candidates by electors: " << endl;
		for (int i = 0; i < DisResults.size(); ++i)
		{
			os << "Candidate: " << DisResults[i].party->getPrimeCandidate() << endl
				<< "Number of Representatives: " << DisResults[i].numberOfReps << endl << endl;
		}
	}

	void results::save(ofstream& file) const
	{
		file.write(rcastcc(&votersNumber), sizeof(int)); //number of voters in the district
		int size = DisResults.size();
		file.write(rcastcc(&size), sizeof(int)); //number of parties 
		for (int i = 0; i < size; ++i) //for any party
		{
			file.write(rcastcc(&(DisResults[i].numberOfVotes)), sizeof(int)); //number of citizens votes to this party
		}
	}

	void results::load(ifstream& file)
	{
		int partiesSize, serial;
		file.read(rcastc(&votersNumber), sizeof(int));	//number of voters in the district

		file.read(rcastc(&partiesSize), sizeof(int)); //number of parties 
		for (int i = 0; i < partiesSize; i++) //for any party
		{
			file.read(rcastc(&(DisResults[i].numberOfVotes)), sizeof(int));
		}
	}
}