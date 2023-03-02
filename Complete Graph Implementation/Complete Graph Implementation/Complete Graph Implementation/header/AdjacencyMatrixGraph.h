#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <queue>
#include "Node_Dist_Struct.h"
#include "GraphIndexedPriorityQueue.h"
#include "UnionFind.h"

struct UndirectedEdge {
	int first;
	int second;
	int cost;

	UndirectedEdge() {}	
	UndirectedEdge(int _first, int _second, int _cost)
	{
		first = _first;
		second = _second;
		cost = _cost;
	}
};

struct unEdgeGreater {
	bool operator() (const UndirectedEdge& a, const UndirectedEdge& b)
	{
		return a.cost > b.cost;
	}
};
struct unEdgeLess {
	bool operator() (const UndirectedEdge& a, const UndirectedEdge& b)
	{
		return a.cost < b.cost;
	}
};
template <typename T>
class Undirected_Adjacency_Matrix_Graph {
public:
	int search(T value)
	{
		if (vertexList.size() != 0)
		{
			return std::find(vertexList.begin(), vertexList.end(), value) - vertexList.begin();
		}
		return -1;
	}
	void addVertex(T vtxContent)
	{
		vertexList.push_back(vtxContent);
		incrementMatrixSize();
	}
	void addEdge(int first, int second, int cost)
	{
		if (!vertexList.empty())
		{
			if (first >= 0 && first < vertexList.size() && second >= 0 && second < vertexList.size())
			{
				if (adjacencyMatrix[first][second] == -1)
				{
					adjacencyMatrix[first][second] = cost;

					adjacencyMatrix[second][first] = cost;
					
				}
			}
		}
	}
	void removeVertex(int vtxIndex)
	{
		if (vertexList.empty() == false)
		{
			if (vtxIndex >= 0 && vtxIndex < vertexList.size())
			{
				vertexList.erase(vertexList.begin() + vtxIndex);
				adjacencyMatrix.erase(adjacencyMatrix.begin() + vtxIndex);
				for (int i = 0; i < adjacencyMatrix.size(); i++)
				{
					adjacencyMatrix[i].erase(adjacencyMatrix[i].begin() + vtxIndex);
				}
			}

		}
	}
	void removeEdge(int first, int second)
	{
		if (vertexList.empty() == false)
		{
			if (first >= 0 && first < vertexList.size() && second >= 0 && second < vertexList.size())
			{
				if (adjacencyMatrix[first][second] > -1)
				{
					adjacencyMatrix[first][second] = 0;
				}
				
				if (adjacencyMatrix[second][first] > -1)
				{
					adjacencyMatrix[second][first] = 0;
				}
			}
		}
	}
	void dfsPrint(int start)
	{
		int n = vertexList.size();
		std::shared_ptr<bool[]> visited(new bool[n] {false});

		dfsPrint_rec(start, visited);
	}
	void bfsPrint(int s)
	{
		if (vertexList.empty() == false)
		{
			if (s >= 0 && s < vertexList.size())
			{
				std::queue<int> q;
				int n = vertexList.size();
				std::unique_ptr<bool[]> visited(new bool[n] {false});
				q.push(s);
				visited[s] = true;

				while (!q.empty())
				{
					int front = q.front();
					q.pop();
					std::cout << vertexList[front] << " ";

					//find neighbours of the front to push to queue:
					for (int i = 0; i < vertexList.size(); i++)
					{
						if (adjacencyMatrix[front][i] > -1 && !visited[i])
						{
							q.push(i); 
							visited[i] = true;
						}
					}

				}
			}
		}
	}
	void lazyDijkstra(int s, std::shared_ptr<int[]>& dist, std::shared_ptr<int[]>& prev)
	{
		int n = vertexList.size();
		std::unique_ptr<bool[]> vis(new bool[n]);
		dist = std::shared_ptr<int[]>(new int[n]);
		prev = std::shared_ptr<int[]>(new int[n]);
		std::priority_queue <Node_Dist, std::vector<Node_Dist>, Node_Dist_Less> pq;
		for (int i = 0; i < n; i++)
		{
			vis[i] = false;
			dist[i] = INT_MAX;
			prev[i] = -1;

		}
		dist[s] = 0;

		pq.push(Node_Dist(s, 0));
		while (pq.empty() == false)
		{
			Node_Dist minDist = pq.top();
			pq.pop();
			vis[minDist.vertexIndex] = true;
			if (dist[minDist.vertexIndex] < minDist.dist) continue;
			//relax all neighbours of minDist.vertexIndex
			for (int i = 0; i < vertexList.size(); i++)
			{
				if (adjacencyMatrix[minDist.vertexIndex][i] > -1)
				{
					int neighbour = i;
					int newDist = minDist.dist + adjacencyMatrix[minDist.vertexIndex][i] ;
					if (newDist < dist[neighbour])
					{
						prev[neighbour] = minDist.vertexIndex;
						dist[neighbour] = newDist;
						pq.push(Node_Dist(neighbour, newDist));
					}

				}
			}
		}
	}
	std::vector<int> findShortestPathLazy(int s, int e)
	{
		std::vector<int> path;
		std::shared_ptr<int[]> dist;
		std::shared_ptr<int[]> prev;
		lazyDijkstra(s, dist, prev);
		if (dist[e] != INT_MAX)
		{
			for (int at = e; at != -1; at = prev[at])
			{
				path.push_back(at);
			}
			std::reverse(path.begin(), path.end());
		}
		return path;
	}
	void eagerDijkstra(int s, std::shared_ptr<int[]>& dist, std::shared_ptr<int[]>& prev)
	{
		int n = vertexList.size();
		std::unique_ptr<bool[]> vis(new bool[n]);
		dist = std::shared_ptr<int[]>(new int[n]);
		prev = std::shared_ptr<int[]>(new int[n]);
		graph_indexed_min_priority_queue<int, intLess> ipq(vertexList.size());
		for (int i = 0; i < n; i++)
		{
			vis[i] = false;
			dist[i] = INT_MAX;
			prev[i] = -1;

		}
		dist[s] = 0;

		ipq.insert(s, 0);
		while (ipq.isEmpty() == false)
		{
			int minDistKi = ipq.peekRootKi();
			int minDist = ipq.pollRoot();
			vis[minDistKi] = true;

			if (dist[minDistKi] < minDist) continue;
			//relax all neighbours of minDistKi
			for (int i = 0; i < vertexList.size(); i++)
			{
				if (adjacencyMatrix[minDistKi][i] > -1)
				{
					int neighbour = i;
					int newDist = minDist + adjacencyMatrix[minDistKi][i] ;
					if (newDist < dist[neighbour])
					{
						prev[neighbour] = minDistKi;
						dist[neighbour] = newDist;
						if (ipq.contains(neighbour))
							ipq.insert(neighbour, newDist);
						else
							ipq.decreaseKey(neighbour, newDist);
					}

				}
			}
		}
	}
	std::vector<int> findShortestPathEager(int s, int e)
	{
		std::vector<int> path;
		std::shared_ptr<int[]> dist;
		std::shared_ptr<int[]> prev;
		lazyDijkstra(s, dist, prev);
		if (dist[e] != INT_MAX)
		{
			for (int at = e; at != -1; at = prev[at])
			{
				path.push_back(at);
			}
			std::reverse(path.begin(), path.end());
		}
		return path;
	}
	std::vector<UndirectedEdge> kruskal()
	{
		if (vertexList.empty() == false && vertexList.size() > 1)
		{
			UnionFind uf(vertexList.size());
			std::vector<UndirectedEdge> mstEdgeList;
			std::vector<UndirectedEdge> processed;
			while (uf.getNumOfComponents() != 1)
			{
				//find the smallest edge
				int currMin = INT_MAX;
				int minI ;
				int minJ ;
				for (int i = 0; i < adjacencyMatrix.size(); i++)
				{
					for (int j = 0; j < adjacencyMatrix.size(); j++)
					{
						if (std::find_if(processed.begin(), processed.end(), [&](UndirectedEdge edge) {
							return (edge.first == i && edge.second == j) || (edge.first == j && edge.second == i);
							}) == processed.end())
						{
							if (adjacencyMatrix[i][j] < currMin && adjacencyMatrix[i][j] > -1)
							{
								currMin = adjacencyMatrix[i][j];
								minI = i;
								minJ = j;
							}
						}
					}
				}
				if (!uf.connected(minI, minJ))
				{
					uf.unify(minI, minJ);
					mstEdgeList.push_back(UndirectedEdge(minI, minJ, adjacencyMatrix[minI][minJ]));
				}
				processed.push_back(UndirectedEdge(minI, minJ, adjacencyMatrix[minI][minJ]));
			}
			return mstEdgeList;
		}
		return std::vector<UndirectedEdge>();
	}
	std::vector<UndirectedEdge> lazyPrims(int start)
	{
		if (start < vertexList.size() && start >= 0)
		{
			int m = vertexList.size() - 1;
			int edgeCount = 0;
			std::vector<UndirectedEdge> mstEdge;
			std::shared_ptr<bool[]> visited(new bool[m + 1]);
			for (int i = 0; i < vertexList.size(); i++)
			{
				visited[i] = false;
			}
			std::priority_queue<UndirectedEdge, std::vector<UndirectedEdge>, unEdgeGreater> pq;

			addEdges(start, visited, pq);

			while (!pq.empty() && edgeCount != m)
			{
				UndirectedEdge edge = pq.top();
				pq.pop();

				int destNodeIndex = (visited[edge.first] == true) ? edge.second : edge.first;

				if (visited[destNodeIndex])
					continue;

				mstEdge.push_back(edge);
				edgeCount++;

				addEdges(destNodeIndex, visited, pq);
			}
			if (edgeCount != m)
				return std::vector<UndirectedEdge>();
			return mstEdge;
		}
		return std::vector<UndirectedEdge>();
	}
	std::vector<UndirectedEdge> eagerPrims(int start)
	{
		if (start < vertexList.size() && start >= 0)
		{
			int m = vertexList.size() - 1;
			int edgeCount = 0;
			std::vector<UndirectedEdge> mstEdge;
			std::shared_ptr<bool[]> visited(new bool[m + 1]);
			for (int i = 0; i < vertexList.size(); i++)
			{
				visited[i] = false;
			}
			graph_indexed_min_priority_queue<UndirectedEdge, unEdgeLess> ipq(vertexList.size());

			relaxEdgesAtNode(start, visited, ipq);

			while (!ipq.isEmpty() && edgeCount != m)
			{
				int destNodeIndex = ipq.peekRootKi();
				UndirectedEdge edge = ipq.pollRoot();

				mstEdge.push_back(edge);
				edgeCount++;

				relaxEdgesAtNode(destNodeIndex, visited, ipq);
			}
			if (edgeCount != m)
				return std::vector<UndirectedEdge>();
			return mstEdge;
		}
		return std::vector<UndirectedEdge>();
	}
	Undirected_Adjacency_Matrix_Graph<T> makeMST(std::vector<UndirectedEdge> mstEdgeList)
	{
		if (vertexList.size() > 1 && mstEdgeList.size() > 0)
		{
			Undirected_Adjacency_Matrix_Graph<T> mst;
			mst.vertexList = std::vector<T>(vertexList);
			mst.adjacencyMatrix.resize(vertexList.size());
			for (int i = 0; i < vertexList.size(); i++)
			{
				mst.adjacencyMatrix[i].resize(vertexList.size(), -1);

			}

			for (auto& edge : mstEdgeList)
			{
				mst.adjacencyMatrix[edge.first][edge.second] = edge.cost;
				mst.adjacencyMatrix[edge.second][edge.first] = edge.cost;
			}
			return mst;
		}
		return Undirected_Adjacency_Matrix_Graph<T>();
	}
private:
	std::vector<T> vertexList;
	std::vector<std::vector<int>> adjacencyMatrix;
	void incrementMatrixSize()
	{
		//resize already add more vectors to the matrix
		adjacencyMatrix.resize(adjacencyMatrix.size() + 1);
		for (int i = 0; i < adjacencyMatrix.size(); i++)
		{
			adjacencyMatrix[i].resize(adjacencyMatrix.size(), -1);
		}
	}
	void dfsPrint_rec(int at, std::shared_ptr<bool[]> visited)
	{
		if (visited[at] == true) return;
		visited[at] = true;
		std::cout << vertexList[at] << " ";

		for (int i = 0; i < vertexList.size(); i++)
		{
			if (adjacencyMatrix[at][i] > -1) dfsPrint_rec(i, visited);
		}
	}
	void addEdges(int at, std::shared_ptr<bool[]> visited, std::priority_queue<UndirectedEdge, std::vector<UndirectedEdge>, unEdgeGreater>& pq)
	{
		visited[at] = true;

		for (int i = 0; i < vertexList.size(); i++)
		{
			if (adjacencyMatrix[at][i] > -1)
			{ 
				if (!visited[i])
					pq.push(UndirectedEdge(at, i, adjacencyMatrix[at][i]));
			}
		}
	}
	void relaxEdgesAtNode(int at, std::shared_ptr<bool[]> visited, graph_indexed_min_priority_queue<UndirectedEdge, unEdgeLess>& ipq)
	{
		visited[at] = true;

		for (int i = 0; i < vertexList.size(); i++)
		{
			if (adjacencyMatrix[at][i] > -1)
			{
				int neighbour = i;

				if (!visited[neighbour])
				{
					if (!ipq.contains(neighbour))
						ipq.insert(neighbour, UndirectedEdge(at, neighbour, adjacencyMatrix[at][i]));
					else
						ipq.decreaseKey(neighbour, UndirectedEdge(at, neighbour, adjacencyMatrix[at][i]));
				}

			}
		}
	}
};

