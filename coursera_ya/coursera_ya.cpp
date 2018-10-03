// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

int main()
{
	int num_q;
	cin >> num_q;

	map<string, vector<string>> buses;
	map<string, vector<string>> stops;

	for (int i = 0; i < num_q; i++) {
		string command;
		cin >> command;

		if (command == "NEW_BUS") {
			string bus;
			int stop_count;
			cin >> bus >> stop_count;
			vector<string> cur_stops(stop_count);
			for (auto& stop : cur_stops) {
				cin >> stop;
				stops[stop].push_back(bus);
			}			
			buses[bus] = move(cur_stops);
		}
		else if (command == "BUSES_FOR_STOP") {
			string stop;
			cin >> stop;

			if (stops.count(stop) == 0)
				cout << "No stop" << endl;
			else {
				for (auto& bus : stops[stop]) {
					cout << bus << " ";
				}
				cout << endl;
			}
		}
		else if (command == "STOPS_FOR_BUS") {
			string bus;
			cin >> bus;

			if (buses.count(bus) == 0)
				cout << "No bus" << endl;
			else {
				for (auto& stop : buses[bus]) {
					cout << "Stop " << stop << ": ";
					for (auto& cur_bus : stops[stop]) {
						if(cur_bus != bus)
							cout << cur_bus << " ";
						else if (cur_bus == bus && stops[stop].size() == 1) {
							cout << "no interchange";
						}
					}
					cout << endl;
				}
			}
		}
		else if (command == "ALL_BUSES") {
			if (buses.empty())
				cout << "No buses" << endl;
			else {
				for (auto& p : buses) {
					cout << "Bus " << p.first << ": ";
					for (auto& cur_stop : buses[p.first]) {
						cout << cur_stop << " ";
					}
					cout << endl;
				}
			}
		}

	}


#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}

