#pragma once
#include <memory>
#include <stdexcept>

class UnionFind
{
private:
	std::unique_ptr<int[]> id;
	std::unique_ptr<int[]> sz;
	int size;
	int numComponents; //number of components in union find
public:
	UnionFind(int _size)
	{
		if (_size <= 0) throw std::invalid_argument("Union Find constructor argument smaller than 1");
		id = std::unique_ptr<int[]>(new int[_size]);
		sz = std::unique_ptr<int[]>(new int[_size]);
		size = _size;
		numComponents = _size;
		for (int i = 0; i < size; i++)
		{
			id[i] = i;
			sz[i] = 1;
		}
	}
	void unify(int viA, int viB); 
	int find(int vertexIndex); //return the root of component
	bool connected(int viA, int viB); //check if 2 vertices in the same component
	int getComponentSize(int vi); //get size of component where vi belongs
	int getNumOfComponents(); //return number of components in this union find
};

int UnionFind::find(int vi) //return the root
{
	//find the root of p
	int root = vi;
	while (root != id[root]) root = id[root];

	//Do path compression
	while (vi != root)
	{
		int next = id[vi];
		id[vi] = root;
		vi = next;
	}

	return root;
}
bool UnionFind::connected(int viA, int viB)
{
	return find(viA) == find(viB);
}
int UnionFind::getComponentSize(int vi)
{
	return sz[find(vi)];
}
int UnionFind::getNumOfComponents()
{
	return numComponents;
}
void UnionFind::unify(int viA, int viB) //unify components containing viA and viB
{
	if (viA >= 0 && viA < size && viB >= 0 && viB < size)
	{
		if (viA != viB && !connected(viA, viB))
		{
			int rootA = find(viA);
			int rootB = find(viB);
			if (sz[rootA] > sz[rootB]) //make root 
			{
				id[rootB] = rootA;
				sz[rootA] += sz[rootB];
				sz[rootB] = 0;
			}
			else
			{
				id[rootA] = rootB;
				sz[rootB] += sz[rootA];
				sz[rootA] = 0;
			}

			numComponents--;
		}
	}

}