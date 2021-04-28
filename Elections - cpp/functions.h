#pragma once
#include <iostream>
#include "citizen.h"

using namespace std;
using namespace Elections;

class PlusOne
{
public:
	void operator()(int& x) { x++; }
};

template <class T, class Fun>
void updateByOne(T val, Fun f)
{
	f(val);
}

inline void printList(const list<citizen*>& lst)
{
	if (lst.empty())
		throw out_of_range("There are no citizens..");
	for (auto itr = lst.begin(); itr != lst.end(); ++itr) {
		cout << *(*itr) << endl;
	}
}

template <class T>
void Swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}





