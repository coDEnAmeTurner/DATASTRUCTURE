#include <iostream>
#include "header/AdjacencyMatrixGraph.h"

int main()
{
	Undirected_Adjacency_Matrix_Graph<int> g;
	g.addVertex(0);
	g.addVertex(1);
	g.addVertex(2);
	g.addVertex(3);
	g.addVertex(4);
	g.addVertex(5);
	g.addVertex(6);
	g.addVertex(7);
	
	g.addEdge(0, 1, 10);
	g.addEdge(0, 2, 1);
	g.addEdge(0, 3, 4);
	g.addEdge(1, 2, 3);
	g.addEdge(1, 4, 0);
	g.addEdge(2, 5, 8);
	g.addEdge(2, 3, 2);
	g.addEdge(5, 4, 1);
	g.addEdge(5, 3, 2);
	g.addEdge(5, 6, 6);
	g.addEdge(5, 7, 9);
	g.addEdge(3, 6, 7);
	g.addEdge(6, 7, 12);
	g.addEdge(7, 4, 8);

	auto vec = g.eagerPrims(0);
	auto mst = g.makeMST(vec);
	mst.dfsPrint(0);
	std::cin.get();
}