#include "input.h"

int input(int s, const std::string& filename, int n, double* a) {
	if (s == 0) return finput(filename, n, a);
	else return sinput(s, n, a);
}

int finput(const std::string& filename, int n, double* a) {
	double curr = 0;

	std::ifstream in(filename);
	if (!in.is_open()) return 1;
	
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (!(in >> a[i*n+j])) {
				in.close(); 
				return -2;
			}
		}
	}

	in.close();
	return 0;
}

int sinput(int s, int n, double* a) {
	if (s < 1 || s > 4) return -3;
	
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			a[i*n+j] = formula(s, n, i, j);
		}
	}

	return 0;
}

//formuls changed because (i, j) starts from (0, 0)
double formula(int s, int n, int i, int j) {
	switch (s) {
		case 1:
			return n- (i > j ? i : j);
		case 2:
			return (i > j ? i : j)+1;
		case 3:
			return (i > j ? i : j) - (i < j ? i : j);
		case 4:
			return 1./(i+j+1);
	}
	return 0;
}
