#include "UnionResults.h"
namespace Elections
{
	bool Union_Results::setWinParty()
	{
		int max = DisResults[0].numberOfReps;
		party* win = DisResults[0].party;
		for (auto iter = DisResults.begin(); iter != DisResults.end(); ++iter)
		{
			if (max < (*iter).numberOfReps)
			{
				max = (*iter).numberOfReps;
				win = (*iter).party;
			}
		}
		winParty = win;
		return true;
	}

	void Union_Results::toOs(ostream& os) const
	{
		os << "The winner Candidate in the district: " << getWinParty().getPrimeCandidate() << endl;
	}
}