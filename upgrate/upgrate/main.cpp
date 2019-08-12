#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
	string from;
	string to;
	string body;
};


class Worker {
public:
	virtual ~Worker() = default;
	virtual void Process(unique_ptr<Email> email) = 0;
	virtual void Run() {
		// только первому worker-у в пайплайне нужно это имплементировать
		throw logic_error("Unimplemented");
	}

protected:
	// реализации должны вызывать PassOn, чтобы передать объект дальше
	// по цепочке обработчиков
	void PassOn(unique_ptr<Email> email) const {
		if(next_worker)
			next_worker->Process(move(email));
	}

public:
	void SetNext(unique_ptr<Worker> next) {
		next_worker = move(next);
	}

protected:
	unique_ptr<Worker> next_worker;
};


class Reader : public Worker {
public:
	/*конструктор должен принимать входной поток (istream&), из которого нужно читать сообщения*/
	Reader(istream& input_)
		: input(input_)
	{}

	/*метод Run должен читать из входного потока все e-mail'ы и передавать дальше каждый из них. 
	Один e-mail представляется во входном потоке полями from, to, body, 
	каждое из которых находится на своей строке.
	*/
	void Run() {
		while (input) {
			unique_ptr<Email> ptrEmail = make_unique<Email>();

			getline(input, ptrEmail->from);
			if (!input) break;
			getline(input, ptrEmail->to);
			if (!input) break;
			getline(input, ptrEmail->body);
			PassOn(move(ptrEmail));
		}
	}

	void Process(unique_ptr<Email> email) {
	}

private:
	istream& input;
};


class Filter : public Worker {
public:
	using Function = function<bool(const Email&)>;

public:
	Filter(Function predicate_)
		: predicate(predicate_)
	{}

	void Process(unique_ptr<Email> email) {
		if (predicate(*email.get())) {
			PassOn(move(email));
		}
	}

private:
	Function predicate;
};


class Copier : public Worker {
public:
	Copier(string sendTo_)
		: sendTo(sendTo_)
	{}

	void Process(unique_ptr<Email> email) {
		auto em = email.get();
		unique_ptr<Email> email2;
		if (em->to != sendTo) {
			email2 = make_unique<Email>(Email({ em->from, sendTo, em->body }));
		}
		PassOn(move(email));
		if(email2)
			PassOn(move(email2));
	}

private:
	string sendTo;
};


class Sender : public Worker {
public:
	Sender(ostream& output_)
		: output(output_)
	{}

	void Process(unique_ptr<Email> email) {
		auto em = email.get();
		output << em->from << "\n";
		output << em->to << "\n";
		output << em->body << "\n";

		PassOn(move(email));
	}

private:
	ostream& output;
};


// реализуйте класс
class PipelineBuilder {
public:
	PipelineBuilder() = delete;

	// добавляет в качестве первого обработчика Reader
	explicit PipelineBuilder(istream& in)
		: reader(new Reader(in))
	{
	}

	// добавляет новый обработчик Filter
	PipelineBuilder& FilterBy(Filter::Function filter) {
		workers.emplace_back(make_unique<Filter>(filter));
		return *this;
	}

	// добавляет новый обработчик Copier
	PipelineBuilder& CopyTo(string recipient) {
		workers.emplace_back(make_unique<Copier>(recipient));
		return *this;
	}

	// добавляет новый обработчик Sender
	PipelineBuilder& Send(ostream& out) {
		workers.emplace_back(make_unique<Sender>(out));
		return *this;
	}

	// возвращает готовую цепочку обработчиков
	unique_ptr<Reader> Build() {
		if (!workers.empty()) {
			unique_ptr<Worker> main_worker = move(workers.back());
			workers.pop_back();
			while (!workers.empty()) {			
				unique_ptr<Worker> worker = move(workers.back());
				workers.pop_back();
				worker->SetNext(move(main_worker));
				main_worker = move(worker);
			}
			reader->SetNext(move(main_worker));
		}
		return move(reader);
	}

private:
	vector<unique_ptr<Worker>> workers;
	unique_ptr<Reader> reader;
};


void TestSanity() {
	string input = (
		"erich@example.com\n"
		"richard@example.com\n"
		"Hello there\n"

		"erich@example.com\n"
		"ralph@example.com\n"
		"Are you sure you pressed the right button?\n"

		"ralph@example.com\n"
		"erich@example.com\n"
		"I do not make mistakes of that kind\n"
		);
	istringstream inStream(input);
	ostringstream outStream;

	PipelineBuilder builder(inStream);
	builder.FilterBy([](const Email& email) {
		return email.from == "erich@example.com";
		});
	builder.CopyTo("richard@example.com");
	builder.Send(outStream);
	auto pipeline = builder.Build();

	pipeline->Run();

	string expectedOutput = (
		"erich@example.com\n"
		"richard@example.com\n"
		"Hello there\n"

		"erich@example.com\n"
		"ralph@example.com\n"
		"Are you sure you pressed the right button?\n"

		"erich@example.com\n"
		"richard@example.com\n"
		"Are you sure you pressed the right button?\n"
		);

	ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestSanity);


#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}