#pragma once
void minheapify(int a[], int n)
{
	if (sizeof(a) / sizeof(int) == 0 || sizeof(a) / sizeof(int) == 1) return;

	int i = n / 2 - 1;
	while (i >= 0)
	{
		int swap = 2 * i + 1;
		if (swap + 1 < n && a[swap] > a[swap + 1])
			swap += 1;
		if (a[i] > a[swap])
			std::swap(a[swap], a[i]);
		i--;
	}
	i = 1;
	while (i < n / 2 - 1)
	{
		int swap = 2 * i + 1;
		if (swap + 1 < n && a[swap] > a[swap + 1])
		{
			swap += 1;
		}
		if (a[i] > a[swap])
			std::swap(a[swap], a[i]);
		i++;
	}
}
void maxheapify(int a[], int n)
{
	if (sizeof(a) / sizeof(int) == 0 || sizeof(a) / sizeof(int) == 1) return;

	int i = n / 2 - 1;
	while (i >= 0)
	{
		int swap = 2 * i + 1;
		if (swap + 1 < n && a[swap] < a[swap + 1])
			swap += 1;
		if (a[i] < a[swap])
			std::swap(a[swap], a[i]);
		i--;
	}
	i = 1;
	while (i < n / 2 - 1)
	{
		int swap = 2 * i + 1;
		if (swap + 1 < n && a[swap] < a[swap + 1])
			swap += 1;
		if (a[i] < a[swap])
			std::swap(a[swap], a[i]);
		i++;
	}
}
void dheapSort(int a[], int n)
{
	if (sizeof(a) / sizeof(int) == 0 || sizeof(a) / sizeof(int) == 1) return;

	minheapify(a, n);
	while (n >= 2)
	{
		std::swap(a[0], a[n - 1]);
		n--;
		minheapify(a, n);
	}
}
void aheapSort(int a[], int n)
{
	if (sizeof(a) / sizeof(int) == 0 || sizeof(a) / sizeof(int) == 1) return;

	maxheapify(a, n);
	while (n >= 2)
	{
		std::swap(a[0], a[n - 1]);
		n--;
		maxheapify(a, n);
	}
}