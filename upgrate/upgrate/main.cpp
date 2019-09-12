#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "test_runner.h"

using namespace std;

bool IsSubdomain(string_view subdomain, string_view domain) {
    int i = 0;
    int j = 0;
    while (i < subdomain.size() && j < domain.size()) {
        if (subdomain[i++] != domain[j++]) {
            return false;
        }
    }

    return (i == subdomain.size() && j == domain.size())
        //|| (i == subdomain.size() && domain[j] == '.')
        || (j == domain.size() && subdomain[i] == '.');

    //int i = subdomain.size() - 1;
    //int j = domain.size() - 1;
    //while (i >= 0 && j >= 0) {
    //    if (subdomain[i--] != domain[j--]) {
    //        return false;
    //    }
    //}
    //return (i < 0 && j < 0)
    //    || (i < 0 && domain[j] == '.')
    //    || (j < 0 && subdomain[i] == '.');
}


vector<string> ReadDomains() {
    size_t count;
    cin >> count;

    vector<string> domains;
    for (size_t i = 0; i < count; ++i) {
        string domain;
        cin >> domain;
        //getline(cin, domain);
        domains.push_back(domain);
    }
    return domains;
}

void ReverseStringsAndSort(vector<string>& v) {
    for (string& s : v) {
        reverse(begin(s), end(s));
    }
    sort(begin(v), end(v));
}

void EraseSubdomains(vector<string>& banned_domains) {
    size_t insert_pos = 0;
    for (string& domain : banned_domains) {
        if (insert_pos == 0 || !IsSubdomain(domain, banned_domains[insert_pos - 1])) {
            //banned_domains[insert_pos++] = move(domain);
            swap(banned_domains[insert_pos++], domain);
        }
    }
    banned_domains.resize(insert_pos);
}

bool CheckDomain(const string& domain, const vector<string>& banned_domains) {
    const auto it = upper_bound(begin(banned_domains), end(banned_domains), domain);
    if (it == begin(banned_domains) || it != begin(banned_domains) && !IsSubdomain(domain, *prev(it))) {
        return true;
    }
    
    return false;
}

void TestIsSubdomain() {
    string domain = "moc";
    string subdomain = "moc.y";

    ASSERT_EQUAL(IsSubdomain(subdomain, domain), true);
    ASSERT_EQUAL(IsSubdomain(domain, subdomain), false);
}

void TestEraseSubdomains() {
    vector<string> domains = {
        "com",
        "ta.com",
        "ya",
        "res.ta.com",
        "res.ya",
        "res.m.ru",
        "m.ru"
    };

    ReverseStringsAndSort(domains);
    EraseSubdomains(domains);

    vector<string> ideal = {
        "com",
        "ya",
        "m.ru"
    };

    ReverseStringsAndSort(ideal);

    ASSERT_EQUAL(domains, ideal);
}

void TestCheckDomain() {
    vector<string> banned_domains = {
        "com",
        "ya",
        "m.ru"
    };
    ReverseStringsAndSort(banned_domains);

    ASSERT_EQUAL(CheckDomain("moc", banned_domains), false);
    ASSERT_EQUAL(CheckDomain("moc.ser", banned_domains), false);
    ASSERT_EQUAL(CheckDomain("mmoc", banned_domains), true);

    banned_domains = {
    "em.spam",
    "moc",
    "ur.ay"
    };

    ASSERT_EQUAL(CheckDomain("ur.ay", banned_domains), false);
    ASSERT_EQUAL(CheckDomain("moc.ay", banned_domains), false);
    ASSERT_EQUAL(CheckDomain("em.spam.m", banned_domains), false);
    ASSERT_EQUAL(CheckDomain("ur.ay.m.wocsom", banned_domains), false);
    ASSERT_EQUAL(CheckDomain("moc.spam", banned_domains), false);
    ASSERT_EQUAL(CheckDomain("ur.spam", banned_domains), true);
    ASSERT_EQUAL(CheckDomain("ay.ay", banned_domains), true);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIsSubdomain);
    RUN_TEST(tr, TestEraseSubdomains);
    RUN_TEST(tr, TestCheckDomain);

    vector<string> banned_domains = ReadDomains();
    vector<string> domains_to_check = ReadDomains();

    ReverseStringsAndSort(banned_domains);

    EraseSubdomains(banned_domains);

    for (string& domain : domains_to_check) {
        reverse(begin(domain), end(domain));
    }

    for (const string& domain : domains_to_check) {
        if (CheckDomain(domain, banned_domains)) {
            cout << "Good" << endl;
        }
        else {
            cout << "Bad" << endl;
        }
    }

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}