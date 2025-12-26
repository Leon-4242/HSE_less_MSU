#include "output.h"

void output(int r, int l, int n, double* a) {
	std::cout << std::scientific << std::setprecision(3);
//	std::cout << "atrix: " << std::endl;	
	for (int i = 0; i < (r < l ? r : l); ++i) {
		for (int j = 0 ; j < (r < n ? r : n); ++j) {
			std::cout << " " << std::setw(10) << a[i*n+j];
		}
		std::cout << "\n";
	}

	std::cout << std::defaultfloat;
}
