#include "FileManager.h"
#include "Elections.h"
#include "NormalElections.h"
#include "SimpleElections.h"

namespace Elections
{
	elections* FileManager::load_elections(const string& filename)
	{
		elections* e = nullptr;
		ifstream file;
		file.open(filename, ios::binary);

		if (!file) {
			throw "Error with file";
		}

		int type, d, m, y;
		file.read(rcastc(&d), sizeof(int));
		file.read(rcastc(&m), sizeof(int));
		file.read(rcastc(&y), sizeof(int));
		file.read(rcastc(&type), sizeof(int));

		if (type == SIMPLE)
		{
			int repsNum;
			file.read(rcastc(&repsNum), sizeof(int));
			e = new Simple_Elections(d, m, y, repsNum);
		}
		else
			e = new Normal_Elections(d, m, y);

		if (!file.eof())
			e->readElections(file);

		file.close();
		return e;
	}

	void FileManager::save_elections(elections* e, const string& filename)
	{
		ofstream file(filename, ios::binary | ios::trunc);
		if (!file) {
			throw "Error with file";
		}
		int i, n;

		e->getDate().save(file); //save date

		int type = static_cast<int>(FileManager::getType(e));

		file.write(rcastcc(&type), sizeof(int)); //elections type

		if (type == SIMPLE)
		{
			n = e->getDistricts()[0]->getRepNum();
			file.write(rcastcc(&n), sizeof(int));
		}
		else
			dynamic_cast<Normal_Elections*>(e)->save(file);//save districts

		//save citizens
		n = e->Citizens().size();
		if (n > 0)
		{
			file.write(rcastcc(&n), sizeof(int)); //number of citizens
			for (auto itr = e->Citizens().begin(); itr != e->Citizens().end(); ++itr)
			{
				(*(*itr)).save(file);
			}
		}

		//save parties
		int partiesNum = e->getParLogSize();
		file.write(rcastcc(&partiesNum), sizeof(int)); //number of parties
		if (partiesNum > 0)
		{
			for (i = 0; i < partiesNum; i++)
				e->getParties()[i]->save(file);
		}

		//save votes per district
		int districtsNum = e->getDisLogSize();
		file.write(rcastcc(&districtsNum), sizeof(int));
		if (districtsNum > 0)
		{
			for (i = 0; i < districtsNum; i++)
				e->getDistricts()[i]->getRes()->save(file);
		}

		file.close();
	}

	FileManager::electinosType FileManager::getType(const elections* e)
	{
		if (typeid(*e) == typeid(Normal_Elections))
			return electinosType::NORMAL;
		else
			return electinosType::SIMPLE;
	}
}