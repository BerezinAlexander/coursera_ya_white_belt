// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

void calc(float n, float a, float b, float x, float y)
{
	if (n > b)
		cout << n - (n * y / 100.) << endl;
	else if (n > a)
		cout << n - (n * x / 100.) << endl;
	else
		cout << n << endl;
}

//void calc(int a, int b)
//{
//	for (int i = a; i <= b; i++) {
//		if (i % 2 == 0)
//			cout << i << " ";
//	}
//}

int main()
{
	float n, a, b, x, y;
	cin >> n >> a >> b >> x >> y;
	calc(n, a, b, x, y);

	//int a, b;
	//cin >> a >> b;
	//calc(a, b);

	system("pause");
    return 0;
}

