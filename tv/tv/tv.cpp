#include "stdafx.h"

#include <iostream>
#include <cstdint>
#include <fstream>

#include "criterias.h"
#include "synchronous_split.h"
#include "phased_split.h"
#include <string>

char check(vector<double>& input_intervals)// Зачем нужна написано ниже //Паша
{
	if (std::is_sorted(input_intervals.begin(), input_intervals.end()))//Паша
	{//Паша
		return 't';//Паша
	}//Паша
	else {//Паша
		return 'i';//Паша
	}//Паша
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	// В данном файле представлены интервалы
	//fstream in("CoalMineDisasters.txt");
	//fstream in("NervousImpulses.txt");
	//fstream in("Aircraft2.txt");
	//fstream in("Transport.txt");
	fstream in("CompFailures.txt");

	//vector<double> input_intervals = {6, 1, 3, 9, 5, 7, 2, 8, 1, 9, 5, 4, 1, 8, 5, 4, 6, 5, 4, 7};
	vector<double> input_intervals;
	string line;

	while (getline(in, line)) {
		double d = atof(line.c_str());
		input_intervals.push_back(d);
	}

	char type = check(input_intervals);//Паша
	cout << endl << " ->input type: " << type;//Паша

	if (type == 't') {//Паша
		cout << endl << "transform ";//Паша
		vector<double> tmp;//Паша
		for (size_t i = 0; i < input_intervals.size() - 1; ++i) {
			tmp.push_back(input_intervals[i + 1] - input_intervals[i]);//Паша
		}
		input_intervals = tmp;//Паша
		cout << endl << " ->input type: i" << endl << endl;//Паша
	}

	// Проверим, являются ли данные н.о.р. с.в с помощью гипотез случайности
	// 1ый - "Фазово-частотный критерий Валлиса-Мура"
	// Параметр задает область принятия гипотезы |area| < param
	// Параметр 1.96 соответствует пороговум значению 0.05%
	double param = 1.96;
	pair<bool, double> wm = WallisMoore(input_intervals, param);

	// 2ой - "Инверсионный критерий"
	pair<bool, double> inv = InversionTest(input_intervals, param);

	// Если хотя бы один критерий не выполнился, поток считается нерекуррентным
	if (wm.first && inv.first) {
		cout << "Stream is recurrent" << endl;
		cout << "WallisMoore's result: " << wm.second << endl;
		cout << "InversionTest's result: " << inv.second << endl;
	}
	else {
		cout << "Stream isn't recurrent" << endl;
		cout << "WallisMoore's result: " << wm.second << endl;
		cout << "InversionTest's result: " << inv.second << endl;
	}

	wm.first = false;
	inv.first = false;
	// Если потоки оказались нерекуррентными, выделяем среди них пачки.
	// Сами пачки, и заявки в каждой пачке должны быть рекуррентным потоками

	cout << "----------------------------------------" << endl;
	// 1ый способ разбиения на пачки - "Синхронный"
	bool flag = true;
	// В качестве начального параметра h примем минимальный интервал
	//double h = *min_element(input_intervals.begin(), input_intervals.end());
	double h = 10;
	// В качестве контейнера используется вектор векторов,
	// где каждый вектор содержит новую пачку
	vector<vector<double>> packs_of_cars;

	while (flag) {
		//cout << h << endl;
		ofstream out("firs.txt");
		out << "----------------------------------------" << "\n";
		out << "New iteration" << "\n";
		out << "h = " << h << "\n";
		packs_of_cars = SynchronousMethod(input_intervals, h);
		out << "Number of packs = " << packs_of_cars.size() << "\n";
		out << "\n";

		for (const auto& item : packs_of_cars) {
			out << "Slow car is " << item.at(0) << "\n";
			out << "Cars in pack: ";
			for (const auto& i : item) {
				out << i << " ";
			}
			out << "\n";
			out << "Size pack = " << item.size();
			out << "\n";
			out << "\n";
		}

		// Медленные машины - первый элемент в пачке
		vector<double> tmp;

		for (size_t i = 0; i < packs_of_cars.size() - 1; ++i) {
			if (i == 0) {
				tmp.push_back(*packs_of_cars[i].begin());
			}
			else {
				tmp.push_back(
					accumulate(packs_of_cars[i - 1].begin(),
						packs_of_cars[i - 1].end(),
						*packs_of_cars[i].begin()));
			}
		}

		//		for (const auto& item : packs_of_cars) {
		////			tmp.push_back(item.at(0));
		////		}

		// Проверяем критерии для медленных машин
		//cout << endl << "START";
		//for (size_t i = 0; i < tmp.size() - 1; i++) {
		//	cout << " "<< packs_of_cars[i].size();
		//}
		pair<bool, double> one = WallisMoore(tmp, param);
		pair<bool, double> two = InversionTest(tmp, param);
		if (one.first && two.first) {
			out << "Slow car's stream is recurrent" << "\n";
			out << "WallisMoore's result: " << one.second << "\n";
			out << "InversionTest's result: " << two.second << "\n";
		}
		else {
			out << "Slow car's stream isn't recurrent" << "\n";
			out << "WallisMoore's result: " << one.second << "\n";
			out << "InversionTest's result: " << two.second << "\n";
			// Увеличим параметр h в 2 раза
			h += 1;
			//h *= 2;
			// Перейдем на следующую итерацию
			continue;
		}
		out << "\n";

		// Проверяем количество машин в пачках
		vector<double> tmp2;
		for (const auto& item : packs_of_cars) {
			tmp2.push_back(item.size());
		}

		pair<bool, double> one2 = WallisMoore(tmp2, param);
		pair<bool, double> two2 = InversionTest(tmp2, param);
		if (one2.first && two2.first) {
			out << "Number of packs is recurrent" << "\n";
			out << "WallisMoore's result: " << one2.second << "\n";
			out << "InversionTest's result: " << two2.second << "\n";
		}
		else {
			out << "Number of packs isn't recurrent" << "\n";
			out << "WallisMoore's result: " << one2.second << "\n";
			out << "InversionTest's result: " << two2.second << "\n";
			// Увеличим параметр h в 2 раза
			h += 1;
			//h *= 2;
			// Перейдем на следующую итерацию
			continue;
		}
		flag = false;
	}

	cout << "SynchronousMethod" << endl;
	cout << "Number of packs = " << packs_of_cars.size() << endl;
	cout << "Step is " << h << endl;

	cout << "----------------------------------------" << endl;
	// Второй(третий) способ разбиения на пачки - Поэтапное делание
	bool _flag = true;
	//double _h = *min_element(input_intervals.begin(), input_intervals.end());
	double _h = 10;
	double _h1 = 1.5 * _h;
	int _d = 10;
	vector<vector<double>> _packs;

	while (_flag) {
		//cout << _h << endl;
		//cout << _h1 << endl;
		ofstream out2("sec.txt");
		out2 << "----------------------------------------" << "\n";
		out2 << "New iteration" << "\n";
		out2 << "h = " << _h << "\n";
		out2 << "h1 = " << _h1 << "\n";
		out2 << "d = " << _d << "\n";
		// На первой итерации метод "Поэтапного делени" работает так же, как предыдущий
		_packs = SynchronousMethod(input_intervals, _h);
		out2 << "Number of packs = " << _packs.size() << "\n";
		out2 << "\n";
		for (const auto& item : _packs) {
			out2 << "Slow car is " << item.at(0) << endl;
			out2 << "Cars in pack: ";
			for (const auto& i : item) {
				out2 << i << " ";
			}
			out2 << "\n";
			out2 << "\n";
		}

		bool ans = true;
		while (ans) {
			ans = PhasedSplit(_packs, _h1, _d);
			for (const auto& item : _packs) {
				out2 << "Slow car is " << item.at(0) << "\n";
				out2 << "Cars in pack: ";
				for (const auto& i : item) {
					out2 << i << " ";
				}
				out2 << "\n";
				out2 << "Size pack = " << item.size();
				out2 << "\n";
				out2 << "\n";
			}
			out2 << "Number of packs = " << _packs.size() << endl;
			out2 << ans << endl;
		}

		// Медленные машины - первый элемент в пачке
		vector<double> tmp;

		for (size_t i = 0; i < _packs.size() - 1; ++i) {
			if (i == 0) {
				tmp.push_back(*_packs[i].begin());
			}
			else {
				tmp.push_back(
					accumulate(_packs[i - 1].begin(), _packs[i - 1].end(),
						*_packs[i].begin()));
			}
		}

		// Проверяем критерии для медленных машин
		pair<bool, double> one = WallisMoore(tmp, param);
		pair<bool, double> two = InversionTest(tmp, param);
		if (one.first && two.first) {
			out2 << "Slow car's stream is recurrent" << "\n";
			out2 << "WallisMoore's result: " << one.second << "\n";
			out2 << "InversionTest's result: " << two.second << "\n";
		}
		else {
			out2 << "Slow car's stream isn't recurrent" << "\n";
			out2 << "WallisMoore's result: " << one.second << "\n";
			out2 << "InversionTest's result: " << two.second << "\n";
			// Увеличим параметр h в 2 раза
			_h += 1;
			_h1 = 1.5 * _h;
			// Перейдем на следующую итерацию
			continue;
		}
		out2 << "\n";

		// Проверяем количество машин в пачках
		vector<double> tmp2;
		for (const auto& item : _packs) {
			tmp2.push_back(item.size());
		}

		if (h == 40) {
			cout <<endl<<"!!!";
		}
		if (_h1 == 60) {
			cout << endl << "!!!";
		}
		pair<bool, double> one2 = WallisMoore(tmp2, param);
		pair<bool, double> two2 = InversionTest(tmp2, param);
		if (one2.first && two2.first) {
			out2 << "Number of packs is recurrent" << "\n";
			out2 << "WallisMoore's result: " << one2.second << "\n";
			out2 << "InversionTest's result: " << two2.second << "\n";
		}
		else {
			out2 << "Number of packsisn't recurrent" << "\n";
			out2 << "WallisMoore's result: " << one2.second << "\n";
			out2 << "InversionTest's result: " << two2.second << "\n";
			// Увеличим параметр h в 2 раза
			_h += 1;
			_h1 = 1.5 * _h;
			// Перейдем на следующую итерацию
			continue;
		}

		_flag = false;
	}

	cout << "PhasedSplit" << endl;
	cout << "Number of packs = " << _packs.size() << endl;
	cout << "h is " << _h << endl;
	cout << "h1 is " << _h1 << endl;
	cout << "d is " << _d << endl;

	return 0;
}
