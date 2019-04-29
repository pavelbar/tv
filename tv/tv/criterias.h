#pragma once
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

using namespace std;

template<class T, class P>
pair<bool,P> WallisMoore(const vector<T>& v, const P& param) {
	size_t n = v.size();

	if (n == 1) {
		return {true,0};
	}

	if (n == 0) {//паша
		return { false,0 };//паша
	}//паша

	// Для всех элементов вектора вычисляем знак разности соседних элементов X_i+1 - X_i
	// Определяем число фаз - подряд идущие знаки составляют одну фазу
	int counter = 0;

	int prev = 0;
	int current = 0;
	for (size_t i = 0; i < n - 1; ++i) {
		if (v[i + 1] - v[i] > 0) {
			current = 1;
		} else if (v[i + 1] - v[i]) {
			current = -1;
		} else {
			continue;
		}
		// Если соседние знаки не совпадают, наступает новая фаза
		if (current != prev) {
			counter++;
		}
		prev = current;
	}
	// Число фаз cчитается без первой и последней
	//cout << endl << "->DBG WallisMoore: number of phases + 2: " << counter << endl;//Паша
	counter -= 2;

	// Вычисляем результат по формуле из лекций
	P z;
	if (n > 30) {
		z = (abs(counter - (2 * static_cast<P>(n) - 7) / 3))
				/ (sqrt((16 * static_cast<P>(n) - 29) / 90));
	} else {
		z = (abs(counter - (2 * static_cast<P>(n) - 7) / 3) - 0.5)
				/ (sqrt((16 * static_cast<P>(n) - 29) / 90));
	}

	if (abs(z) > param) {
		return {false,z};
	} else {
		return {true,z};
	}
}

template<class T, class P>
pair<bool,double> InversionTest(vector<T> v, const P& param) {
	// Построим вариационный ряд и посчитаем число инверсий. Используем пузырьковую сортировку
	// Число инверсий будет совпадать с количеством сравнений в данной сортировке
	size_t n = v.size();

	if (n == 1) {
		return {true,0};
	}

	if (n == 0) {//паша
		return { false,0 };//паша
	}//паша

	int counter = 0;

	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < n - 1; j++) {
			if (v[j] > v[j + 1]) {
				counter++;
				T b = v[j];
				v[j] = v[j + 1];
				v[j + 1] = b;
			}
		}
	}

	//cout << endl << "->DBG InversionTest: number of inversions: " << counter << endl;//Паша

	// Вычисляем результат по формуле из лекций
	P z;
	z = abs(counter - static_cast<P>(n) * (static_cast<P>(n) - 1) / 4) * 6
			/ pow(static_cast<P>(n), 1.5);

	if (z > param) {
		return {false, z};
	} else {
		return {true, z};
	}
}
