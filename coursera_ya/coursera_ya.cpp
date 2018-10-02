// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

void calc(int a, int b)
{
	if (b == 0)
		cout << "Impossible" << endl;
	else
		cout << a / b << endl;
}

int main()
{
	int a, b;

	cin >> a >> b;

	calc(a,b);


	//system("pause");
    return 0;
}

