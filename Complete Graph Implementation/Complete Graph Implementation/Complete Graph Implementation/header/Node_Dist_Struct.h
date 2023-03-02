#pragma once
struct Node_Dist
{
	int vertexIndex;
	int dist;
	Node_Dist(int _vertexIndex, int _dist)
	{
		vertexIndex = _vertexIndex;
		dist = _dist;
	}
};
class Node_Dist_Less {
public:
	bool operator()(const Node_Dist& a, const Node_Dist& b)
	{
		return a.dist < b.dist;
	}
};