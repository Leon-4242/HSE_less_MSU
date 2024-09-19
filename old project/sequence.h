#ifndef SEQ
#define SEQ

#include <vector>
#include <stdarg.h>

using namespace std;

class sequence {
	unsigned v;
	unsigned r;
	vector<int> val;
	vector<int> ratios;
	
	void add(void) {
		int res{};

		for (unsigned i = 0; i < r; ++i) {
			res += ratios[i]*val[v-r+i];
		}
		v++;
		val.push_back(res);
	}

	public:

	sequence(unsigned d, ...) {
		va_list rat;
		va_start(rat, d);
		for (unsigned i = 0; i < d; ++i) {
			ratios.push_back(va_arg(rat, int));
		}
		for (unsigned i = 0; i < d; ++i) {
			val.push_back(va_arg(rat, int));
		}
		v = r = d;
		va_end(rat);
	}

	~sequence() { };

	int operator[](unsigned index) {
		while (index > v-1) {
			this->add();
		}
		return val[index];
	}
};

#endif
