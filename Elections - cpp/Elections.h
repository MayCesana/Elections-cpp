#pragma once
#include <iostream>
#include <list>
#include "DynamicArray.h"
#include "district.h"
#include "party.h"
#include <typeinfo>
#include "FileManager.h"
#include <string>
#include "Date.h"

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

using namespace std;
namespace Elections
{
	class elections
	{
	protected:
		date Date;
		DynamicArray<party*> parties; //array of pointers to all the parties
		DynamicArray<district*> districts; //array of pointers to all the districts
		list<citizen*> citizens; //voters book
		
	public:
		elections(int d, int m, int y) : Date(d, m, y) {}
		elections(const elections& e) = delete;
		virtual ~elections() {}

		//getters
		int getParLogSize() const { return parties.size(); }
		const DynamicArray<party*>& getParties() const { return parties; }
		const date& getDate() const { return Date; }
		int getDisLogSize() const { return districts.size(); }
		const DynamicArray<district*>& getDistricts() const { return districts; }
		virtual const list<citizen*>& Citizens() { return citizens; }

		void add_Party(const string& name, citizen* prime);
		void addRep(int ID, int partyNum, int districtNum = 0);
		virtual void addCitizenToList(const string& newName, int id, int birthYear, int DisNum = 0) = 0; //abstract
		citizen* searchByID(int ID);

		void printParties();
		void printDistricts();

		bool vote(citizen&, int partyNum);

		void preResults(); //Calculats the results to any district
		void setPre(); //set percentages to any party in all the districts
		void setRepsToParties();
		void showResults(); //districts results

		//file
		void readCitizen(ifstream& file);
		void readparty(ifstream& file);
		void readElections(ifstream& file);
	};
}



