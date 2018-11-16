//#include "test_runner.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;


class Person {
public:
	explicit Person(const string &n, const string &t) : name(n), type(t) {}

	virtual void Walk(const string &destination) const {
		cout << type << ": " << name << " walks to: " << destination << endl;
	};

	string Log() const {
		return type + ": " + name;
	}

	string GetName() const {
		return name;
	}

	string GetType() const {
		return type;
	}

protected:
	const string name;
	const string type;
};

class Student : public Person {
public:
	explicit Student(const string &name, const string &favouriteSong) : Person(name, "Student"), FavouriteSong(favouriteSong) {}

	void Learn() const {
		cout << Log() << " learns" << endl;
	}

	void Walk(const string &destination) const override {
		cout << Log() << " walks to: " << destination << endl;
		cout << Log() << " sings a song: " << FavouriteSong << endl;
	}

	void SingSong() const {
		cout << Log() << " sings a song: " << FavouriteSong << endl;
	}

private:
	const string FavouriteSong;
};

class Teacher : public Person {
public:
	explicit Teacher(const string &name, string &subject) : Person(name, "Teacher"), Subject(subject) {}

	void Teach() const {
		cout << Log() << " teaches: " << Subject << endl;
	}

private:
	const string Subject;
};

class Policeman : Person {
public:
	explicit Policeman(const string &name) : Person(name, "Policeman") {}

	void Check(const Person &t) const {
		cout << Log() << " checks " << t.GetType() << ". " << t.GetType() << "'s name is: "
			<< t.GetName() << endl;
	}
};


void VisitPlaces(const Person &person, vector<string> places) {
	for (const auto &p : places) {
		person.Walk(p);
	}
}


int main() {
	string teacher_name = "Matt", student_name = "Ann", policeman_name = "Bob";
	string subject = "Math", song = "We will rock you";
	Teacher t(teacher_name, subject);
	Student s(student_name, song);
	Policeman p(policeman_name);
	vector<string> places = { "Moscow", "London" };

	VisitPlaces(t, places);
	p.Check(s);
	VisitPlaces(s, places);
	return 0;
}