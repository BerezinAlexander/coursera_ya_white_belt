// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

void calc(int a, int b)
{
	for (int i = a; i <= b; i++) {
		if (i % 2 == 0)
			cout << i << " ";
	}
}

int main()
{
	int a, b;
	cin >> a >> b;
	calc(a, b);

	system("pause");
    return 0;
}

