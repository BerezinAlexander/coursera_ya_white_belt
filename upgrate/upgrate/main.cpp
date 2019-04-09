#include "test_runner.h"
#include "profile.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>
#include <list>

using namespace std;

class Editor {
public:
	// –еализуйте конструктор по умолчанию и объ€вленные методы
	Editor() {
		pos = text.end();
	}
	void Left() {
		if (pos != text.begin())
			pos = prev(pos);
	}
	void Right() {
		if (pos != text.end()) {
			pos = next(pos);
		}
	}
	void Insert(char token) {
		if (!token)
			return;

		text.insert(pos, token);
	}
	void Cut(size_t tokens = 1) {
		auto cut_end = pos;
		while (cut_end != text.end() && tokens > 0) {
			cut_end = next(cut_end);
			--tokens;
		}	

		if (pos == cut_end) {
			buffer.clear();
			return;
		}

		buffer = list<char>(pos, cut_end);
		text.erase(pos, cut_end);
		pos = cut_end;
	}
	void Copy(size_t tokens = 1) {
		auto cut_end = pos;
		while (cut_end != text.end() && tokens > 0) {
			cut_end = next(cut_end);
			--tokens;
		}
		
		if (pos == cut_end) {
			buffer.clear();
			return;
		}

		buffer = list<char>(pos, cut_end);
	}
	void Paste() {
		if (buffer.empty())
			return;

		text.insert(pos, buffer.begin(), buffer.end());
		//if(pos != text.end())
		//	pos = next(pos, buffer.size());
	}
	string GetText() const {
		string result;
		for (auto c : text)
			result += c;
		return result;
	}

private:
	list<char>::iterator pos;
	list<char> text;
	list<char> buffer;
};

void TypeText(Editor& editor, const string& text) {
	for (char c : text) {
		editor.Insert(c);
	}
}

void TestEditing() {
	{
		Editor editor;

		const size_t text_len = 12;
		const size_t first_part_len = 7;
		TypeText(editor, "hello, world");
		for (size_t i = 0; i < text_len; ++i) {
			editor.Left();
		}
		editor.Cut(first_part_len);
		for (size_t i = 0; i < text_len - first_part_len; ++i) {
			editor.Right();
		}
		TypeText(editor, ", ");
		editor.Paste();
		editor.Left();
		editor.Left();
		editor.Cut(3);

		ASSERT_EQUAL(editor.GetText(), "world, hello");
	}
	{
		Editor editor;

		TypeText(editor, "misprnit");
		editor.Left();
		editor.Left();
		editor.Left();
		editor.Cut(1);
		editor.Right();
		editor.Paste();

		ASSERT_EQUAL(editor.GetText(), "misprint");
	}
}

void TestReverse() {
	Editor editor;

	const string text = "esreveR";
	for (char c : text) {
		editor.Insert(c);
		editor.Left();
	}

	ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
	Editor editor;
	ASSERT_EQUAL(editor.GetText(), "");

	editor.Left();
	editor.Left();
	editor.Right();
	editor.Right();
	editor.Copy(0);
	editor.Cut(0);
	editor.Paste();

	ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
	Editor editor;

	editor.Paste();
	TypeText(editor, "example");
	editor.Left();
	editor.Left();
	editor.Paste();
	editor.Right();
	editor.Paste();
	editor.Copy(0);
	editor.Paste();
	editor.Left();
	editor.Cut(0);
	editor.Paste();

	ASSERT_EQUAL(editor.GetText(), "example");
}

void TestOne() {
	Editor editor;

	editor.Insert('h');
	editor.Insert('e');
	editor.Insert('l');
	editor.Insert('l');
	editor.Insert('o');
	editor.Insert(',');
	editor.Insert('_');
	editor.Insert('w');
	editor.Insert('o');
	editor.Insert('r');
	editor.Insert('l');
	editor.Insert('d');

	editor.Left();
	editor.Left();
	editor.Left();
	editor.Left();
	editor.Left();
	editor.Left();
	editor.Left();
	editor.Left();
	editor.Left();
	editor.Left();
	editor.Left();
	editor.Left();

	editor.Cut(7);

	editor.Right();
	editor.Right();
	editor.Right();
	editor.Right();
	editor.Right();

	editor.Insert(',');
	editor.Insert('_');

	editor.Paste();

	editor.Left();
	editor.Left();

	editor.Cut(3);

	ASSERT_EQUAL(editor.GetText(), "world,_hello");
}

void TestUser() {
	Editor editor;

	editor.Paste();
	TypeText(editor, "example is very best word");
	
	for (int i = 0; i<9; ++i)
		editor.Left();
	
	editor.Cut(4);
	TypeText(editor, "small ");

	editor.Paste();

	//editor.Paste();
	//editor.Right();
	//editor.Paste();
	//editor.Copy(0);
	//editor.Paste();
	//editor.Left();
	//editor.Cut(0);
	//editor.Paste();

	ASSERT_EQUAL(editor.GetText(), "example is very small best word");

	editor.Paste();

	ASSERT_EQUAL(editor.GetText(), "example is very small bestbest word");


	for (int i = 0; i < 15; ++i)
		editor.Left();
	editor.Cut(6);

	ASSERT_EQUAL(editor.GetText(), "example is very bestbest word");

	for (int i = 0; i < 14; ++i)
		editor.Right();
	editor.Paste();
	ASSERT_EQUAL(editor.GetText(), "example is very bestbest word small");

}



int main() {
	TestRunner tr;
	RUN_TEST(tr, TestEditing);
	RUN_TEST(tr, TestReverse);
	RUN_TEST(tr, TestNoText);
	RUN_TEST(tr, TestEmptyBuffer);
	RUN_TEST(tr, TestOne);
	RUN_TEST(tr, TestUser);

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}