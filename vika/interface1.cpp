#include "Interface.h"

Interface::~Interface() {
	if (k != 0) {
		delete[] var;
	}
}

void Interface::add(const CintN& x) {
	CintN *tmp;
	tmp = new CintN[k+1];
	for (int i = 0; i < k; i++) {
		tmp[i] = var[i];
	}
	tmp[k] = x;
	delete[] var;
	var = tmp;
}

void Interface::set(int kk, const CintN& x) {
	if (kk >= 0 && kk < k) {
		var[kk] = x;
	} else {
		throw std::string{"Wrong index"};
	}
}

std::ostream& operator<< (std::ostream &os, const Interface &view) {
	std::string tmp;
	for (int i = 0; i < k; i++) {
		tmp += var[i].print();
		tmp += " ";
	}

	return os << tmp;
}
