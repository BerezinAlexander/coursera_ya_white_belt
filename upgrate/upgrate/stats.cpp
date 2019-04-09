#include "stats.h"

#include <map>
#include <string_view>
#include <sstream>

using namespace std;

void Stats::AddMethod(string_view method)
{
	if (mMetods.count(method) != 0) {
		++mMetods[method];
	}
	else {
		++mMetods["UNKNOWN"];
	}
}

void Stats::AddUri(string_view uri)
{
	if (mURI.count(uri) != 0) {
		++mURI[uri];
	}
	else {
		++mURI["unknown"];
	}
}

const map<string_view, int>& Stats::GetMethodStats() const
{
	return mMetods;
}

const map<string_view, int>& Stats::GetUriStats() const
{
	return mURI;
}

HttpRequest ParseRequest(string_view line)
{
	HttpRequest req;

	size_t space = line.find(' ');
	while (space == 0) {
		line.remove_prefix(space + 1);
		space = line.find(' ');
	}

	req.method = line.substr(0, space);
	line.remove_prefix(space + 1);

	space = line.find(' ');
	req.uri = line.substr(0, space);
	line.remove_prefix(space + 1);

	req.protocol = line;

	return req;
}