template <typename T>
class Directed_Adjacency_Matrix_Graph {
private:
	std::vector<T> vertexList;
	std::vector<std::vector<int>> adjacencyMatrix;
	void incrementMatrixSize()
	{
		//resize already add more vectors to the matrix
		adjacencyMatrix.resize(adjacencyMatrix.size() + 1);
		for (int i = 0; i < adjacencyMatrix.size(); i++)
		{
			adjacencyMatrix[i].resize(adjacencyMatrix.size(), -1);
		}
	}
	void dfsPrint_rec(int at, std::shared_ptr<bool[]> visited)
	{
		if (visited[at] == true) return;
		visited[at] = true;
		std::cout << vertexList[at] << " ";

		for (int i = 0; i < vertexList.size(); i++)
		{
			if (adjacencyMatrix[at][i] > -1) dfsPrint_rec(i, visited);
		}
	}

public:
	int search(T value)
	{
		if (vertexList.size() != 0)
		{
			return std::find(vertexList.begin(), vertexList.end(), value) - vertexList.begin();
		}
		return -1;
	}
	void addVertex(T vtxContent)
	{
		vertexList.push_back(vtxContent);
		incrementMatrixSize();
	}
	void addEdge(int start, int end, int cost)
	{
		if (!vertexList.empty())
		{
			if (start >= 0 && start < vertexList.size() && end >= 0 && end < vertexList.size())
			{
				if (adjacencyMatrix[start][end] == -1)
				{
					adjacencyMatrix[start][end]  = cost;

				}
			}
		}
	}
	void removeVertex(int vtxIndex)
	{
		if (vertexList.empty() == false)
		{
			if (vtxIndex >= 0 && vtxIndex < vertexList.size())
			{
				vertexList.erase(vertexList.begin() + vtxIndex);
				adjacencyMatrix.erase(adjacencyMatrix.begin() + vtxIndex);
				for (int i = 0; i < adjacencyMatrix.size(); i++)
				{
					adjacencyMatrix[i].erase(adjacencyMatrix[i].begin() + vtxIndex);
				}
			}

		}
	}
	void removeEdge(int start, int end)
	{
		if (vertexList.empty() == false)
		{
			if (start >= 0 && start < vertexList.size() && end >= 0 && end < vertexList.size())
			{
				if (adjacencyMatrix[start][end] > -1)
				{
					adjacencyMatrix[start][end]  = 0;
				}
			}
		}
	}
	void dfsPrint(int start)
	{
		int n = vertexList.size();
		std::shared_ptr<bool[]> visited(new bool[n] {false});

		dfsPrint_rec(start, visited);
	}
	void bfsPrint(int s)
	{
		if (vertexList.empty() == false)
		{
			if (s >= 0 && s < vertexList.size())
			{
				std::queue<int> q;
				int n = vertexList.size();
				std::unique_ptr<bool[]> visited(new bool[n] {false});
				q.push(s);
				visited[s] = true;

				while (!q.empty())
				{
					int front = q.front();
					q.pop();
					std::cout << vertexList[front] << " ";

					//find neighbours of the front to push to queue:
					for (int i = 0; i < adjacencyMatrix.size(); i++)
					{
						if (adjacencyMatrix[front][i] > -1 && !visited[i])
						{
							q.push(i);
							visited[i] = true;
						}
					}

				}
			}
		}
	}
	void lazyDijkstra(int s, std::shared_ptr<int[]>& dist, std::shared_ptr<int[]>& prev)
	{
		int n = vertexList.size();
		std::unique_ptr<bool[]> vis(new bool[n]);
		dist = std::shared_ptr<int[]>(new int[n]);
		prev = std::shared_ptr<int[]>(new int[n]);
		std::priority_queue <Node_Dist, std::vector<Node_Dist>, Node_Dist_Less> pq;
		for (int i = 0; i < n; i++)
		{
			vis[i] = false;
			dist[i] = INT_MAX;
			prev[i] = -1;

		}
		dist[s] = 0;

		pq.push(Node_Dist(s, 0));
		while (pq.empty() == false)
		{
			Node_Dist minDist = pq.top();
			pq.pop();
			vis[minDist.vertexIndex] = true;
			if (dist[minDist.vertexIndex] < minDist.dist) continue;
			//relax all neighbours of minDist.vertexIndex
			for (int i = 0; i < vertexList.size(); i++)
			{
				if (adjacencyMatrix[minDist.vertexIndex][i] > -1)
				{
					int neighbour = i;
					int newDist = minDist.dist + adjacencyMatrix[minDist.vertexIndex][i] ;
					if (newDist < dist[neighbour])
					{
						prev[neighbour] = minDist.vertexIndex;
						dist[neighbour] = newDist;
						pq.push(Node_Dist(neighbour, newDist));
					}

				}
			}
		}
	}
	std::vector<int> findShortestPathLazy(int s, int e)
	{
		std::vector<int> path;
		std::shared_ptr<int[]> dist;
		std::shared_ptr<int[]> prev;
		lazyDijkstra(s, dist, prev);
		if (dist[e] != INT_MAX)
		{
			for (int at = e; at != -1; at = prev[at])
			{
				path.push_back(at);
			}
			std::reverse(path.begin(), path.end());
		}
		return path;
	}
	void eagerDijkstra(int s, std::shared_ptr<int[]>& dist, std::shared_ptr<int[]>& prev)
	{
		int n = vertexList.size();
		std::unique_ptr<bool[]> vis(new bool[n]);
		dist = std::shared_ptr<int[]>(new int[n]);
		prev = std::shared_ptr<int[]>(new int[n]);
		graph_indexed_min_priority_queue<int, intLess> ipq(vertexList.size());
		for (int i = 0; i < n; i++)
		{
			vis[i] = false;
			dist[i] = INT_MAX;
			prev[i] = -1;

		}
		dist[s] = 0;

		ipq.insert(s, 0);
		while (ipq.isEmpty() == false)
		{
			int minDistKi = ipq.peekRootKi();
			int minDist = ipq.pollRoot();
			vis[minDist.vertexIndex] = true;
			if (dist[minDistKi] < minDist) continue;
			//relax all neighbours of minDist.vertexIndex
			for (int i = 0; i < vertexList.size(); i++)
			{
				if (adjacencyMatrix[minDistKi][i] > -1)
				{
					int neighbour = i;
					int newDist = minDist + adjacencyMatrix[minDistKi][i] ;
					if (newDist < dist[neighbour])
					{
						prev[neighbour] = minDistKi;
						dist[neighbour] = newDist;
						if (ipq.contains(neighbour))
							ipq.insert(neighbour, newDist);
						else
							ipq.decreaseKey(neighbour, newDist);
					}

				}
			}
		}
	}
	std::vector<int> findShortestPathEager(int s, int e)
	{
		std::vector<int> path;
		std::shared_ptr<int[]> dist;
		std::shared_ptr<int[]> prev;
		lazyDijkstra(s, dist, prev);
		if (dist[e] != INT_MAX)
		{
			for (int at = e; at != -1; at = prev[at])
			{
				path.push_back(at);
			}
			std::reverse(path.begin(), path.end());
		}
		return path;
	}
	void bellFord(int s, std::shared_ptr<int[]>& dist, std::shared_ptr<int[]>& prev)
	{
		int V = vertexList.size();
		dist = std::shared_ptr<int[]>(new int[V]);
		prev = std::shared_ptr<int[]>(new int[V]);
		for (int i = 0; i < V; i++)
		{
			dist[i] = INT_MAX;
			prev[i] = -1;
		}
		dist[s] = 0;

		for (int i = 0; i < V - 1; i++)
		{
			for (int j = 0; j < V; j++)
			{
				for (int z = 0; z < V; z++)
				{
					if (adjacencyMatrix[j][z] > -1)
					{
						if (dist[j] != INT_MAX && dist[j] + adjacencyMatrix[j][z]  < dist[z])
						{
							prev[z] = j;
							dist[z] = dist[j] + adjacencyMatrix[j][z] ;
						}
					}
				}
			}
		}
		for (int i = 0; i < V - 1; i++)
		{
			for (int j = 0; j < V; j++)
			{
				for (int z = 0; z < V; z++)
				{
					if (adjacencyMatrix[j][z] > -1)
					{
						if (dist[j] != INT_MAX && (dist[j] == INT_MIN || dist[j] + adjacencyMatrix[j][z]  < dist[z]))
						{
							prev[z] = -1;
							dist[z] = INT_MIN;
						}
					}
				}
			}
		}
	}
	std::vector<int> findShortestPathBellFord(int s, int e)
	{
		std::vector<int> path;
		std::shared_ptr<int[]> dist;
		std::shared_ptr<int[]> prev;
		bellFord(s, dist, prev);
		if (dist[e] != INT_MAX && dist[e] != INT_MIN)
		{
			for (int at = e; at != -1; at = prev[at])
			{
				path.push_back(at);
			}
			std::reverse(path.begin(), path.end());
		}
		return path;
	}
};