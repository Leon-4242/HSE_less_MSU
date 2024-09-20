#include "inteface.h"

using namespace std;
void func(void) {
	bool error = false, flag_a, flag_d1, flag_d2;
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
		if (!flag_a && d2 > d1) {
			cout << "Wrong data: I don't know negative numbers! I always on positive!\n" << endl;
		} else {
			char op = (flag_a) ? '+' : '-' ;
			CintN res =(flag_a) ? (d1+d2) : (d1-d2);
			cout << d1 << op << d2 << '=' << res << endl;
		}
	}

}

bool input(bool &action, bool &data1, bool &data2) {
	int tmp;
	cout << "Choose action:\n1)Addition\n2)Substraction\n" << endl;
	cin >> tmp;
	if (choice(tmp, action)) {
		return true;
	}

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
		cout << "\nEnter length of string:\n" << endl;
		int n;
		cin >> n;
		if (n < 0 || n > N) {
			cout << "\nIncorrect data.\n" << endl;
			flag = true;
			return res;
		}
		res.generate(n);		
	} else {
		cout << "\nEnter number:\n" << endl;
		std::string tmp;
		cin >> tmp;	
		
		for (size_t i = 0; i < tmp.length(); i++) {
			if (tmp[i] != 0) {
				if (!isdigit(tmp[i])) {
					cout << "\nIncorrecrt data\n" << endl;
            	   			flag = true;
					return res;
				}
			}
		}
		res = CintN(tmp);
	}
	
	return res;
}
