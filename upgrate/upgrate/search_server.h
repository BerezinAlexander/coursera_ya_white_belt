#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
using namespace std;

vector<string> SplitIntoWords(const string& line);

class InvertedIndex {
public:
  void Add(const string& document);
  list<size_t> Lookup(const string& word) const;

  const string& GetDocument(size_t id) const {
    return docs[id];
  }

  void operator += (const InvertedIndex& other) {
	  const size_t docid_shift = docs.size();
	  for (auto& doc : other.docs) {
		  docs.push_back(doc);
		  /*const size_t docid = docs.size() - 1;
		  for (const auto& word : SplitIntoWords(doc)) {
			  index[word].push_back(docid);
		  }*/
	  }
	  for (auto& ind : other.index) {
		  for (auto& old_docid : ind.second) {
			  index[ind.first].push_back(old_docid + docid_shift);
		  }
	  }
  }

private:
  map<string, list<size_t>> index;
  vector<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  InvertedIndex index;
};
