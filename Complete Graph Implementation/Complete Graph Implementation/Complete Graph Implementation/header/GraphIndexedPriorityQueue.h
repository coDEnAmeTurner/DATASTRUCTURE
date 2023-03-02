#pragma once
#include <memory>
#include "Node_Dist_Struct.h"
//this indexed priority queue is only for graph implementation

struct intLess {
	bool operator() (const int& a, const int& b)
	{
		return a < b;
	}
};
struct intGreater {
	bool operator() (const int& a, const int& b)
	{
		return a > b;
	}
};

template <class valTy, class lessPred>
class graph_indexed_min_priority_queue 
{
public:
	graph_indexed_min_priority_queue(int _vertexListSize)
	{
		vals = std::unique_ptr<valTy[]>(new valTy[_vertexListSize]);
		pm = std::unique_ptr<int[]>(new int[_vertexListSize]);
		im = std::unique_ptr<int[]>(new int[_vertexListSize]);
		for (int i = 0; i < _vertexListSize; i++)
		{
			pm[i] = -1;
			im[i] = -1;
		}
		vertexListSize = _vertexListSize;
		sz = 0;
	}
	bool contains(int ki)
	{
		if (pm[ki] < sz && pm[ki] >= 0)
			return true;
		return false;
	}
	bool isFull()
	{
		if (sz == vertexListSize) return true;
		return false;
	}
	bool isEmpty()
	{
		if (sz == 0) return true;
		return false;
	}
	void insert(int ki, valTy value)
	{
		vals[ki] = value;
		pm[ki] = sz;
		im[sz] = ki;
		sz++;
		swim(sz - 1);
	}
	void swim(int imIndex)
	{
		int i = imIndex;
		for (int p = (i - 1) / 2; i > 0 && less(vals[im[i]], vals[im[p]]);)
		{
			//swap
			swap(i, p);
			i = p;
			p = (i - 1) / 2;
		}
	}
	void sink(int imIndex)
	{
		int i = imIndex;
		while (true)
		{
			int left = i * 2 + 1;
			int right = i * 2 + 2;
			int smallest = left;

			if (right < sz && less(vals[im[right]], vals[im[left]]))
				smallest = right;
			if (left >= sz || less(vals[im[i]], vals[im[smallest]])) break;
			swap(i, smallest);
			i = smallest;
		}
	}
	valTy poll(int ki)
	{
		valTy polledVal = vals[ki];
		int i = pm[ki];
		swap(i, sz - 1);
		sz--;
		sink(i);
		swim(i);
		pm[ki] = -1;
		im[sz] = -1;
		return polledVal;
	}
	valTy pollRoot()
	{
		return poll(im[0]);
	}
	int peekRootKi()
	{
		if (sz > 0)
			return im[0];
		return -1;
	}
	void decreaseKey(int ki, valTy value)
	{
		if (less(value, vals[ki]))
		{
			vals[ki] = value;
			swim(pm[ki]);
		}
	}
	void increaseKey(int ki, valTy value)
	{
		if (less(vals[ki], value))
		{
			vals[ki] = value;
			sink(pm[ki]);
		}
	}
	void print()
	{
		for (int i = 0; i < sz; i++)
		{
			std::cout << vals[im[i]] << " ";
		}
	}
private:
	std::shared_ptr<valTy[]> vals;
	std::shared_ptr<int[]> pm;
	std::shared_ptr<int[]> im;
	int vertexListSize;
	int sz;
	lessPred less;
	void swap(int nodeI, int nodeJ)
	{
		pm[im[nodeJ]] = nodeI;
		pm[im[nodeI]] = nodeJ;
		int tmp = im[nodeI];
		im[nodeI] = im[nodeJ];
		im[nodeJ] = tmp;
	}
};
