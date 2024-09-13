#include "task1.h"
#include <iostream>

using namespace std;
int main(void) {
	CIntN nums1(100), nums2(10);

	cout << nums1.print() << "+" << nums2.print() << "=" << (nums1+nums2).print() << endl;

	return 0;
}
