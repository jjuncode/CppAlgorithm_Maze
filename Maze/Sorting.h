#include "pch.h"
#include <iostream>
#include <vector>

inline void BubbleSort(std::vector<int>& arr)
{
	for (int i = 0; i < arr.size() - 1; ++i)
	{
		for (int j = 0; j < arr.size() - 1 - i; ++j)
		{
			if (arr[j] < arr[j + 1])
			{
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

inline void SelectionSort(std::vector<int>& arr)
{
	int process_idx{};
	for (int j = 0; arr.size() / 2; ++j) {
		if (process_idx >= arr.size() / 2) 
			break;

		int32 min_idx{};
		int32 max_idx{};

		int32 min{ INT32_MAX };
		int32 max{ INT32_MIN };

		for (int32 i = process_idx; i < arr.size()- process_idx; ++i)
		{
			if (arr[i] < min)
			{
				min = arr[i];
				min_idx = i;
			}

			if (arr[i] > max)
			{
				max = arr[i];
				max_idx = i;
			}
		}

		int temp = arr[process_idx];
		arr[process_idx] = min;
		arr[min_idx] = temp;

		temp = arr[arr.size() - 1 - process_idx];
		arr[arr.size() - 1 - process_idx] = max;
		arr[max_idx] = temp;

		++process_idx;
	}
}

inline void InsertionSort(vector<int>& v)
{
	int32 process_idx{};

	while (true)
	{
		for (int32 i = 0; i < process_idx; ++i)
		{
			if (v[i] > v[process_idx])
			{
				int temp = v[i];
				v[i] = v[process_idx];
				v[process_idx] = temp;
			}

		}

		++process_idx;
		if (process_idx >= v.size())
			break;
	}
}

inline void HeapSort(vector<int>& v)
{
	// Èü ÀÌ¿כ
}

inline void MergeResult(vector<int>& v, int left,int mid, int right)
{

	int32 leftIdx =left;
	int32 rightIdx = mid + 1;
	vector<int> temp;

	while (leftIdx <= mid && rightIdx <= right)
	{
		if (v[leftIdx] <= v[rightIdx])
		{
			temp.push_back(v[leftIdx]);
			++leftIdx;
		}
		else
		{
			temp.push_back(v[rightIdx]);
			++rightIdx;
		}
	}

	if (leftIdx > mid)
	{
		while (rightIdx <= right)
		{
			temp.push_back(v[rightIdx]);
			++rightIdx;
		}
	}
	else
	{
		while (leftIdx <= mid)
		{
			temp.push_back(v[leftIdx]);
			++leftIdx;
		}
	}

	for (int i = 0; i < temp.size(); ++i)
		v[left + i] = temp[i];

}


inline void MergeSort(vector<int>& v, int s, int e)
{
	if (s >= e)
		return;

	int32 mid = (s + e) / 2;
	MergeSort(v, s, mid);
	MergeSort(v, mid + 1, e);

	MergeResult(v, s, mid, e);
}