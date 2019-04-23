#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <future>

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

InvertedIndex ExploreBatch(vector<string> documents) {
	InvertedIndex new_index;
	for(auto& doc : documents)
		new_index.Add(move(doc));
	return new_index;
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
	const size_t max_batch_size = 2000;

	vector<string> batch;
	batch.reserve(max_batch_size);

	vector<future<InvertedIndex>> futures;

	for (string current_document; getline(document_input, current_document); ) {
		batch.push_back(move(current_document));
		if (batch.size() >= max_batch_size) {
			futures.push_back(async(ExploreBatch, move(batch)));
			batch.reserve(max_batch_size);
		}
	}

	InvertedIndex new_index;

	if (!batch.empty()) {
		new_index += ExploreBatch(move(batch));
	}

	for (auto& f : futures) {
		new_index += f.get();
	}

	index = move(new_index);
}

void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
  for (string current_query; getline(query_input, current_query); ) {
    const auto words = SplitIntoWords(current_query);

    map<size_t, size_t> docid_count;
    for (const auto& word : words) {
      for (const size_t docid : index.Lookup(word)) {
        docid_count[docid]++;
      }
    }

    vector<pair<size_t, size_t>> search_results(
      docid_count.begin(), docid_count.end()
    );
    sort(
      begin(search_results),
      end(search_results),
      [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
        int64_t lhs_docid = lhs.first;
        auto lhs_hit_count = lhs.second;
        int64_t rhs_docid = rhs.first;
        auto rhs_hit_count = rhs.second;
        return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
      }
    );

    search_results_output << current_query << ':';
    for (auto [docid, hitcount] : Head(search_results, 5)) {
      search_results_output << " {"
        << "docid: " << docid << ", "
        << "hitcount: " << hitcount << '}';
    }
    search_results_output << endl;
  }
}

void InvertedIndex::Add(const string& document) {
  docs.push_back(document);

  const size_t docid = docs.size() - 1;
  for (const auto& word : SplitIntoWords(document)) {
    index[word].push_back(docid);
  }
}

list<size_t> InvertedIndex::Lookup(const string& word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return {};
  }
}
