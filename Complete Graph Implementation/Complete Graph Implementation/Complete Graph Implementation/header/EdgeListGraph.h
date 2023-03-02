#pragma once
#include <vector>
#include <queue>
#include "GraphIndexedPriorityQueue.h"
#include <algorithm>
#include <memory>
#include "UnionFind.h"
#include "Node_Dist_Struct.h"
#include <iostream>

struct UndirectedEdge
{
	int first;
	int second;
	int cost;

	UndirectedEdge(){}
	UndirectedEdge(int _first, int _second, int _cost) {
		first = _first;
		second = _second;
		cost = _cost;
	}
};
struct DirectedEdge
{
	int begin;
	int end;
	int cost;

	DirectedEdge(int _begin, int _end, int _cost) {
		begin = _begin;
		end = _end;
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
class Undirected_Edge_List_Graph
{
private:
	std::vector<T> vertexList;
	std::vector<UndirectedEdge> edgeList;
	void addEdges(int start, std::shared_ptr<bool[]> visited, std::priority_queue<UndirectedEdge, std::vector<UndirectedEdge>, unEdgeGreater>& pq)
	{
		visited[start] = true;
		auto begin = edgeList.begin();
		while (begin != edgeList.end())
		{
			std::vector<UndirectedEdge>::iterator toNeighbour = std::find_if(begin, edgeList.end(), [&](UndirectedEdge edge) {
				return edge.first == start || edge.second == start;
				});
			if (toNeighbour == edgeList.end())
				break;
			int neighbour = (toNeighbour->first == start) ? toNeighbour->second : toNeighbour->first;
			if (!visited[neighbour])
			{
				pq.push(*toNeighbour);
			}
			begin = toNeighbour + 1;
		}

	}
	void relaxEdgesAtNode(int at, std::shared_ptr<bool[]> visited, graph_indexed_min_priority_queue<UndirectedEdge, unEdgeLess>& ipq)
	{
		visited[at] = true;

		auto begin = edgeList.begin();
		while (begin != edgeList.end())
		{
			std::vector<UndirectedEdge>::iterator toNeighbour = std::find_if(begin, edgeList.end(), [&](UndirectedEdge edge) {
				return edge.first == at || edge.second == at;
				});
			if (toNeighbour == edgeList.end())
				break;
			int neighbour = (toNeighbour->first == at) ? toNeighbour->second : toNeighbour->first;
			if (!visited[neighbour])
			{
				if (!ipq.contains(neighbour))
					ipq.insert(neighbour, *toNeighbour);
				else
					ipq.decreaseKey(neighbour, *toNeighbour);
			}
			begin = toNeighbour + 1;
		}
	}
	void dfsPrint_rec(int s, std::shared_ptr<bool[]> visited)
	{
		if (visited[s]) return;
		visited[s] = true;
		std::cout << vertexList[s] << " ";

		//find the edge with s and the neighbour, do dfs on the neighbour when edge is found
		std::vector<UndirectedEdge>::iterator begin = edgeList.begin();
		std::vector<UndirectedEdge>::iterator end = edgeList.end();
		while (true)	
		{
			if (begin == end)
			{
				return;
			}
			std::vector<UndirectedEdge>::iterator it = std::find_if(begin, end, [&s](UndirectedEdge edge) {
				return edge.first == s || edge.second == s;
			});
			if (it == end)
			{
				return;
			}
			//do dfs on current edge's other vertex that it points to
			int neighbour = (it->first == s) ? it->second : it->first;
			dfsPrint_rec(neighbour, visited);
			begin = it + 1;
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
		vertexList.push_back(value);
	}
	void removeVertex(int vtxIndex)
	{
		if (vertexList.empty() == false)
		{
			if (vtxIndex >= 0 && vtxIndex < vertexList.size())
			{
				//since what the edges are linked to are not actually there after we remove vertex at vtxIndex, 
				//we must shift those up an index to match the change in vertexList
				while (true)
				{
					std::vector<UndirectedEdge>::iterator it = std::find_if(edgeList.begin(), edgeList.end(), [&vtxIndex](UndirectedEdge edge) {
						return edge.first == vtxIndex || edge.second == vtxIndex;
						});
					if (it == edgeList.end()) break;
					edgeList.erase(it);
				}
				auto begin = edgeList.begin();
				auto end = edgeList.end();
				while (true)
				{
					if (begin == end) break;
					std::vector<UndirectedEdge>::iterator it = std::find_if(begin, edgeList.end(), [&vtxIndex](UndirectedEdge edge) {
						return edge.first > vtxIndex || edge.second > vtxIndex;
						});
					if (it == edgeList.end()) break;
					if (it->first > vtxIndex) it->first--;
					if (it->second > vtxIndex) it->second--;
					begin = it + 1;
				}

				vertexList.erase(vertexList.begin() + vtxIndex);

			}
		}
	}
	void addEdge(int first, int second, int cost)
	{
		if (!vertexList.empty())
		{
			if (first >= 0 && first < vertexList.size() && second >= 0 && second < vertexList.size())
			{
				if (std::find_if(edgeList.begin(), edgeList.end(), [&cost, &first, &second](const UndirectedEdge& edge) {
					return ((edge.first == first && edge.second == second) || (edge.second == first && edge.first == second)) && edge.cost == cost;
					}) == edgeList.end())
				{
					UndirectedEdge target(first, second, cost);
					edgeList.push_back(target);
				}
			}
		}
	}
	void removeEdge(int first, int second, int cost)
	{
		if (!edgeList.empty())
		{
			auto it = std::find_if(edgeList.begin(), edgeList.end(), [](const UndirectedEdge& edge) {
				return ((edge.first == first && edge.second == second) || (edge.second == first && edge.first == second)) && edge.cost == cost;
				});
			if (it != edgeList.end())
			{
				edgeList.erase(it);
			}
		}
	}
	void dfsPrint(int s)
	{
		if (!vertexList.empty())
		{
			if (vertexList.size() == 1)
			{
				if (s == 0)
				{
					std::cout << vertexList[0] << " ";
				}
			}
			else
			{
				if (s < 0 || s >= vertexList.size())
				{
					return;
				}
				int n = vertexList.size();
				std::shared_ptr<bool[]> visited(new bool[n] {false}) ;
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
					auto begin = edgeList.begin();
					auto end = edgeList.end();
					while (true)
					{
						if (begin == end) break;
						std::vector<UndirectedEdge>::iterator it = std::find_if(begin, end, [&front](UndirectedEdge edge) {
							return edge.first == front || edge.second == front;
							});
						if (it == edgeList.end()) break;
						if (it->first == front && !visited[it->second])
						{
							q.push(it->second);
							visited[it->second] = true;
						}
						if (it->second == front && !visited[it->first])
						{
							q.push(it->first);
							visited[it->first] = true;
						}
						begin = it + 1;
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
			vis[minDistKi] = true;
			if (dist[minDistKi] < minDist) continue;
			//relax all neighbours of minDistKi
			std::vector<UndirectedEdge>::iterator edgeToNeighbour;
			std::vector<UndirectedEdge>::iterator begin = edgeList.begin();
			while (begin != edgeList.end())
			{
				edgeToNeighbour = std::find_if(begin, edgeList.end(), [&minDist](UndirectedEdge edge) {
					return edge.first == minDistKi || edge.second == minDistKi;
					});
				if (edgeToNeighbour == edgeList.end()) break;
				int neighbour = (edgeToNeighbour->first == minDistKi) ? edgeToNeighbour->second : edgeToNeighbour->first;
				//if (!vis[neighbour]) {
					int newDist = minDist + edgeToNeighbour->cost;
					if (newDist < dist[neighbour])
					{
						prev[neighbour] = minDistKi;
						dist[neighbour] = newDist;
						pq.push(Node_Dist(neighbour, newDist));
					}
				//}
				//shorten seeking range
				begin = edgeToNeighbour + 1;
			}
		}
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
			if (minDistKi == -1) break;
			vis[minDistKi] = true;
			int minDist = ipq.pollRoot();
			if (dist[minDistKi] < minDist) continue;
			//relax all neighbours of minDistKi
			std::vector<UndirectedEdge>::iterator edgeToNeighbour;
			std::vector<UndirectedEdge>::iterator begin = edgeList.begin();
			while (begin != edgeList.end())
			{
				edgeToNeighbour = std::find_if(begin, edgeList.end(), [&](UndirectedEdge edge) {
					return edge.first == minDistKi || edge.second == minDistKi;
					});
				if (edgeToNeighbour == edgeList.end()) break;
				int neighbour = (edgeToNeighbour->first == minDistKi) ? edgeToNeighbour->second : edgeToNeighbour->first;
				int newDist = minDist + edgeToNeighbour->cost;
				if (newDist < dist[neighbour])
				{
					prev[neighbour] = minDistKi;
					dist[neighbour] = newDist;
					if (!ipq.contains(neighbour))
						ipq.insert(neighbour, newDist);
					else
						ipq.decreaseKey(neighbour, newDist);
				}
				begin = edgeToNeighbour + 1;
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
			if (edgeList.size() > 1)
			{
				UnionFind uf(vertexList.size());
				std::vector<UndirectedEdge> mstEdgeList;
				std::sort(edgeList.begin(), edgeList.end(), [](UndirectedEdge a, UndirectedEdge b) {
					return a.cost < b.cost;
					});
				for (UndirectedEdge& edge : edgeList)
				{
					if (uf.getNumOfComponents() == 1) break;
					if (!uf.connected(edge.first, edge.second))
					{
						uf.unify(edge.first, edge.second);
						mstEdgeList.push_back(edge);
					}
				}
				return mstEdgeList;
			}
		}
		return std::vector<UndirectedEdge>();
	}
	Undirected_Edge_List_Graph makeMST(std::vector<UndirectedEdge> mstEdgeList)
	{
		if (vertexList.size() > 1 && mstEdgeList.size() > 0)
		{
			Undirected_Edge_List_Graph<T> mst;
			mst.vertexList = std::vector<T>(vertexList);
			mst.edgeList = std::vector<UndirectedEdge>(mstEdgeList);	
			return mst;
		}
		return Undirected_Edge_List_Graph();
	}
	std::vector<UndirectedEdge> primLazy(int start)
	{
		if (vertexList.size() > 1 && edgeList.size() > 0)
		{
			int m = vertexList.size() - 1;
			int edgeCount = 0;
			std::priority_queue<UndirectedEdge, std::vector<UndirectedEdge>, unEdgeGreater> pq;
			std::vector<UndirectedEdge> mstEdgeList;
			std::shared_ptr<bool[]> visited(new bool[vertexList.size()]);
			for (int i = 0; i < vertexList.size(); i++)
				visited[i] = false;
			addEdges(start, visited, pq);

			while (!pq.empty() && edgeCount != m)
			{
				UndirectedEdge toNeighbour = pq.top();
				pq.pop();

				int neighbour = (visited[toNeighbour.first] == true) ? toNeighbour.second : toNeighbour.first; 

				if (visited[neighbour])
					continue;

				mstEdgeList.push_back(toNeighbour);
				edgeCount++;

				addEdges(neighbour, visited, pq);

			}
			if (edgeCount != m)
				return std::vector<UndirectedEdge>();

			return mstEdgeList;
		}
	}
	std::vector<UndirectedEdge> primEager(int start)
	{
		if (vertexList.size() > 1 && edgeList.size() > 0)
		{
			int m = vertexList.size() - 1;
			int edgeCount = 0;
			graph_indexed_min_priority_queue<UndirectedEdge, unEdgeLess> ipq(vertexList.size());
			std::vector<UndirectedEdge> mstEdgeList;
			std::shared_ptr<bool[]> visited(new bool[vertexList.size()]);
			for (int i = 0; i < vertexList.size(); i++)
				visited[i] = false;
			relaxEdgesAtNode(start, visited, ipq);

			while (!ipq.isEmpty() && edgeCount != m)
			{
				UndirectedEdge toNeighbour = ipq.pollRoot();

				int neighbour = (visited[toNeighbour.first] == true) ? toNeighbour.second : toNeighbour.first;

				if (visited[neighbour])
					continue;

				mstEdgeList.push_back(toNeighbour);
				edgeCount++;

				relaxEdgesAtNode(neighbour, visited, ipq);

			}
			if (edgeCount != m)
				return std::vector<UndirectedEdge>();

			return mstEdgeList;
		}
	}

};

template <typename T>
class Directed_Edge_List_Graph
{
private:
	std::vector<T> vertexList;
	std::vector<DirectedEdge> edgeList;
	void dfsPrint_rec(int s, std::shared_ptr<bool[]> visited)
	{
		if (visited[s]) return;
		visited[s] = true;
		std::cout << vertexList[s] << " ";

		//find the edge with s and the neighbour, do dfs on the neighbour when edge is found
		std::vector<DirectedEdge>::iterator findbegin = edgeList.begin();
		std::vector<DirectedEdge>::iterator findend = edgeList.end();
		while (true)
		{
			if (findbegin == findend)
			{
				return;
			}
			std::vector<DirectedEdge>::iterator it = std::find_if(findbegin, findend, [&s](DirectedEdge edge) {
				return edge.begin == s;
				});
			if (it == findend)
			{
				return;
			}
			//do dfs on current edge's other vertex that it points to
			int neighbour = it->end;
			dfsPrint_rec(neighbour, visited);
			findbegin = it + 1;
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
		vertexList.push_back(value);
	}
	void removeVertex(int vtxIndex)
	{
		if (vertexList.empty() == false)
		{
			if (vtxIndex >= 0 && vtxIndex < vertexList.size())
			{
				//since what the edges are linked to are not actually there after we remove vertex at vtxIndex, 
				//we must shift those up an index to match the change in vertexList
				while (true)
				{
					std::vector<DirectedEdge>::iterator it = std::find_if(edgeList.begin(), edgeList.end(), [&vtxIndex](DirectedEdge edge) {
						return edge.begin == vtxIndex || edge.end == vtxIndex;
						});
					if (it == edgeList.end()) break;
					edgeList.erase(it);
				}
				auto begin = edgeList.begin();
				auto end = edgeList.end();
				while (true)
				{
					if (begin == end) break;
					std::vector<DirectedEdge>::iterator it = std::find_if(begin, edgeList.end(), [&vtxIndex](DirectedEdge edge) {
						return edge.begin > vtxIndex || edge.end > vtxIndex;
						});
					if (it == edgeList.end()) break;
					if (it->begin > vtxIndex) it->begin--;
					if (it->end > vtxIndex) it->end--;
					begin = it + 1;
				}

				vertexList.erase(vertexList.begin() + vtxIndex);

			}
		}
	}
	void addEdge(int begin, int end, int cost)
	{
		if (!vertexList.empty())
		{
			if (begin >= 0 && begin < vertexList.size() && end >= 0 && end < vertexList.size())
			{
				if (std::find_if(edgeList.begin(), edgeList.end(), [&cost, &begin, &end](const DirectedEdge& edge) {
					return (edge.begin == begin && edge.end == end) && edge.cost == cost;
					}) == edgeList.end())
				{
					DirectedEdge target(begin, end, cost);
					edgeList.push_back(target);
				}
			}
		}
	}
	void removeEdge(int begin, int end, int cost)
	{
		if (!edgeList.empty())
		{
			auto it = std::find_if(edgeList.begin(), edgeList.end(), [&cost, &begin, &end](DirectedEdge& edge) {
				return (edge.begin == begin && edge.end == end) && edge.cost == cost;
				});
			if (it != edgeList.end())
			{
				edgeList.erase(it);
			}
		}
	}
	void dfsPrint(int s)
	{
		if (!vertexList.empty())
		{
			if (vertexList.size() == 1)
			{
				if (s == 0)
				{
					std::cout << vertexList[0] << " ";
				}
			}
			else
			{
				if (s < 0 || s >= vertexList.size())
				{
					return;
				}
				int n = vertexList.size();
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
					auto begin = edgeList.begin();
					auto end = edgeList.end();
					while (true)
					{
						if (begin == end) break;
						std::vector<DirectedEdge>::iterator it = std::find_if(begin, end, [&front](DirectedEdge edge) {
							return edge.begin == front;
							});
						if (it == edgeList.end()) break;
						if (!visited[it->end])
						{
							q.push(it->end);
							visited[it->end] = true;

						}
						
						begin = it + 1;
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
			vis[minDistKi] = true;
			if (dist[minDistKi] < minDist) continue;
			//relax all neighbours of minDistKi
			std::vector<DirectedEdge>::iterator edgeToNeighbour;
			std::vector<DirectedEdge>::iterator begin = edgeList.begin();
			while (begin != edgeList.end())
			{
				edgeToNeighbour = std::find_if(begin, edgeList.end(), [&minDist](DirectedEdge edge) {
					return edge.begin == minDistKi;
					});
				if (edgeToNeighbour == edgeList.end()) break;
				int neighbour = edgeToNeighbour->end;
				int newDist = minDist + edgeToNeighbour->cost;
				if (newDist < dist[neighbour])
				{
					prev[neighbour] = minDistKi;
					dist[neighbour] = newDist;
					pq.push(Node_Dist(neighbour, newDist));
				}
				//shorten seeking range
				begin = edgeToNeighbour + 1;
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
			if (dist[minDistKi] < minDist) continue;
			//relax all neighbours of minDistKi
			std::vector<DirectedEdge>::iterator edgeToNeighbour;
			std::vector<DirectedEdge>::iterator begin = edgeList.begin();
			while (begin != edgeList.end())
			{
				edgeToNeighbour = std::find_if(begin, edgeList.end(), [&minDist](DirectedEdge edge) {
					return edge.begin == minDistKi;
					});
				if (edgeToNeighbour == edgeList.end()) break;
				int neighbour = edgeToNeighbour->end;
				int newDist = minDist + edgeToNeighbour->cost;
				if (newDist < dist[neighbour])
				{
					prev[neighbour] = minDistKi;
					dist[neighbour] = newDist;
					if (!ipq.contains(neighbour))
						ipq.insert(neighbour, newDist);
					else
						ipq.decreaseKey(neighbour, newDist);
				}
				//shorten seeking range
				begin = edgeToNeighbour + 1;
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
			for (DirectedEdge edge : edgeList)
			{
				if (dist[edge.begin] != INT_MAX)
				{
					if (dist[edge.begin] + edge.cost < dist[edge.end])
					{
						prev[edge.end] = edge.begin;
						dist[edge.end] = dist[edge.begin] + edge.cost;
					}

				}
			}
		}
		for (int i = 0; i < V - 1; i++)
		{
			for (DirectedEdge edge : edgeList)
			{
				if (dist[edge.begin] != INT_MAX)
				{
					if (dist[edge.begin] == INT_MIN || dist[edge.begin] + edge.cost < dist[edge.end])
					{
						prev[edge.end] = -1;
						dist[edge.end] = INT_MIN;
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