#pragma once
#include <fstream>

using namespace std;

namespace Elections
{
	class elections;
	class FileManager
	{
	public:
		enum electinosType { NORMAL = 1, SIMPLE };

	public:
		static elections* load_elections(const string& filename);
		static void save_elections(elections* e, const string& filename);
		static electinosType getType(const elections* e);
	};
}

