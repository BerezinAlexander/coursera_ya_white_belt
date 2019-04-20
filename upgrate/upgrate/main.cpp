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

inline void addToMap(map<string, int>& m, const string& word) {
	m.count(word) ? m[word]++ : m[word] = 1;
}

struct Stats {
	map<string, int> word_frequences;

	void operator += (const Stats& other) {
		for (auto& p : other.word_frequences) {
			word_frequences.count(p.first) ? word_frequences[p.first] += p.second 
										   : word_frequences[p.first] = p.second;
		}
	}
	
	friend ostream& operator << (ostream& s, const Stats& my)
	{
		cout << "Stats:" << endl;
		for (const auto& p : my.word_frequences) {
			s << p.first << " " << p.second << endl;
		}
		return s;
	}
};


Stats ExploreLine(const set<string>& key_words, const string& line) {
	stringstream ss(line);
	Stats stats;
	string word;
	while (ss >> word) {
		if (key_words.count(word)) {
			addToMap(stats.word_frequences, word);
		}
	}
	return stats;
}

Stats ExploreKeyWordsSingleThread(
	const set<string>& key_words, istream& input
) {
	Stats result;
	for (string line; getline(input, line); ) {
		result += ExploreLine(key_words, line);
	}
	return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
	
	LOG_DURATION("ExploreKeyWords");

	// Реализуйте эту функцию
	vector<future<Stats>> futures;
	for (string line; getline(input, line); ) {
		futures.push_back( async(ExploreLine, ref(key_words), line) );
	}

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
	//cout << stats.word_frequences << endl << endl;

	const map<string, int> expected = {
	  {"yangle", 6},
	  {"rocks", 2},
	  {"sucks", 1}
	};

	//cout << expected << endl << endl;

	ASSERT_EQUAL(stats.word_frequences, expected);
}

void TestBasic2() {
	const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

	const int max_count = 1000;

	stringstream ss;
	for (int i = 0; i < max_count; ++i) {
		ss << "this new yangle service really rocks\n";
		ss << "It sucks when yangle isn't available\n";
		ss << "10 reasons why yangle is the best IT company\n";
		ss << "yangle rocks others suck\n";
	}

	const auto stats = ExploreKeyWords(key_words, ss);

	//cout << stats << endl;
	//cout << stats.word_frequences << endl << endl;

	const map<string, int> expected = {
	  {"yangle", 6 * max_count},
	  {"rocks", 2 * max_count},
	  {"sucks", 1 * max_count}
	};

	//cout << expected << endl << endl;

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