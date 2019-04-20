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



template <typename Iterator>
class IteratorRange {
public:
	IteratorRange(Iterator begin, Iterator end)
		: first(begin)
		, last(end)
		, size_(distance(first, last))
	{
	}

	Iterator begin() const {
		return first;
	}

	Iterator end() const {
		return last;
	}

	size_t size() const {
		return size_;
	}

private:
	Iterator first, last;
	size_t size_;
};

template <typename Iterator>
class Paginator {
private:
	vector<IteratorRange<Iterator>> pages;

public:
	Paginator(Iterator begin, Iterator end, size_t page_size) {
		for (size_t left = distance(begin, end); left > 0; ) {
			size_t current_page_size = min(page_size, left);
			Iterator current_page_end = next(begin, current_page_size);
			pages.push_back({ begin, current_page_end });

			left -= current_page_size;
			begin = current_page_end;
		}
	}

	auto begin() const {
		return pages.begin();
	}

	auto end() const {
		return pages.end();
	}

	size_t size() const {
		return pages.size();
	}
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
	return Paginator(begin(c), end(c), page_size);
}







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

template <typename ContainerOfVectors>
Stats ExploreKeyWordsSingleThread(
	const set<string>& key_words, ContainerOfVectors& strings
) {
	Stats result;
	for (auto& s : strings) {
		result += ExploreLine(key_words, s);
	}
	//for (string line; getline(input, line); ) {
	//	result += ExploreLine(key_words, line);
	//}
	return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
	
	LOG_DURATION("ExploreKeyWords");


	vector<string> strings;
	{
		LOG_DURATION("Fill");
		for (string line; getline(input, line); ) {
			strings.push_back(move(line));
		}
		cout << endl;
	}

	vector<future<Stats>> futures;
	Stats result;
	{
		LOG_DURATION("Calc");
		for (auto page : Paginate(strings, 1000)) {
			futures.push_back(async([=] { return ExploreKeyWordsSingleThread(key_words, page); }));
		}
		for (auto& f : futures) {
			result += f.get();
		}
		cout << endl;
	}

	//// Реализуйте эту функцию
	//vector<future<Stats>> futures;
	//for (string line; getline(input, line); ) {
	//	futures.push_back( async(ExploreLine, ref(key_words), line) );
	//}

	//Stats stats;
	//for (auto& f : futures) {
	//	stats += f.get();
	//}

	cout << endl;

	return result;
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

	const int max_count = 50000;

	stringstream ss;
	for (int i = 0; i < max_count; ++i) {
		ss << "this new yangle service really rocks\n";
		ss << "It sucks when yangle isn't available\n";
		ss << "10 reasons why yangle is the best IT company\n";
		ss << "yangle rocks others suck\n";
	}

	const auto stats = ExploreKeyWords(key_words, ss);

	cout << endl;

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