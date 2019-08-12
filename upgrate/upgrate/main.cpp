#include "test_runner.h"
#include "Common.h"

#include <memory>

#include <sstream>

using namespace std;

enum class NodeType {
	Value,
	Sum,
	Product
};

// �����, �������������� ���������� ����� - ���� ������ ���������
class ValueExpr : public Expression {
public:
	ValueExpr(int value) : value_(value) {}

	int Evaluate() const override {
		return value_;
	}
	string ToString() const override {
		return to_string(value_);
	}

private:
	int value_;
};

// ������� ����� �������� ��������
class BinaryExpr : public Expression {
public:
	BinaryExpr(ExpressionPtr left, ExpressionPtr right) :
		left_(move(left)),
		right_(move(right)) {}

	// ����� ����������� ������� ���������������� � �������� ������ "final".
	// ��� �� �� �����, ��� � "override", �� ������ �� ��������� ����������
	// �� ��������������� � �����������. �������������, �� ����� ��������,
	// ��� ���������� ������ �������������� �������� ������� GetSymbol() �
	// EvaluateOnValues(), � ���� ������� ToString() � Evaluate() ������� ������
	// �� �����.
	string ToString() const final {
		ostringstream result;
		result << '(' << left_->ToString() << ')'
			<< GetSymbol()
			<< '(' << right_->ToString() << ')';
		return result.str();
	}
	int Evaluate() const final {
		return EvaluateOnValues(left_->Evaluate(), right_->Evaluate());
	}

private:
	// �������� ���� ����� ����������� ������� ��������� ��������� ������������
	// ���� � �����������, �.�. ���� ����� ��� ������ � �������� ToString() �
	// Evaluate() �������� (�������) ������. � ���������� ������ �������� ������
	// �������� � ��������� � � ���������� ���������.
	virtual char GetSymbol() const = 0;
	virtual int EvaluateOnValues(int l, int r) const = 0;

	ExpressionPtr left_;
	ExpressionPtr right_;
};

// ����� ��� �������� ���������
class ProductExpr : public BinaryExpr {
public:
	ProductExpr(ExpressionPtr left, ExpressionPtr right) :
		BinaryExpr(move(left), move(right)) {}

private:
	char GetSymbol() const override {
		return '*';
	}
	int EvaluateOnValues(int left, int right) const override {
		return left * right;
	}
};

// ����� ��� �������� ��������
class SumExpr : public BinaryExpr {
public:
	// ������ ��������� ��������� ������������ ����������� �� �������� ������.
	// �������� � ����������� ������ ProductExpr, ��� �� ������������ "�������"
	// �����������, ������� ��������� ����� �� �� ���������, ��� � �����������
	// �������� ������, � ������ ������� �� ���. �� �� ����� ������ �� ���
	// ����� ��������� ����������, ���� �� ������ ��� ������������ �����������.
	using BinaryExpr::BinaryExpr;

private:
	char GetSymbol() const override {
		return '+';
	}
	int EvaluateOnValues(int left, int right) const override {
		return left + right;
	}
};

// ������� ��� ������������ ���������
ExpressionPtr Value(int value) {
	return make_unique<ValueExpr>(value);
}
ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
	return make_unique<SumExpr>(move(left), move(right));
}
ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
	return make_unique<ProductExpr>(move(left), move(right));
}

string Print(const Expression* e) {
	if (!e) {
		return "Null expression provided";
	}
	stringstream output;
	output << e->ToString() << " = " << e->Evaluate();
	return output.str();
}

void Test() {
	ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
	ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

	ExpressionPtr e2 = Sum(move(e1), Value(5));
	ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

	ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, Test);

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}