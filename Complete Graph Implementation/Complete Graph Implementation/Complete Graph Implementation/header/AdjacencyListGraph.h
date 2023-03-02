#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <queue>
#include "Node_Dist_Struct.h"
#include "GraphIndexedPriorityQueue.h"
#include "UnionFind.h"

struct DirectedEdge
{
	int to;
	int cost;

	DirectedEdge(int _to, int _cost)
	{
		to = _to;
		cost = _cost;
	}
};
struct UndirectedEdge {
	int first;
	int second;
	int cost;

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
class Undirected_Adjacency_List_Graph {
public:
	int search(T value)
	{
		if (vertexList.size() != 0)
		{
			return std::find(vertexList.begin(), vertexList.end(), value) - vertexList.begin();
		}
		return -1;
	}
	void addVertex(T value)
	{
		std::vector<DirectedEdge> newVertex;
		adjacencyList.push_back(newVertex);
		vertexList.push_back(value);
	}
	void addEdge(int first, int second, int cost)
	{
		if (adjacencyList.empty() == false)
		{
			if (first >= 0 && first < adjacencyList.size() && second >= 0 && second < adjacencyList.size())
			{
				//check at first
				if (std::find_if(adjacencyList[first].begin(), adjacencyList[first].end(), [&second](DirectedEdge DirectedEdge) {
					return DirectedEdge.to == second;
					}) == adjacencyList[first].end())
				{
					DirectedEdge newEdge(second, cost);
					adjacencyList[first].push_back(newEdge);
				}

				//check at second
				if (std::find_if(adjacencyList[second].begin(), adjacencyList[second].end(), [&first](DirectedEdge DirectedEdge) {
					return DirectedEdge.to == first;
					}) == adjacencyList[second].end())
				{
					DirectedEdge newEdge(first, cost);
					adjacencyList[second].push_back(newEdge);
				}
			}
		}
	}
	void removeVertex(int vtxIndex)
	{
		if (adjacencyList.empty() == false)
		{
			if (vtxIndex >= 0 && vtxIndex < adjacencyList.size())
			{
				//Any edge with edge.to == vtxIndex is to be deleted
				for (int i = 0; i < adjacencyList.size(); i++)
				{
					if (i != vtxIndex)
					{
						while (true)
						{
							auto it = std::find_if(adjacencyList[i].begin(), adjacencyList[i].end(), [&vtxIndex](DirectedEdge edge) {
								return edge.to == vtxIndex;
								});
							if (it == adjacencyList[i].end()) break;
							adjacencyList[i].erase(it);
						}
					}
				}

				//Any edge with edge.to > vtxIndex is to be decremented by 1
				for (int i = 0; i < adjacencyList.size(); i++)
				{
					if (i != vtxIndex)
					{
						auto begin = adjacencyList[i].begin();
						auto end = adjacencyList[i].end();
						while (true)
						{
							if (begin == end) break;
							auto it = std::find_if(begin, end, [&vtxIndex](DirectedEdge edge) {
								return edge.to > vtxIndex;
								});
							if (it == adjacencyList[i].end()) break;
							it->to--;
							begin = it + 1;
						}
					}
				}

				//edge.from is not there in the edge, by deleting vector vtxIndex, we 
				//already droped edge.from > vtxIndex.
				adjacencyList.erase(adjacencyList.begin() + vtxIndex);
				vertexList.erase(vertexList.begin() + vtxIndex);
			}
		}
	}
	void removeEdge(int first, int second)
	{
		if (adjacencyList.empty() == false)
		{
			if (first < adjacencyList.size() && first >= 0 && second < adjacencyList.size() && second >= 0)
			{
				auto it = std::find_if(adjacencyList[first].begin(), adjacencyList[first].end(), [&second](DirectedEdge edge) {
					return edge.to == second;
					});
				if (it != adjacencyList[first].end()) adjacencyList[first].erase(it);

				it = std::find_if(adjacencyList[second].begin(), adjacencyList[second].end(), [&first](DirectedEdge edge) {
					return edge.to == first;
					});
				if (it != adjacencyList[second].end()) adjacencyList[second].erase(it);
			}
		}
	}
	void dfsPrint(int s)
	{
		if (adjacencyList.empty() == false)
		{
			if (s >= 0 && s < adjacencyList.size())
			{
				int n = adjacencyList.size();
				std::shared_ptr<bool[]> visited(new bool[n]{false});
				dfsPrint_rec(s, visited);
			}
		}
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
					for (int i = 0; i < adjacencyList[front].size(); i++)
					{
						if (!visited[adjacencyList[front][i].to])
						{
							q.push(adjacencyList[front][i].to);
							visited[adjacencyList[front][i].to] = true;
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
			for (DirectedEdge edge : adjacencyList[minDist.vertexIndex])
			{
				int neighbour = edge.to;
				int newDist = minDist.dist + edge.cost;
				if (newDist < dist[neighbour])
				{
					prev[neighbour] = minDist.vertexIndex;
					dist[neighbour] = newDist;
					pq.push(Node_Dist(neighbour, newDist));
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
			vis[minDistKi] = true;
			int minDist = ipq.pollRoot();
			if (dist[minDistKi] < minDist.dist) continue;
			//relax all neighbours of minDist.vertexIndex
			for (DirectedEdge edge : adjacencyList[minDistKi])
			{
				int neighbour = edge.to;
				int newDist = minDist + edge.cost;
				if (newDist < dist[neighbour])
				{
					prev[neighbour] = minDistKi;
					dist[neighbour] = newDist;
					if (!ipq.contains(neighbour))
						ipq.insert(neighbour, newDist);
					else
						ipq.decreaseKey(neighbour, newDist);
				}
			}
		}
	}
	std::vector<int> findShortestPathEager(int s, int e)
	{
		std::vector<int> path;
		std::shared_ptr<int[]> dist;
		std::shared_ptr<int[]> prev;
		eagerDijkstra(s, dist, prev);
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
				int minBeg;
				int minTo;
				for (int i = 0; i < adjacencyList.size(); i++)
				{
					for (int j = 0; j < adjacencyList[i].size(); j++)
					{
						if (std::find_if(processed.begin(), processed.end(), [&](UndirectedEdge edge) {
							return (edge.first == i && edge.second == adjacencyList[i][j].to) || 
								(edge.first == adjacencyList[i][j].to && edge.second == i);
							}) == processed.end())
						{
							if (adjacencyList[i][j].cost < currMin && adjacencyList[i][j].cost > 0)
							{
								currMin = adjacencyList[i][j].cost;
								minBeg = i;
								minTo = adjacencyList[i][j].to;
							}
						}
					}
				}
				if (!uf.connected(minBeg, minTo))
				{
					uf.unify(minBeg, minTo);
					mstEdgeList.push_back(UndirectedEdge(minBeg, minTo, currMin));
				}
				processed.push_back(UndirectedEdge(minBeg, minTo, currMin));
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
			graph_indexed_min_priority_queue<UndirectedEdge, intLess> ipq(vertexList.size());
			
			relaxEdgesAtNode(start, visited, ipq);

			while (!ipq.isEmpty() && edgeCount != m)
			{
				int destNodeIndex = ipq.peekRootKi();
				UndirectedEdge edge = ipq.pollRoot();

				mstEdge.push_back(edge);
				edgeCount++;

				relaxEdgesAtNode(destNodeIndex);
			}
			if (edgeCount != m)
				return std::vector<UndirectedEdge>();
			return mstEdge;
		}
		return std::vector<UndirectedEdge>();
	}
	Undirected_Adjacency_List_Graph<T> makeMST(std::vector<UndirectedEdge> mstEdgeList)
	{
		if (vertexList.size() > 1 && mstEdgeList.size() > 0)
		{
			Undirected_Adjacency_List_Graph<T> mst;
			mst.vertexList = std::vector<T>(vertexList);
			mst.adjacencyList.resize(vertexList.size());

			for (auto edge : mstEdgeList)
			{
				mst.adjacencyList[edge.first].push_back(DirectedEdge(edge.second, edge.cost));
				mst.adjacencyList[edge.second].push_back(DirectedEdge(edge.first, edge.cost));
			}
			return mst;
		}
		return Undirected_Adjacency_List_Graph<T>();
	}
private:
	std::vector<T> vertexList;
	std::vector<std::vector<DirectedEdge>> adjacencyList;
	void dfsPrint_rec(int at, std::shared_ptr<bool[]> visited)
	{
		if (visited[at]) return;
		visited[at] = true;
		std::cout << vertexList[at] << " ";

		//extract the neighbour
		for (DirectedEdge edge : adjacencyList[at])
		{
			dfsPrint_rec(edge.to, visited);
		}
	}
	void addEdges(int at, std::shared_ptr<bool[]> visited, std::priority_queue<UndirectedEdge, std::vector<UndirectedEdge>, unEdgeGreater>& pq)
	{
		visited[at] = true;

		for (auto& edge : adjacencyList[at])
		{
			if (visited[edge.to] != true)
				pq.push(UndirectedEdge(at, edge.to, edge.cost));
		}
	}
	void relaxEdgesAtNode(int at, std::shared_ptr<bool[]> visited, graph_indexed_min_priority_queue<UndirectedEdge, unEdgeLess>& ipq)
	{
		visited[at] = true;

		for (auto& edge : adjacencyList[at])
		{
			int neighbour = edge.to;

			if (!visited[neighbour])
			{
				if (!ipq.contains(neighbour))
					ipq.insert(neighbour, edge);
				else
					ipq.decreaseKey(neighbour, edge);
			}
		}
	}

};

template <typename T>
class Directed_Adjacency_List_Graph {
private:
	std::vector<T> vertexList;
	std::vector<std::vector<DirectedEdge>> adjacencyList;
	void dfsPrint_rec(int at, std::shared_ptr<bool[]> visited)
	{
		if (visited[at]) return;
		visited[at] = true;
		std::cout << vertexList[at] << " ";

		//extract the neighbour
		for (DirectedEdge edge : adjacencyList[at])
		{
			dfsPrint_rec(edge.to, visited);
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
	void addVertex(T value)
	{
		std::vector<DirectedEdge> newVertex;
		adjacencyList.push_back(newVertex);
		vertexList.push_back(value);
	}
	void addEdge(int begin, int end, int cost)
	{
		if (adjacencyList.empty() == false)
		{
			if (begin >= 0 && begin < adjacencyList.size() && end >= 0 && end < adjacencyList.size())
			{
				//check at first
				if (std::find_if(adjacencyList[begin].begin(), adjacencyList[begin].end(), [&end](DirectedEdge DirectedEdge) {
					return DirectedEdge.to == end;
					}) == adjacencyList[begin].end())
				{
					DirectedEdge newEdge(end, cost);
					adjacencyList[begin].push_back(newEdge);
				}
			}
		}
	}
	void removeVertex(int vtxIndex)
	{
		if (adjacencyList.empty() == false)
		{
			if (vtxIndex >= 0 && vtxIndex < adjacencyList.size())
			{
				//Any edge with edge.to == vtxIndex is to be deleted
				for (int i = 0; i < adjacencyList.size(); i++)
				{
					if (i != vtxIndex)
					{
						while (true)
						{
							auto it = std::find_if(adjacencyList[i].begin(), adjacencyList[i].end(), [&vtxIndex](DirectedEdge edge) {
								return edge.to == vtxIndex;
								});
							if (it == adjacencyList[i].end()) break;
							adjacencyList[i].erase(it);
						}
					}
				}

				//Any edge with edge.to > vtxIndex is to be decremented by 1
				for (int i = 0; i < adjacencyList.size(); i++)
				{
					if (i != vtxIndex)
					{
						auto begin = adjacencyList[i].begin();
						auto end = adjacencyList[i].end();
						while (true)
						{
							if (begin == end) break;
							auto it = std::find_if(begin, end, [&vtxIndex](DirectedEdge edge) {
								return edge.to > vtxIndex;
								});
							if (it == adjacencyList[i].end()) break;
							it->to--;
							begin = it + 1;
						}
					}
				}

				//edge.from is not there in the edge, by deleting vector vtxIndex, we 
				//already droped edge.from > vtxIndex.
				adjacencyList.erase(adjacencyList.begin() + vtxIndex);
				vertexList.erase(vertexList.begin() + vtxIndex);
			}
		}
	}
	void removeEdge(int begin, int end)
	{
		if (adjacencyList.empty() == false)
		{
			if (begin < adjacencyList.size() && begin >= 0 && end < adjacencyList.size() && end >= 0)
			{
				auto it = std::find_if(adjacencyList[begin].begin(), adjacencyList[begin].end(), [&end](DirectedEdge edge) {
					return edge.to == end;
					});
				if (it != adjacencyList[begin].end()) adjacencyList[begin].erase(it);
			}
		}
	}
	void dfsPrint(int s)
	{
		if (adjacencyList.empty() == false)
		{
			if (s >= 0 && s < adjacencyList.size())
			{
				int n = adjacencyList.size();
				std::shared_ptr<bool[]> visited(new bool[n] {false});
				dfsPrint_rec(s, visited);
			}
		}
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
					for (int i = 0; i < adjacencyList[front].size(); i++)
					{
						if (!visited[adjacencyList[front][i].to])
						{
							q.push(adjacencyList[front][i].to);
							visited[adjacencyList[front][i].to] = true;
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
			for (DirectedEdge edge : adjacencyList[minDist.vertexIndex])
			{
				int neighbour = edge.to;
				int newDist = minDist.dist + edge.cost;
				if (newDist < dist[neighbour])
				{
					prev[neighbour] = minDist.vertexIndex;
					dist[neighbour] = newDist;
					pq.push(Node_Dist(neighbour, newDist));
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
			vis[minDistKi] = true;
			int minDist = ipq.pollRoot();
			if (dist[minDistKi] < minDist.dist) continue;
			//relax all neighbours of minDist.vertexIndex
			for (DirectedEdge edge : adjacencyList[minDistKi])
			{
				int neighbour = edge.to;
				int newDist = minDist + edge.cost;
				if (newDist < dist[neighbour])
				{
					prev[neighbour] = minDistKi;
					dist[neighbour] = newDist;
					if (!ipq.contains(neighbour))
						ipq.insert(neighbour, newDist);
					else
						ipq.decreaseKey(neighbour, newDist);
				}
			}
		}
	}
	std::vector<int> findShortestPathEager(int s, int e)
	{
		std::vector<int> path;
		std::shared_ptr<int[]> dist;
		std::shared_ptr<int[]> prev;
		eagerDijkstra(s, dist, prev);
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
				for (DirectedEdge edge : adjacencyList[j])
				{
					if (dist[j] != INT_MAX && dist[j] + edge.cost < dist[edge.to])
					{
						prev[edge.to] = j;
						dist[edge.to] = dist[j] + edge.cost;
					}
				}
			}
		}
		for (int i = 0; i < V - 1; i++)
		{
			for (int j = 0; j < V; j++)
			{
				for (DirectedEdge edge : adjacencyList[j])
				{
					if (dist[j] != INT_MAX && (dist[j] == INT_MIN || dist[j] + edge.cost < dist[edge.to]))
					{
						prev[edge.to] = -1;
						dist[edge.to] = INT_MIN;
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