#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <future>
#include <thread>
#include <random>
#include <chrono>

#include "profile.h"

using namespace std::chrono;

vector<string> SplitIntoWords(const string& line) {
	istringstream words_input(line);
	return { istream_iterator<string>(words_input), istream_iterator<string>() };
}

SearchServer::SearchServer(istream& document_input)
	: firstUpdate(true)
{
	UpdateDocumentBase(document_input);
}

SearchServer::SearchServer()
	: firstUpdate(true)
{
}


void SearchServer::UpdateDocumentBaseAsync(istream& document_input, SearchServer& server) {
	InvertedIndex new_index;

	for (string current_document; getline(document_input, current_document); ) {
		new_index.Add(move(current_document));
	}

	server.index.GetAccess().ref_to_value = move(new_index);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {

	//updateIndex.push_back(async(UpdateDocumentBaseAsync, ref(document_input), ref(*this)));
	
	//if (firstUpdate) {
	//	updateIndex.back().get();
	//	firstUpdate = false;
	//}

	//this_thread::sleep_for(2s);

	if (firstUpdate) {
		InvertedIndex new_index;

		for (string current_document; getline(document_input, current_document); ) {
			new_index.Add(move(current_document));
		}

		index.GetAccess().ref_to_value = move(new_index);

		firstUpdate = false;
	}
}

void SearchServer::AddQueriesStream(
	istream& query_input, ostream& search_results_output
) {
	size_t docs_size = index.GetAccess().ref_to_value.GetDocsSize();
	size_t vsize = (docs_size < 5) ? 5 : docs_size;
	vector<pair<size_t, size_t>> docid_count(vsize);

	for (string current_query; getline(query_input, current_query); ) {
		const auto words = SplitIntoWords(current_query);

		docid_count.assign(vsize, { 0,0 });
		for (const auto& word : words) {
			for (auto& p : index.GetAccess().ref_to_value.Lookup(word)) {
				docid_count[p.first].first = p.first;
				docid_count[p.first].second += p.second;
			}
		}

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
