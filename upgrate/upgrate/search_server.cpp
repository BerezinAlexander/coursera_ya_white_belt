#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <future>

#include "profile.h"

vector<string> SplitIntoWords(const string& line) {
	istringstream words_input(line);
	return { istream_iterator<string>(words_input), istream_iterator<string>() };
}

SearchServer::SearchServer(istream& document_input) {
	UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
	InvertedIndex new_index;

	for (string current_document; getline(document_input, current_document); ) {
		new_index.Add(move(current_document));
	}

	index = move(new_index);
}

istream& ReadLine(istream& input, string& s, TotalDuration& dest) {
	//ADD_DURATION(dest);
	return getline(input, s);
}

void SearchServer::AddQueriesStream(
	istream& query_input, ostream& search_results_output
) {
	TotalDuration read("new Total read");
	TotalDuration parse("new Total parse");
	TotalDuration count("new Total count");
	TotalDuration search("new Total search");

	size_t docs_size = index.GetDocsSize();
	size_t vsize = (docs_size < 5) ? 5 : docs_size;
	vector<pair<size_t, size_t>> docid_count(vsize);

	for (string current_query; /*getline(query_input, current_query);*/ ReadLine(query_input, current_query, read); ) {
		ADD_DURATION(parse);		

		const auto words = SplitIntoWords(current_query);

		ADD_DURATION(count);

		docid_count.assign(vsize, { 0,0 });
		for (const auto& word : words) {
			for (auto& p : index.Lookup(word)) {
				docid_count[p.first].first = p.first;
				docid_count[p.first].second += p.second;
			}

			//if (docid_count.size() < 5) {
			//	while(doc)
			//}
			//	docid_count.resize(5);
			//for (const auto& p : index.Lookup(word)) {
			//	docid_count[p.first] = p
			//}
		}

		ADD_DURATION(count);

		size_t size_sort = (docid_count.size() < 5) ? docid_count.size() : 5;

		partial_sort(
			begin(docid_count),
			begin(docid_count) + size_sort,
			end(docid_count),
			[](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
				int64_t lhs_docid = lhs.first;
				auto lhs_hit_count = lhs.second;
				int64_t rhs_docid = rhs.first;
				auto rhs_hit_count = rhs.second;
				return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
			}
		);

		search_results_output << current_query << ':';
		for (auto[docid, hitcount] : Head(docid_count, 5)) {
			if (hitcount != 0) {
				search_results_output << " {"
					<< "docid: " << docid << ", "
					<< "hitcount: " << hitcount << '}';
			}
		}
		search_results_output << endl;
	}
}

void InvertedIndex::Add(const string& document) {
	docs.push_back(document);

	const size_t docid = docs.size() - 1;
	for (const auto& word : SplitIntoWords(document)) {
		auto& vec = index[word];
		auto it = find_if(vec.begin(), vec.end(), 
			[&docid](const pair<size_t, size_t>& p) {
				return docid == p.first; }
		);

		if (it == vec.end()) {
			vec.emplace_back(docid, 1 );
		}
		else {
			it->second++;
		}
	}
}

const vector<pair<size_t, size_t>>& InvertedIndex::Lookup(const string& word) const {
	if (auto it = index.find(word); it != index.end()) {
		return it->second;
	}
	else {
		return {};
	}
}
