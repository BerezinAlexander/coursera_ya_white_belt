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
			return sections[name];
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

	Document Load(istream& input) {
		Document result;

		Section* current_section = nullptr;
		for (string line; getline(input, line); ) {
			if (!line.empty()) {
				if (line[0] == '[') {
					current_section = &result.AddSection(line.substr(1, line.size() - 2));
				}
				else {
					size_t eq_pos = line.find('=');
					current_section->insert({ line.substr(0, eq_pos), line.substr(eq_pos + 1) });
				}
			}
		}

		return result;
	}

}

ostream& operator<<(ostream& os, const Ini::Section& sec) {
	for (const auto& p : sec) {
		os << p.first << "=" << p.second << "\n";
	}
	return os;
}