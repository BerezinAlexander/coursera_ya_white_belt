#include "json.h"

using namespace std;

namespace Json {

  Document::Document(Node root) : root(move(root)) {
  }

  const Node& Document::GetRoot() const {
    return root;
  }

  Node LoadNode(istream& input);

  Node LoadArray(istream& input) {
    vector<Node> result;

    for (char c; input >> c && c != ']'; ) {
      if (c != ',') {
        input.putback(c);
      }
      result.push_back(LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadValue(istream& input) {
    double result = 0;

	bool negative = false;
	if (input.peek() == '-') {
		negative = true;
		input.ignore(1);
	}

    while (isdigit(input.peek())) {
      result *= 10;
      result += input.get() - '0';
    }

	int64_t offset = 1;
	if (input.peek() == '.') {
		input.ignore(1);
		while (isdigit(input.peek())) {
			offset *= 10;
			result += static_cast<double>(input.get() - '0') / offset;
		}
	}

	if (negative) {
		result *= -1;
	}
	
	if (offset == 1 && result > 0)
		return Node(static_cast<int>(result));

	return Node(result);
  }

  Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
    return Node(move(line));
  }

  Node LoadBool(istream& input) {
	  bool result = false;
	  char c;
	  input >> c;
	  if (c == 't') {
		  input.ignore(3);
		  result = true;
	  }
	  else {
		  input.ignore(4);
	  }
	  //string str;
	  //getline(input, str, '}');
	  //if (str == "true")
		 // result = true;
	  //input.putback('}');
	  return Node(result);
  }

  Node LoadDict(istream& input) {
    map<string, Node> result;

    for (char c; input >> c && c != '}'; ) {
      if (c == ',') {
        input >> c;
      }

      string key = LoadString(input).AsString();
      input >> c;
	  result.emplace(move(key), LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadNode(istream& input) {
    char c;
    input >> c;

	if (c == '[') {
		return LoadArray(input);
	}
	else if (c == '{') {
		return LoadDict(input);
	}
	else if (c == '"') {
		return LoadString(input);
	}
	else if (c == 't' || c == 'f') {
		input.putback(c);
		return LoadBool(input);
	} else {
      input.putback(c);
      return LoadValue(input);
    }
  }

  Document Load(istream& input) {
    return Document{LoadNode(input)};
  }

}
