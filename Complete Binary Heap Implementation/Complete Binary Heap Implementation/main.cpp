#include <iostream>
#include <algorithm>
#include <HeapSort.h>
//#include <CompleteBinaryHeapImp.h>
using namespace std;


int main()
{
	int a[] = { 7, 2, 5, 9, 1 };
	aheapSort(a, sizeof(a) / sizeof(int));
	for (int i = 0; i < sizeof(a) / sizeof(int); i++)
	{
		std::cout << a[i] << " "; 
	}

	cin.get();
}