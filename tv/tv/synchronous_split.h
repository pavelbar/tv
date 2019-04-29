#pragma once
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

//способ 1

template<class T>
vector<vector<T>> SynchronousMethod(const vector<T>& v, const T& step) {
	vector<vector<T>> result;
	auto slow_car = v.begin();
	auto current = v.begin()++;
	auto next_slow_car = v.begin();

	while (current + 1 != v.end()) {
		current++;
		if (step > accumulate(slow_car + 1, current, 0)) {
			continue;
		} else {
			next_slow_car = current;
			vector<T> tmp;
			tmp.insert(tmp.end(), slow_car, next_slow_car);
			result.push_back(tmp);
			slow_car = next_slow_car;
		}
	}
	next_slow_car = v.end();
	vector<T> tmp;
	tmp.insert(tmp.end(), slow_car, next_slow_car);
	result.push_back(tmp);
	slow_car = next_slow_car;

	return result;
}
