#pragma once
struct Node
{
	int data;
	Node* next;
	
	Node(int val)
	{
		data = val;
		next = nullptr;
	}
};