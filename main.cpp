#include "1.h"
#include <iostream>

using namespace std;
int main(void) {
	CintN k(4), t(5);

	cout << k.print() << "-" << t.print() << "=" << (k-t).print() << endl;

	return 0;
}
