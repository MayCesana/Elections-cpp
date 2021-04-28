#pragma once
#include <iostream>

using namespace std;

template <class T>
class DynamicArray
{
public:
	DynamicArray(int size = 2) : logSize(0), phySize(size) {
		if (size < 1)
			throw invalid_argument("Invalid size");
		else
		{
			try {
				arr = new T[size];
			}
			catch (bad_alloc& ex) {
				cout << ex.what() << endl;
				exit(1);
			}
		}
	}
	DynamicArray(const DynamicArray& other) : arr(nullptr) { *this = other; }
	~DynamicArray() { delete[] arr; }

	const DynamicArray& operator=(const DynamicArray& other)
	{
		if (this != &other) {
			logSize = other.logSize;
			phySize = other.phySize;
			delete[] arr;
			try {
				arr = new T[phySize];
			}
			catch (bad_alloc& ex) {
				cout << ex.what() << endl;
				exit(1);
			}
			for (int i = 0; i < logSize; i++)
				arr[i] = other.arr[i];
		}
		return *this;
	}

	const T& operator[](int i) const { return at(i); }
	T& operator[](int i) { return at(i); }

	const T& at(int i) const
	{
		if (i < logSize)
			return arr[i];
		throw std::out_of_range("Accessed position is out of range!");
	}
	T& at(int i)
	{
		if (i < logSize)
			return arr[i];
		throw std::out_of_range("Accessed position is out of range!");
	}

	void push_back(const T& value)
	{
		if (logSize == phySize)
			resize();
		arr[logSize++] = value;
	}

	const T& front() const { return arr[0]; }
	int size() const { return logSize; }
	int capacity() const { return logSize; }
	bool empty() const { return logSize == 0; }
	void clear() { logSize = 0; }

private:
	T* arr;
	int logSize;
	int phySize;

	void resize()
	{
		phySize *= 2;
		T* temp;
		try {
			temp = new T[phySize];
		}
		catch (bad_alloc& ex) {
			cout << ex.what() << endl;
			exit(1);
		}
		for (int i = 0; i < logSize; i++)
			temp[i] = arr[i];

		delete[] arr;
		arr = temp;
	}

public:
	class iterator
	{
	private:
		DynamicArray* array;
		int				_i;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using different_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		iterator(DynamicArray& arr, int i) : array(&arr), _i(i) {}
		iterator(const iterator& other) : array(other.array), _i(other._i) {}

		friend class const_iterator;

		const iterator& operator=(const iterator& other) {
			array = other.array;
			_i = other._i;
			return *this;
		}

		bool operator==(const iterator& other) const {
			return (array == other.array) && (_i == other._i);
		}
		bool operator!=(const iterator& other) const {
			return !(*this == other);
		}

		T& operator*() { return array->arr[_i]; }
		T* operator->() { return &array->arr[_i]; }

		iterator& operator++() {
			++_i;
			return *this;
		}
		iterator operator++(int) {
			iterator temp(*this);
			++_i;
			return temp;
		}
		iterator& operator--() {
			--_i;
			return *this;
		}
		iterator operator--(int) {
			iterator temp(*this);
			--_i;
			return temp;
		}
	};

	class const_iterator
	{
	private:
		const DynamicArray* array;
		int _i;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using different_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;

		const_iterator(const iterator& other) : array(other.array), _i(other._i) {}

		const iterator& operator=(const iterator& other) {
			array = other.array;
			_i = other._i;
			return *this;
		}

		bool operator==(const iterator& other) const {
			return (array == other.array) && (_i == other._i);
		}
		bool operator!=(const iterator& other) const {
			return !(*this == other);
		}

		const T& operator*() { return array->arr[_i]; }
		const T* operator->() { return &array->arr[_i]; }

		iterator& operator++() {
			++_i;
			return *this;
		}
		iterator operator++(int) {
			iterator temp(*this);
			++_i;
			return temp;
		}
		iterator& operator--() {
			--_i;
			return *this;
		}
		iterator operator--(int) {
			iterator temp(*this);
			--_i;
			return temp;
		}
	};

public:
	
	void insert(const iterator& pos, const T& val) {
		if (logSize == phySize)
			resize();

		iterator itrEnd = end();
		iterator itrCurrent = itrEnd, itrPrev = --itrEnd;
		while (itrCurrent != pos)
		{
			*itrCurrent = *itrPrev;
			itrCurrent = itrPrev--;
		}

		iterator p = pos;
		*p = val;
		++logSize;
	}
	iterator erase(const iterator& pos) 
	{
		if (pos >= end()) {
			throw std::out_of_range("erasing an element out of bounds");
		}
		if (pos + 1 != end())
			copy(pos + 1, end(), pos);
		--logSize;
		return pos;
	} 
	iterator erase(const iterator& first, const iterator& last)
	{
		if (last >= end()) {
			throw std::out_of_range("erasing an element out of bounds");
		}
		copy(last, end(), first);
		logSize -= last - first;
		return first;
	}

	iterator begin() { return iterator(*this, 0); }
	iterator end() { return iterator(*this, logSize); }
	const_iterator begin() const {return const_iterator(*this, 0);}
	const_iterator end() const {return const_iterator(*this, logSize);}
	const_iterator cbegin() const { return const_iterator(*this, 0); }
	const_iterator cend() const { return const_iterator(*this, logSize); }
	iterator rbegin() { return iterator(*this, logSize-1); }
	iterator rend() { return iterator(*this, 0); }
	const_iterator rbegin() const{ return iterator(*this, logSize - 1); }
	const_iterator rend() const{ return iterator(*this, 0); }

	void print() const {
		for (int i = 0; i < logSize; i++)
			cout << *(arr[i]) << " ";
	}
};