#include "Common.h"
#include <list>
#include <algorithm>
#include <mutex>
#include <unordered_map>
#include <utility>

using namespace std;

class LruCache : public ICache {
public:
	LruCache( shared_ptr<IBooksUnpacker> books_unpacker_,
			  const Settings& settings_) 
		: settings(settings_), books_unpacker(books_unpacker_),
		cacheSize(0)
	{
	}

	BookPtr GetBook(const string& book_name) override {
		lock_guard<mutex> lock(mtx);
		BookPtr bookPtr = takeBookFromCacheAndUpdateRating(book_name);
		if (!bookPtr) {
			bookPtr = books_unpacker->UnpackBook(book_name);
			bookPtr = addToCache(bookPtr);
		}

		return bookPtr;
	}

private:
	BookPtr takeBookFromCacheAndUpdateRating(const string& book_name) {
		if (!mapBooks.count(book_name))
			return BookPtr();

		auto itBook = mapBooks[book_name];
		books.emplace_front(*itBook);
		books.erase(itBook);
		mapBooks[book_name] = books.begin();

		return books.front();
	}

	BookPtr addToCache(BookPtr book) {

		books.push_front(book);
		mapBooks[book->GetName()] = books.begin();
		cacheSize += book->GetContent().size();

		while (cacheSize >= settings.max_memory) {
			if (books.empty())
				return book;
			cacheSize -= books.back()->GetContent().size();
			mapBooks.erase(books.back()->GetName());
			books.pop_back();
		}	

		if (books.empty())
			return book;

		return books.front();
	}

private:
	Settings settings;
	shared_ptr<IBooksUnpacker> books_unpacker;
	list<BookPtr> books;
	unordered_map<string, list<BookPtr>::iterator> mapBooks;
	size_t cacheSize;
	mutex mtx;
};


unique_ptr<ICache> MakeCache(
	shared_ptr<IBooksUnpacker> books_unpacker,
	const ICache::Settings& settings
) {
	return make_unique<LruCache>(books_unpacker, settings);
}