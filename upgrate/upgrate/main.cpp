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

class Node : public Expression {
public:
	Node(int value_)
		: value(value_), type(NodeType::Value)
	{}

	Node(ExpressionPtr lhs, ExpressionPtr rhs, NodeType type_)
		: left(move(lhs)), right(move(rhs)), type(type_)
	{}

	// Вычисляет значение выражения
	int Evaluate() const {
		switch (type) {
		case NodeType::Value:	return value;
		case NodeType::Sum:		return left->Evaluate() + right->Evaluate();
		case NodeType::Product: return left->Evaluate() * right->Evaluate();
		default:				return 0;
		}
	}

	// Форматирует выражение как строку
	// Каждый узел берётся в скобки, независимо от приоритета
	string ToString() const {
		switch (type){
		case NodeType::Value:	return to_string(value);
		case NodeType::Sum:		return "(" + left->ToString() + ")" + "+" + "(" + right->ToString() + ")";
		case NodeType::Product: return "(" + left->ToString() + ")" + "*" + "(" + right->ToString() + ")";
		default:				return "";
		}
	}

private:
	ExpressionPtr left;
	ExpressionPtr right;
	NodeType type;
	int value;
};

ExpressionPtr Value(int value) {
	return make_unique<Node>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
	return make_unique<Node>(move(left), move(right), NodeType::Sum);
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
	return make_unique<Node>(move(left), move(right), NodeType::Product);
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