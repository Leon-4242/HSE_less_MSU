#include <iostream>

using namespace std;

int main(void) {
	int k;
	unsigned long long p, q;
	cin >> k >> p;
	int sign = 1;
	if ((1 << 63) & p) >> 63) {
	   	sign *= -1;     
	}
	unsigned long long m = (1 << k) - 1;
    m = m & p;
    p = p >> k;
    
    if (p == (1 << (63-k)) - 1) {
    	cout << "NaN" << endl;
        return 0;
    } 
    if (p == 0) {
    	q = 1 - k - (1 << (62-k));
        p = m;
   	} else {
    	q = p + 1 - k - (1 << (62-k));
    	p = m + 1;
    }
    // x = p * (2 ** q)//
    
    if (p == 0) {
    	cout << 0 << endl;
        return 0;
    }
    
    while (p % 2 == 0) {
    	p = p / 2;
        q++;
   	}
    
    if (q == 0) {
    	cout << p << endl;
    } else if ( q > 0) {
    	cout << p << "*2**" << q << endl;
    } else {
    	cout << p << "/2**" << -q << endl;
    }
    return 0;
}
