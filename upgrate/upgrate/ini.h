#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <sstream>

using namespace std;

namespace Ini {

	using Section = unordered_map<string, string>;

	class Document {
	public:
		Section& AddSection(string name) {
			sections.insert(make_pair(name, Section()));
			return sections.at(name);
		}
		const Section& GetSection(const string& name) const {
			return sections.at(name);
		}
		size_t SectionCount() const {
			return sections.size();
		}

	private:
		unordered_map<string, Section> sections;
	};

	bool LoadSectionName(istream& input, string& sectionName) {
		return static_cast<bool>(getline(input, sectionName, ']'));
	}

	void parseKeyValue(istream& input, string& key, string& value) {
		getline(input, key, '=');
		input >> value;
	}

	void LoadSection(istream& input, Section& section) {
		string sec_str;
		getline(input, sec_str, '[');
		stringstream ss;
		ss << sec_str;
		
		string key, value;
		string keyValue;
		while (ss >> keyValue) {
			stringstream ss;
			ss << keyValue;
			parseKeyValue(ss, key, value);
			section.insert({ key, value });
		}
	}

	Document Load(istream& input) {
		Document doc;

		char c;
		input >> c;
		if (c == '[') {
			string sectionName;
			while (LoadSectionName(input, sectionName)) {
				Section& section = doc.AddSection(sectionName);
				LoadSection(input, section);
			}
		}

		return doc;
	}

}

ostream& operator<<(ostream& os, const Ini::Section& sec) {
	for (const auto& p : sec) {
		os << p.first << "=" << p.second << "\n";
	}
	return os;
}