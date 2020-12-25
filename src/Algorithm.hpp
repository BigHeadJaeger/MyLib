#pragma once
#include<iostream>
#include<vector>
namespace Algorithm
{
	// 冒泡
	void sort1(std::vector<int>& target)
	{
		for (auto i = 0; i < target.size(); i++)
		{
			for (auto j = 0; j < target.size() - i - 1; j++)
			{
				if (target[j] > target[j + 1])
				{
					int temp = target[j];
					target[j] = target[j + 1];
					target[j + 1] = temp;
				}
			}
		}
	}

	// 插入
	void sort2(std::vector<int>& target)
	{
		for (auto i = 1; i < target.size(); i++)
		{
			for (auto j = i; j > 0; j--)
			{
				if (target[j] < target[j - 1])
				{
					int temp = target[j];
					target[j] = target[j - 1];
					target[j - 1] = temp;
				}
				else
					break;
			}
		}
	}

	// 归并
	void merge(std::vector<int>& targetArray, int start, int mid, int end)
	{
		std::vector<int> mergeRes;
		mergeRes.resize(end - start + 1, 0);

		int i = start;
		int j = mid + 1;
		int k = 0;

		while (i <= mid && j <= end)
		{
			if (targetArray[i] < targetArray[j])
			{
				mergeRes[k++] = targetArray[i];
				i++;
			}
			else
			{
				mergeRes[k++] = targetArray[j];
				j++;
			}
		}
		while (i <= mid)
		{
			mergeRes[k++] = targetArray[i];
			i++;
		}
		while (j <= end)
		{
			mergeRes[k++] = targetArray[j];
			j++;
		}

		k = 0;
		for (auto idx = start; idx <= end; idx++)
			targetArray[idx] = mergeRes[k++];
	}
	void mergeSort(std::vector<int>& targetArray, int start, int end)
	{
		if (start >= end)
			return;

		int mid = (end - start) / 2 + start;

		mergeSort(targetArray, start, mid);
		mergeSort(targetArray, mid + 1, end);
		merge(targetArray, start, mid, end);
	}
	void sort3(std::vector<int>& target)
	{
		mergeSort(target, 0, target.size() - 1);
	}

	// 快速
	int getStand(std::vector<int>& targetArray, int start, int end)
	{
		int key = targetArray[start];
		int i = start;
		int j = end;

		while (i < j)
		{
			while (i < j && targetArray[j] >= key)
			{
				j--;
			}
			if (i < j)
			{
				targetArray[i] = targetArray[j];
			}

			while (i < j && targetArray[i] <= key)
			{
				i++;
			}
			if (i < j)
			{
				targetArray[j] = targetArray[i];
			}
		}

		targetArray[i] = key;
		return i;
	}
	void quickSort(std::vector<int>& targetArray, int start, int end)
	{
		if (start < end)
		{
			int standPos = getStand(targetArray, start, end);
			quickSort(targetArray, start, standPos);
			quickSort(targetArray, standPos + 1, end);
		}
	}
	void sort4(std::vector<int>& target)
	{
		quickSort(target, 0, target.size() - 1);
	}

	// 选择
	void sort5(std::vector<int>& target)
	{
		for (int i = 0; i < target.size(); i++)
		{
			int min = target[i];
			int minIndex = i;

			for (int j = i+ 1; j < target.size(); j++)
			{
				if (target[j] < min)
				{
					min = target[j];
					minIndex = j;
				}
			}

			target[minIndex] = target[i];
			target[i] = min;
		}
	}
}

