#pragma once
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

template<class T, class P>
bool PhasedSplit(vector<vector<T>>& v, const T& h1, const P& d) {
	vector<vector<T>> result;
	for(size_t i = 0; i < v.size() - 1; ++i) {
		if((*v[i+1].begin() < h1) && (static_cast<P>(v[i].size()) <= d) && (static_cast<P>(v[i+1].size()) <= d)) {
			v[i].insert(v[i].end(), v[i+1].begin(), v[i+1].end());
			v.erase(v.begin()+i+1);
			return true;
		}
	}

	return false;
}
