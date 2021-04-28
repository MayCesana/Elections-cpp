#pragma once
#include "district.h"
#include "party.h"
#include "final.h"

namespace Elections
{
	class Simple_Elections : public elections
	{
	public:
		Simple_Elections(int d, int m, int y, int repsNum);
		Simple_Elections(const Simple_Elections& e) = delete;
		~Simple_Elections() {}

		virtual void addCitizenToList(const string& newName, int id, int birthYear, int DisNum = 0) override;
		virtual const list<citizen*>& Citizens() const { return citizens; }
	};
}

