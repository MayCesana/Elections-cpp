#pragma once
#include <list>
#include "citizen.h"
#include "results.h"
#include <fstream>

#define UNION 1
#define DIVIDED 2

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

namespace Elections
{
	class district
	{
	private:
		string name;
		int serial;
		int type; //UNION / DIVIDED
		int repNum; //number of Representatives for this district
		int Percentage = 0; //votes Percentage
		results* res; //district's results
	public:
		list<citizen*> citizensDis; //list of the citizens that allowed to vote in this district

		district(string Name, int serial, int type, int repnum);
		district(const district& dis);
		district(ifstream& file);
		~district();
		
		//getters
		const string& getName() const { return name; }
		const int getSerial() const { return serial; }
		const int getType() const { return type; }
		const int getRepNum() const { return repNum; }
		results* getRes() { return res; }
		results* getRes() const { return res; }

		//setters
		void setPercentage() { Percentage = res->getPercentage(); }

		friend ostream& operator<<(ostream& os, const district& d);
		void operator=(const district& other);

		//file
		void save(ofstream& file)const;
	};
}


