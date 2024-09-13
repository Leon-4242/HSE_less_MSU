#include "1.h"
#include <iostream>

using namespace std;
int main(void) {
	CintN k(5), t(8);

	cout << k.print() << "+" << t.print() << "=" << (k+t).print() << endl;

	return 0;
}
