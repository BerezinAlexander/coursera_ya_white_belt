//#include <iostream>
//#include <string>
////#include <cmath>
////#include <set>
////#include <vector>
////#include <algorithm>
////#include <map>
////#include <sstream>
////#include <chrono>
////#include <numeric>
////#include <deque>
////
//////#include "LogDuration.h"
////
//using namespace std;
//////using namespace std::chrono;
//
//void SendSms(const string& number, const string& message) {
//	cout << "Send '" << message << "' to number " << number << endl;
//}
//
//void SendEmail(const string& email, const string& message) {
//	cout << "Send '" << message << "' to e-mail " << email << endl;
//}


class INotifier {
public:
	virtual void Notify(const string& message) = 0;
};

class SmsNotifier : public INotifier {
public:
	SmsNotifier(const string& number_)
		: number(number_)
	{}
	void Notify(const string& message) override {
		SendSms(number, message);
	}

private:
	string number;
};

class EmailNotifier : public INotifier {
public:
	EmailNotifier(const string& emale_)
		: emale(emale_)
	{}
	void Notify(const string& message) override {
		SendEmail(emale, message);
	}

private:
	string emale;
};

//void Notify(INotifier& notifier, const string& message) {
//	notifier.Notify(message);
//}
//
//int main() {
//	SmsNotifier sms{ "+7-495-777-77-77" };
//	EmailNotifier email{ "na-derevnyu@dedushke.ru" };
//
//	Notify(sms, "I have White belt in C++");
//	Notify(email, "And want a Yellow one");
//
//#ifdef _MSC_VER
//	system("pause");
//#endif
//
//	return 0;
//}