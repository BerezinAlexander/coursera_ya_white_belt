#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <sstream>
#include <map>
#include <functional>
#include <algorithm>
#include <set>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
	IteratorRange(Iterator begin, Iterator end)
		: first(begin)
		, last(end)
	{
	}

	Iterator begin() const {
		return first;
	}

	Iterator end() const {
		return last;
	}

private:
	Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
	return IteratorRange{ v.begin(), next(v.begin(), min(top, v.size())) };
}

struct Person {
	string name;
	int age, income;
	bool is_male;
};

class PeopleStore {
public:
	PeopleStore() = default;

	void AddPerson(Person& p) {
		people.emplace_back(move(p));
		const Person& person = people.back();

		incomeId.insert(person.income);
		ageId.insert(person.age);
		
		(person.is_male) ? maleNames[person.name]++ : famaleNames[person.name]++;
	}

	const vector<Person>& getPeople() const { return people; }
	const multiset<int, greater<int>>& getIncomePeople() const { return incomeId; }
	const multiset<int>& getAgePeolpe() const { return ageId; }

	void calcPopularNames() {
		auto el1 = max_element(maleNames.begin(), maleNames.end(),
			[](const pair<string, int>& p1, const pair<string, int>& p2) {
				return p1.second < p2.second;
			}
		);

		if (el1 != maleNames.end())
			maleNamePop = el1->first;

		auto el2 = max_element(famaleNames.begin(), famaleNames.end(),
			[](const pair<string, int>& p1, const pair<string, int>& p2) {
				return p1.second < p2.second;
			}
		);
		
		if (el2 != famaleNames.end())
			famaleNamePop = el2->first;
	}

	string getPopularNameMale() const {
		return maleNamePop;
	}

	string getPopularNameFamale() const {
		return famaleNamePop;
	}

private:
	vector<Person> people;
	multiset<int, greater<int>> incomeId;
	multiset<int> ageId;
	map<string, size_t> maleNames;
	map<string, size_t> famaleNames;
	string maleNamePop;
	string famaleNamePop;
};

PeopleStore ReadPeople(istream& input) {
	int count;
	input >> count;

	PeopleStore result;
	for(int i=0; i< count; ++i){
		Person p;
		char gender;
		input >> p.name >> p.age >> p.income >> gender;
		p.is_male = (gender == 'M');
		result.AddPerson(p);
	}

	return result;
}

int program(istream& input) {
	PeopleStore peopleStore = ReadPeople(input);
	peopleStore.calcPopularNames();
	const vector<Person>& people = peopleStore.getPeople();

	for (string command; input >> command; ) {
		if (command == "AGE") {
			int adult_age;
			input >> adult_age;

			const auto& ages = peopleStore.getAgePeolpe();
			auto adult_begin = ages.lower_bound(adult_age);

			cout << "There are " << std::distance(adult_begin, end(ages))
				<< " adult people for maturity age " << adult_age << '\n';
		}
		else if (command == "WEALTHY") {
			int count;
			input >> count;

			const auto& incomePeople = peopleStore.getIncomePeople();			
			count = (count < incomePeople.size()) ? count : incomePeople.size();

			int total_income = accumulate(
				incomePeople.begin(), next(incomePeople.begin(), count), 0, [](int cur, int income) {
					return cur += income;
				}
			);

			cout << "Top-" << count << " people have total income " << total_income << '\n';
		}
		else if (command == "POPULAR_NAME") {
			char gender;
			input >> gender;

			string genderCount = (gender == 'M') 
					? peopleStore.getPopularNameMale() 
					: peopleStore.getPopularNameFamale();

			cout << "Most popular name among people of gender " << gender << " is "
				<< genderCount << '\n';
		}
	}

	return 0;
}

#ifdef _MSC_VER
	int main() {
		string testInput = "11 \
			Ivan 25 1000 M \
			Olga 30 623 W \
			Sergey 24 825 M \
			Maria 42 1254 W \
			Mikhail 15 215 M \
			Oleg 18 230 M \
			Denis 53 8965 M \
			Maxim 37 9050 M \
			Ivan 47 19050 M \
			Ivan 17 50 M \
			Olga 23 550 W \
			AGE 18 \
			AGE 25 \
			WEALTHY 5 \
			WEALTHY 11 \
			WEALTHY 20 \
			POPULAR_NAME M \
			POPULAR_NAME W";

		stringstream ss(testInput);

		int result = program(ss);

		system("pause");
		return result;
	}
#else
	int main() {
		return program(cin);
	}
#endif