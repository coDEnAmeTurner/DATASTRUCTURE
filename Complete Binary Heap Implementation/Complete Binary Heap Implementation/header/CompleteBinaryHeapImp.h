#pragma once
//This is a Min Heap
#include <algorithm>
#include <Node.h>
#include <memory>

class BinMinHeap
{
private:
	static const int Capacity = 100;
	int size;
	int array[Capacity]{ INT_MIN };
private:
	int getParentIndex(int i) {
		return (i - 2) / 2;
	}
	int getLeftChildIndex(int i) {
		return 2 * i + 1;
	}
	int getRightChildIndex(int i) {
		return 2 * i + 2;
	}
	bool hasParent(int i) {
		if (getParentIndex(i) < 0) {
			return false;
		}
		return true;
	}
	bool hasLeftChild(int i) {
		if (getLeftChildIndex(i) < size)
		{
			return true;
		}
		return false;
	}
	bool hasRightChild(int i) {
		if (getRightChildIndex(i) < size)
		{
			return true;
		}
		return false;
	}
	void heapifyUp() {
		int currentNodeIndex = size - 1;
		while (hasParent(currentNodeIndex) && array[getParentIndex(currentNodeIndex)] > array[currentNodeIndex])
		{
			std::swap(array[getParentIndex(currentNodeIndex)], array[currentNodeIndex]);
			currentNodeIndex = getParentIndex(currentNodeIndex);

		}
	}
	void heapifyDown() {
		int currentNodeIndex = 0;
		while (hasLeftChild(currentNodeIndex))
		{
			int smallerChildIndex = getLeftChildIndex(currentNodeIndex);
			if (hasRightChild(currentNodeIndex) && array[getRightChildIndex(currentNodeIndex)] < array[smallerChildIndex])
			{
				smallerChildIndex = getRightChildIndex(currentNodeIndex);
			}

			if (array[currentNodeIndex] < array[smallerChildIndex])
			{
				break;
			}
			else
			{
				std::swap(array[currentNodeIndex], array[smallerChildIndex]);
			}
			currentNodeIndex = smallerChildIndex;
		}
	}
public:
	BinMinHeap() {
		size = 0;
	}
	bool is_empty()
	{
		if (size == 0)
			return true;
		return false;
	}
	int peek()
	{
		return array[0];
	}
	void insert(int x)
	{
		if (size == Capacity)
		{
			return;
		}
		array[size++] = x;
		heapifyUp();
	}
	void removeMin()
	{
		if (size == 0)
		{
			return;
		}
		array[0] = array[size - 1];
		size--;
		heapifyDown();
	}
	void print()
	{
		if (size == 0)
		{
			return;
		}
		for (int i = 0; i < size; i++)
		{
			std::cout << array[i] << " ";
		}
	}
};

void aHeapSort(int* list, int n)
{
	if (n == 0 || n == 1)
	{
		return;
	}

	BinMinHeap heap;
	std::unique_ptr<int[]> sorted(new int[n]);

	for (int i = 0; i < n; i++)
	{
		heap.insert(list[i]);
	}

	int sortedSize = 0;
	while (!heap.is_empty())
	{
		sorted[sortedSize++] = heap.peek();
		heap.removeMin();
	}

	for (int i = 0; i < n; i++)
	{
		list[i] = sorted[i];
	}
}
void aHeapSort(Node*& head)
{
	if (head == nullptr || head->next == NULL)
	{
		return;
	}

	BinMinHeap heap;
	Node* temp = head;
	Node* prev = NULL;
	while (temp)
	{
		heap.insert(temp->data);
		prev = temp;
		temp = temp->next;
		delete prev;
		prev = NULL;
	}

	head = new Node(heap.peek());
	heap.removeMin();
	temp = head;
	while (!heap.is_empty())
	{
		temp->next = new Node(heap.peek());
		heap.removeMin();
		temp = temp->next;
	}

}
