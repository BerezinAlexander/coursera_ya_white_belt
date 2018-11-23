#include "test_runner.h"

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <numeric>
#include <map>
#include <algorithm>
#include <set>

//#include "LogDuration.h"

using namespace std;

class Learner {
private:
	set<string> dict;

public:
	int Learn(const vector<string>& words) {
		//LOG_DURATION("Learn: ");
		int newWords = 0;
		for (const auto& word : words) {
			if (dict.count(word) == 0) {
				dict.insert(word);
				++newWords;
			}
		}
		return newWords;
	}

	vector<string> KnownWords() {
		//LOG_DURATION("KnownWords: ");
		//sort(dict.begin(), dict.end());
		//dict.erase(unique(dict.begin(), dict.end()), dict.end());
		
		vector<string> result;
		for (const auto& word : dict) {
			result.emplace_back(word);
		}

		return result;
	}
};

int main() {
	Learner learner;
	string line;
	while (getline(cin, line)) {
		vector<string> words;
		stringstream ss(line);
		string word;
		while (ss >> word) {
			//if(word == "exit")
			//	break;
			words.push_back(word);
		}
		//if (word == "exit")
		//	break;
		cout << learner.Learn(words) << "\n";
	}
	cout << "=== known words ===\n";
	for (auto word : learner.KnownWords()) {
		cout << word << "\n";
	}


#ifdef _MSC_VER
	system("pause");
#endif
}