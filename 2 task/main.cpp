#include "interface.h"

using namespace std;

int main(void) {
	using namespace InFace;
	Interface I; int act, way; size_t n, m; polynom P; string op;
	while (true) {
		cout << "\nEnter action:\n1)Add polynom\n2)Print polynoms\n3)Actions with polynoms" << endl;
		cin >> act;
		if (act != 1 && act != 2 && act !=3) {
			break;
		}
		if (act == 1) {
			cout << "\nEnter polynom" << endl;
			cin >> P;
			I += P;
		} else if (act == 2) {
			cout << I << endl;
		} else {
			if (I.empty()) {
				cout << "\nNot enough data" << endl;
			} else {
				cout << "\nEnter action with polynoms:\n1)add\n2)substract\n3)multiplication\n4)division\n5)GCD\n6)derivative\n7)polynom_same_roots" << endl;
				cin >> act;
				if (act != 1 && act != 2 && act != 3 && act != 4 && act != 5 && act != 6 && act != 7) {
					break;
				}
				
				cout << "\nEnter" << ((act == 6 || act == 7 ) ? "number" : "numbers") << "of polynoms for action:" << endl;
				(act == 6 || act == 7) ? cin >> n : cin >> n >> m;
				
				cout << "\nChoose way to result:\n1)Print\n2)Save\n3)Print and save" << endl;
				cin >> way;
				if (way != 1 && way != 2 && way != 3) {
					break;
				}

				if (act== 6 || act == 7) {
					if (n >= I.size()) {
						cout << "\nIncorrect" << endl;
					} else {
						if (act == 6) {
							P = der(I[n]);
						} else {
							P = same_roots(I[n]);
						}
						
						if (way == 1 || way == 3) {
							cout << "\n" << P << endl;
						} 
						if (way == 2 || way == 3) {
							I += P;
						}
					}
				} else {
					if (n >= I.size() || m >= I.size()) {
                                                cout << "\nIncorrect" << endl;
                                        } else {
						if ( act == 1) {
							op = "+";
							P = I[n] + I[m];
						} else if (act == 2) {
							op = "-";
							P = I[n] - I[m];
						} else if (act == 3) {
							op = "*";
							P = I[n]*I[m];
						} else if (act == 4) {
							op = "/";
							P = I[n]/I[m];
						} else if (act == 5) {
							op = " LCD ";
							P = NOD(I[n], I[m]);
						}

						if (way == 1 || way == 3) {
							cout << "\n" << I[n] << op << I[m] << "=" << P << endl;
						}
						if (way == 2 || way == 3) {
							I += P;
						}
					}
				}
			}
		}
	}
	return 0;
}
