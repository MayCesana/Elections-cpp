#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "NormalElections.h"
#include "SimpleElections.h"
#include "menu.h"

using namespace std;
using namespace Elections;

const int MAX_SIZE = 150;

void addDistrictMenu(elections* e)
{
	string name;
	int type, Repnum;
	cout << "please enter District name:" << endl;

	cin.ignore();
	cin >> name;
	cout << "please enter the type of the district (union = 1, diveded=2) and the number of representatives:" << endl;
	cin >> type >> Repnum;
	
	dynamic_cast<Normal_Elections*>(e)->addDistrict(name, Repnum, type);
	cout << "District added successfully" << endl;
}

void addCitizenMenu(elections* e)
{
	char name[MAX_SIZE];
	int id, year, district = 0;
	cout << "Please enter name:" << endl;
	cin.ignore();
	cin.getline(name, MAX_SIZE);
	cout << "Please enter id, year of birth and district number:" << endl;
	cin >> id >> year >> district;
	
	if (typeid(*e) == typeid(Normal_Elections))
	{
		e->addCitizenToList(name, id, year, district);
	}

	else //Simple elections
	{
		e->addCitizenToList(name, id, year);
	}

	cout << "Citizen added successfully" << endl;
}

void addPartyMenu(elections* e)
{
	char name[MAX_SIZE];
	int id;
	cout << "Please enter the party name:" << endl;
	cin.ignore();
	cin.getline(name, MAX_SIZE);
	cout << "Please enter the prime minister candidate ID:" << endl;
	cin >> id;
	citizen* prime = e->searchByID(id);
	
	e->add_Party(name, prime);

	cout << "party added successfully" << endl;
}

void addRepMenu(elections* e)
{
	int id, districtNum, partyNum;
	cout << "Please enter representative ID, the number of party and the number of district:" << endl;
	cin >> id >> partyNum >> districtNum;;
	if (typeid(*e) == typeid(Simple_Elections))
		districtNum = 0;

	if (e->getDisLogSize() < districtNum + 1 || districtNum < 0)
		throw invalid_argument("There is no such district");
	
	if (e->getParLogSize() < partyNum + 1 || partyNum < 0)
		throw invalid_argument("There is no such party");

	if (e->getParties()[partyNum]->enoughReps(districtNum))
		throw out_of_range("The representatives list is full");

	e->addRep(id, partyNum, districtNum);
	cout << "The representative added successfully" << endl;
}

void voteMenu(elections* e)
{
	int id, partyNum;
	
	cout << "Please write your ID and the number of the party you're voting for:" << endl;
	cin >> id >> partyNum;

	citizen* voter = e->searchByID(id);
	if (voter == nullptr)
		throw invalid_argument("There is no such citizen!");
	
	if (voter->isVoted())
		throw invalid_argument("You already voted!");

	if (partyNum >= e->getParLogSize() || partyNum < 0)
		throw invalid_argument("There is no such party");

	e->vote(*voter, partyNum);
	cout << "Your vote has been picked up!" << endl;

}

void showResultsMenu(elections* e)
{
	Normal_Elections* NE = dynamic_cast<Normal_Elections*>(e);
	e->preResults();
	
	if (typeid(*e) == typeid(Normal_Elections))
	{
		NE->makeFinalResults();
	}

	e->showResults(); //results by district
	if (typeid(*e) == typeid(Normal_Elections))
	{
		NE->showFinalResult();
	}
}

elections* LoadElectionsMenu()
{
	string name;
	cout << "please write the name of the file" << endl;
	cin >> name;
	elections* e = FileManager::load_elections(name);
	if (e != nullptr)
		cout << "the file loaded successfully!" << endl;
	return e;
}

void SaveElectionMenu(elections* e)
{
	string name;
	cout << "please enter a name for the file :" << endl;
	cin >> name;
	FileManager::save_elections(e, name);
	cout << "the file saved successfully!" << endl;
}

void chooseFromMenu(int op, elections** e)
{
	switch (op)
	{
	case(1):
		if (typeid(**e) == typeid(Simple_Elections))
			throw invalid_argument("adding district in simple elections is not allowed!");
		else
			addDistrictMenu(*e);
		break;
	case(2):
		addCitizenMenu(*e);
		break;
	case(3):
		addPartyMenu(*e);
		break;
	case(4):
		addRepMenu(*e);
		break;
	case(5):
		(*e)->printDistricts();
		break;
	case(6):
		printList((*e)->Citizens());
		break;
	case(7):
		(*e)->printParties();
		break;
	case(8):
		voteMenu(*e);
		break;
	case(9):
		showResultsMenu(*e);
		break;
	case(10):
		break;
	case(11):
		SaveElectionMenu(*e);
		break;
	case(12):
		delete* e;
		*e = LoadElectionsMenu();
		break;
	default:
		throw invalid_argument("wrong input!, please try again..");
		break;
	}
	system("pause");
	system("CLS");
}

elections* startElections(int type)
{
	elections* e;
	int d, m, y, repsNum;
	if (type != 1 && type != 2)
		throw "wrong input! please try again:";

	cout << "please enter electios date(day(1-31) -> month(1-12) -> year): " << endl;
	cin >> d >> m >> y;
	if (type == 1)
	{
		cout << "please enter the number of representitives" << endl;
		cin >> repsNum;
		e = new Simple_Elections(d, m, y, repsNum);
	}
	else
		e = new Normal_Elections(d, m, y);
	return e;
}

void chooseFromFirstMenu(int op, elections** e)
{
	bool ok = false;
	if (op < 1 || op > 3)
	{
		throw invalid_argument("wrong input! please try again:");
	}
	if (op == 1)
	{
		int type;
		cout << "please enter 1 for simple elections or 2 for normal elections:" << endl;
		cin >> type;
		while(!ok)
		{
			ok = true;
			try {
				*e = startElections(type);
			}
			catch (std::exception& ex)
			{
				cout << ex.what() << endl <<
					"Let's try again..." << endl;
				ok = false;
			}
			catch (const char* tryAgain)
			{
				cout << tryAgain << endl;
				cin >> type;
				ok = false;
			}
		}
	}
	else if (op == 2)
	{
		try {
			*e = LoadElectionsMenu();
		}
		catch (const char* fileErrorMsg) { //file Error
			cout << fileErrorMsg << endl;
			exit(1);
		}
	}

	else if (op == 3)
		std::exit(1);
}

int main()
{
	bool ok = false;
	int op = 0;
	elections* e = nullptr;
	Menu::showFirst_Menu();
	
	
	while (!ok)
	{
		ok = true;
		try {
			cin >> op;
			chooseFromFirstMenu(op, &e);
		}
		catch (std::exception& ex)
		{
			cout << ex.what() << endl;
			ok = false;
		}
	}

	cout << "WELCOME TO ELECTIONS " << e->getDate() << endl;

	while (op != 10)
	{
		Menu::showMenu();
		cin >> op;
		system("CLS");
		try
		{
			chooseFromMenu(op, &e);
		}
		catch (std::exception& ex)
		{
			cout << "Error: " << ex.what() << endl;
			system("pause");
			system("CLS");
		}
		catch (bad_alloc& ex) {
			cout << ex.what() << endl;
			exit(1);
		}
		catch (const char* fileErrorMsg) { //file error
			cout << fileErrorMsg << endl;
			exit(1);
		}
	}

	cout << "The elections is end... goodBye." << endl;
	system("pause");

	delete e;
}