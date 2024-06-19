#include <iostream>

using namespace std;

int main(void) {
	int k;
	unsigned long long p;
	long long q;
	cin >> k >> p;
	int sign = 1;
	if (((1ULL << 63ULL) & p) >> 63ULL == 1ULL) {
	   	sign *= -1;     
	}
	unsigned long long m = (1ULL << k) - 1ULL;
    	m = m & p;
	p = p & (~(1ULL << 63ULL));
    	p = p >> k;

   
    	if (p == (1ULL << (63ULL-k)) - 1ULL) {
    		cout << "NaN" << endl;
        	return 0;
    	}

	if (p == 0ULL) {
    		q = 1LL - k - (1LL << (62LL-k));
        	p = m;
	} else {
    		q = p + 1LL - k - (1LL << (62LL-k));
    		p = m + (1ULL << k);
    	}

    	if (p == 0ULL) {
    		cout << 0 << endl;
        	return 0;
    	}
    
    	while (p % 2ULL == 0) {
    		p = p / 2ULL;
    	    	q++;
   	}
    
    	if (q == 0LL) {
    		cout << p << endl;
    	} else if ( q > 0LL) {
		if (sign == -1) {
			cout << "-" << p << "*2**" << q << endl;
		} else {	
    			cout << p << "*2**" << q << endl;
		}
    	} else {
		if (sign == -1) {
                        cout << "-" << p << "/2**" << q << endl;
                } else {
                        cout << p << "/2**" << q << endl;
                }
    	}
    	return 0;
}
