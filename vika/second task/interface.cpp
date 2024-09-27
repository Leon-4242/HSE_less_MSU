#include "interface.h"

using namespace std;
using namespace cint;

void func(void) {
	bool error = false, flag_d1, flag_d2; int flag_a;
	srand(time(nullptr));
	while (true) {
		if (input(flag_a, flag_d1, flag_d2)) {
			break;
		}
		
		CintN d1 = make(flag_d1, error);
		if (error) {
			break;
		}
		CintN d2 = make(flag_d2, error);
		if (error) {
			break;
		}
		char op; CintN res;
		if (flag_a == 1) {
			op = '+';
			res = d1+d2;
		} else if (flag_a == 2) {
			op = '-';
			res = d1-d2;
		} else {
			op = '*';
			res = d1*d2;
		}
		cout << d1 << op << d2 << '=' << res << endl;
	}

}

bool input(int &action, bool &data1, bool &data2) {
	int tmp;
	cout << "Choose action:\n1)Addition\n2)Substraction\n3)multiplication" << endl;
	cin >> tmp;
	if (tmp != 1 && tmp != 2 && tmp != 3) {
		return true;
	}
	action = tmp;
	cout << "\nChoose way for input data (first number):\n1)Generation\n2)Enter\n" << endl;
	cin >> tmp;
	if (choice(tmp, data1)) {
		return true;
	}
	cout << "\nChoose way for input data (second number):\n1)Generation\n2)Enter\n" << endl;
	cin >> tmp;
    	if (choice(tmp, data2)) {
		return true;
	}
	
	return false;
}

bool choice(int tmp, bool &flag) {
	if (tmp == 1) {
		flag = true;
	} else if (tmp == 2) {
		flag = false;
	} else {
		cout << "\nYou choose exit.\n" << endl;
		return true;
	}
	
	return false;
}

CintN make(bool flag_d, bool &flag) {
	CintN res(0);
	if (flag_d) {
		cout << "\nEnter sign of number(-1 or 1):\n" << endl;
		int sgn;
		cin >> sgn;
		if (sgn != 1 && sgn != -1) {
			cout << "\nIncorrect data.\n" << endl;
                        flag = true;
                        return res;
		}
		bool sign = (sgn == 1) ? true : false;

		cout << "\nEnter length of string:\n" << endl;
		int n;
		cin >> n;
		if (n <= 0) {
			cout << "\nIncorrect data.\n" << endl;
			flag = true;
			return res;
		}
		return CintN(n, sign);		
	} else {
		cout << "\nEnter number:\n" << endl;
		cin >> res;
	}
	
	return res;
}
