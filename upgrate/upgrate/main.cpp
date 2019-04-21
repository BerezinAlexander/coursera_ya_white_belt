#include "test_runner.h"
#include "profile.h"

#include <cstdint>
#include <iterator>
#include <numeric>

#include <iostream>

#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>
#include <list>
#include <deque>

#include <string_view>
#include <tuple>
#include <memory>
#include <iterator>

#include <future>
#include <mutex>


using namespace std;

struct Stats {
	map<string, int> word_frequences;

	void operator += (const Stats& other) {
		for (auto& p : other.word_frequences) {
			word_frequences[p.first] += p.second;
		}
	}
	
	//friend ostream& operator << (ostream& s, const Stats& my)
	//{
	//	cout << "Stats:" << endl;
	//	for (const auto& p : my.word_frequences) {
	//		s << p.first << " " << p.second << endl;
	//	}
	//	return s;
	//}
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
	string_view sv(line);
	Stats stats;

	while (true) {
		size_t space = sv.find(' ');
		string str(sv.substr(0, space));

		if (key_words.count(str)) {
			stats.word_frequences[str]++;
		}

		if (space == str.npos) {
			break;
		}
		else {
			sv.remove_prefix(space+1);
		}
	}

	return stats;
}



//Stats ExploreLine(const set<string>& key_words, const string& line) {
//	//set<string_view> key_words_view;
//	//for (auto& key : key_words)
//	//	key_words_view.insert(key);
//	
//	string_view sv(line);
//	//stringstream ss(move(line));
//	Stats stats;
//	size_t pos = sv.find_first_not_of(" ");
//	while (pos != string::npos) {
//		sv.remove_prefix(pos);
//		pos = sv.find_first_of(" ");
//		if (pos != string::npos) {
//			string str(sv.substr(0, pos));
//			if (key_words.count(str)) {
//				addToMap(stats.word_frequences, str);
//			}
//		}
//		else {
//			string str(sv.substr(0));
//			if (key_words.count(str)) {
//				addToMap(stats.word_frequences, str);
//			}
//			break;
//		}
//		sv.remove_prefix(pos);
//		pos = sv.find_first_not_of(" ");
//	}
//
//	//string word;
//	//while (ss >> word) {
//	//	if (key_words.count(word)) {
//	//		addToMap(stats.word_frequences, word);
//	//	}
//	//}
//	return stats;
//}

//Stats ExploreLine(const set<string>& key_words, const string& line) {
//	stringstream ss(move(line));
//	Stats stats;
//
//	string word;
//	while (ss >> word) {
//		if (key_words.count(word)) {
//			addToMap(stats.word_frequences, word);
//		}
//	}
//	return stats;
//}

//Stats ExploreKeyWordsSingleThread(
//		const set<string>& key_words, istream& input) {
//	Stats result;
//	for (string line; getline(input, line); ) {
//		result += ExploreLine(key_words, line);
//	}
//	return result;
//}

Stats ExploreKeyWordsSingleThread(
	const set<string>& key_words, vector<string>& input) {
	Stats result;
	for (auto line : input) {
		result += ExploreLine(key_words, line);
	}
	return result;
}

//Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
//
//	LOG_DURATION("ExploreKeyWords");
//
//	//int count_threads = 1000;
//	int count_ss = 25000;
//
//	// Реализуйте эту функцию
//	vector<future<Stats>> futures;
//
//
//	vector<stringstream> vss(2); // (count_threads + 1);
//	//vss.emplace_back("");
//	int count = 0;
//	int c_t = 0;
//	for (string line; getline(input, line); ) {
//		vss[c_t] << line << "\n";
//		if (++count == count_ss) {
//			++c_t;
//			//vss.emplace_back("");
//		}
//	}
//
//	for (auto& ss : vss) {
//		futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(ss)));
//	}
//
//	Stats stats;
//	for (auto& f : futures) {
//		stats += f.get();
//	}
//
//	return stats;
//}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
	//int count_ss = 25000;
	vector<future<Stats>> futures;

	//vector<vector<string>> vss;
	//vss.emplace_back(vector<string>());
	//int count = 0;
	//for (string line; getline(input, line); ) {
	//	vss.back().emplace_back(move(line));
	//	//vss.back() += "\n";
	//	if (++count == count_ss) {
	//		//futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(vss.back()) ));
	//		vss.emplace_back(vector<string>());
	//	}
	//}

	vector<string> vss;
	for (string line; getline(input, line); ) {
		vss.push_back(move(line));
	}

	//auto middle = vss.begin() + (vss.end() - vss.begin()) / 2;

	//vector<string> temp1(make_move_iterator(vss.begin()), make_move_iterator(middle));
	//vector<string> temp2(make_move_iterator(middle), make_move_iterator(vss.end()));

	//futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(temp1)));
	//futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(temp2)));

	size_t dist = (vss.end() - vss.begin()) / 3;
	auto middle1 = vss.begin() + dist;
	auto middle2 = middle1 + dist;

	vector<string> temp1(make_move_iterator(vss.begin()), make_move_iterator(middle1));
	vector<string> temp2(make_move_iterator(middle1), make_move_iterator(middle2));
	vector<string> temp3(make_move_iterator(middle2), make_move_iterator(vss.end()));

	futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(temp1)));
	futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(temp2)));
	futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(temp3)));

	Stats stats;
	for (auto& f : futures) {
		stats += f.get();
	}

	return stats;
}

void TestBasic() {
	const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

	stringstream ss;
	ss << "this new yangle service really rocks\n";
	ss << "It sucks when yangle isn't available\n";
	ss << "10 reasons why yangle is the best IT company\n";
	ss << "yangle rocks others suck\n";
	ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

	const auto stats = ExploreKeyWords(key_words, ss);

	//cout << stats << endl;
	cout << endl << stats.word_frequences << endl << endl;

	const map<string, int> expected = {
	  {"yangle", 6},
	  {"rocks", 2},
	  {"sucks", 1}
	};

	cout << expected << endl << endl;

	ASSERT_EQUAL(stats.word_frequences, expected);
}

void TestBasic2() {
	const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

	const int max_count = 50000;

	stringstream ss;
	for (int i = 0; i < max_count; ++i) {
		ss << "this new yangle service really rocks\n";
		ss << "It sucks when yangle isn't available\n";
		ss << "10 reasons why yangle is the best IT company\n";
		ss << "yangle rocks others suck\n";
		ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
	}

	const auto stats = ExploreKeyWords(key_words, ss);

	//cout << stats << endl;
	cout << endl << stats.word_frequences << endl << endl;

	const map<string, int> expected = {
	  {"yangle", 6 * max_count},
	  {"rocks", 2 * max_count},
	  {"sucks", 1 * max_count}
	};

	cout << expected << endl << endl;

	ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestBasic);
	RUN_TEST(tr, TestBasic2);

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}