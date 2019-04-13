#include "test_runner.h"
#include "profile.h"

#include <cstdint>
#include <iterator>
#include <numeric>

#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>
#include <list>
#include <deque>

#include <string_view>
#include <tuple>

using namespace std;

// ��������� Sentence<Token> ��� ������������� ���� Token
// ��������� vector<Token>.
// ��������� ����� � �������� ������������� ��������
// ������� ����� ������� �� ������������ ������ ��������,
// � ������ ����������� � vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

// ����� Token ����� ����� bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
	// �������� ���������� �������, �� ������� ������� ���� Token
	vector<Sentence<Token>> sentenses;
	Sentence<Token> sentense;
	
	auto it = tokens.begin();

	while (it != tokens.end()) {

		while (it != tokens.end() && !(*it).IsEndSentencePunctuation()) {
			sentense.push_back(move(*it));
			++it;
		}

		while (it != tokens.end() && (*it).IsEndSentencePunctuation()) {
			sentense.push_back(move(*it));
			++it;
		}

		sentenses.push_back(move(sentense));

	}

	return sentenses;
}

struct TestToken {
	string data;
	bool is_end_sentence_punctuation = false;

	bool IsEndSentencePunctuation() const {
		return is_end_sentence_punctuation;
	}
	bool operator==(const TestToken& other) const {
		return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
	}
};

ostream& operator<<(ostream& stream, const TestToken& token) {
	return stream << token.data;
}

// ���� �������� ����������� �������� ������ TestToken.
// ��� �������� ���������� ����������� � ������� SplitIntoSentences
// ���������� �������� ��������� ����.
void TestSplitting() {
	ASSERT_EQUAL(
		SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, {"!"} })),
		vector<Sentence<TestToken>>({
			{{"Split"}, {"into"}, {"sentences"}, {"!"}}
			})
	);

	ASSERT_EQUAL(
		SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, 
			{"!", true} })),
		vector<Sentence<TestToken>>({
			{{"Split"}, {"into"}, {"sentences"}, {"!", true}}
			})
	);

	ASSERT_EQUAL(
		SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, 
			{"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true} })),
		vector<Sentence<TestToken>>({
			{{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
			{{"Without"}, {"copies"}, {".", true}},
			})
			);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestSplitting);

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}