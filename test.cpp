#include "sequence.h"
#include <iostream>

using namespace std;
int main(void) {
	sequence fib(2, 1, 1, 1, 1);
	int k;
	while(true) {
		cout << "Enter number: ";
		cin >> k;
		if (k < 0) {
			break;
		}
		cout << fib[k] << "\n" << endl;
	}

	cout << 1845853122 + 1412467027 << endl;

	return 0;
}
