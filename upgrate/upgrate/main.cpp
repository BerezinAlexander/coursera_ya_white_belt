#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>

using namespace std;

class ReadingManager {
public:
	ReadingManager()
		// -1 ������, ��� �� ��������� �� ������ READ
		: user_page_counts_(MAX_USER_COUNT_ + 1, -1),
		page_achieved_by_count_(MAX_PAGE_COUNT_ + 1, 0) {}

	void Read(int user_id, int page_count) {
		UpdatePageRange(user_page_counts_[user_id] + 1, page_count + 1);
		user_page_counts_[user_id] = page_count;
	}

	double Cheer(int user_id) const {
		const int pages_count = user_page_counts_[user_id];
		if (pages_count == -1) {
			return 0;
		}
		const int user_count = GetUserCount();
		if (user_count == 1) {
			return 1;
		}
		// �� ��������� ������� �������������, �������
		// ����� �������� ��������� � ���� double.
		// ������� ������ ������� ��� � �������� ��� �� 1.0.
		return (user_count - page_achieved_by_count_[pages_count]) * 1.0
			/ (user_count - 1);
	}

private:
	// ����������� ���� �� ����������� ������-���� ����������� ������� ������. 
	// �� ���� ��� ���������� ����������, � ������ ������ - �����������.
	// ���� ��� ���������, � ��� ����� ���� �� ���������� �������
	// ��������� �������: ReadingManager::MAX_USER_COUNT.
	static const int MAX_USER_COUNT_ = 100'000;
	static const int MAX_PAGE_COUNT_ = 1'000;

	// ����� ��������, �� ������� ������� ������������ <����>
	vector<int> user_page_counts_;
	// ���������� �������������, ���������� (��� �������) �� �������� <������>
	vector<int> page_achieved_by_count_;

	int GetUserCount() const {
		return page_achieved_by_count_[0];
	}

	// lhs ������������, rhs �� ������������
	void UpdatePageRange(int lhs, int rhs) {
		for (int i = lhs; i < rhs; ++i) {
			++page_achieved_by_count_[i];
		}
	}
};


//void Test() {
//	ReadingManager manager;
//
//	ASSERT_EQUAL(manager.Cheer(5), 0);
//	manager.Read(1, 10);
//	ASSERT_EQUAL(manager.Cheer(1), 1);
//	manager.Read(2, 5);
//	manager.Read(3, 7);
//	ASSERT_EQUAL(manager.Cheer(2), 0);
//	ASSERT_EQUAL(manager.Cheer(3), 0.5);
//	manager.Read(3, 10);
//	ASSERT_EQUAL(manager.Cheer(3), 0.5);
//	manager.Read(3, 11);
//	ASSERT_EQUAL(manager.Cheer(3), 1);
//	ASSERT_EQUAL(manager.Cheer(1), 0.5);
//}

int main() {
	//TestRunner tr;
	//RUN_TEST(tr, Test);

	// ��� ��������� ������ ������ ����������� �������������
	// cin � cout � stdio,
	// � ����� ����������� ������� cin �� cout
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ReadingManager manager;

	int query_count = 43;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;
		cin >> query_type;
		int user_id;
		cin >> user_id;

		if (query_type == "READ") {
			int page_count;
			cin >> page_count;
			manager.Read(user_id, page_count);
		}
		else if (query_type == "CHEER") {
			cout << setprecision(6) << manager.Cheer(user_id) << "\n";
		}
	}


#ifdef _MSC_VER
	system("pause");
#endif
}