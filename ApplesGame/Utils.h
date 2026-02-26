#pragma once
#include <vector>

namespace ApplesGame
{
	// Insertion sort: comp(a, b) returns true if a should come before b
	template<typename T, typename Comparator>
	void Sort(std::vector<T>& v, Comparator comp)
	{
		for (int i = 1; i < (int)v.size(); ++i)
		{
			T key = v[i];
			int j = i - 1;
			while (j >= 0 && comp(key, v[j]))
			{
				v[j + 1] = v[j];
				--j;
			}
			v[j + 1] = key;
		}
	}
}
