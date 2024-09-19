#include "interface.h"

using namespace std;
int func(void) {
	int flag_action, flag_data1, flag_data2;
	bool flag_a, flag_d1, flag_d2;
	srand(time(NULL));

	while (true) {
		cout << "Choose action:\n1)Addition        2)Substraction\n" << endl;
		cin >> flag_action;
		if (flag_action == 1) {
			flag_a = true;
		} else if (flag_action == 2) {
			flag_a = false;
		} else {
			cout << "\nYou choose exit\n" << endl;
			return 0;
		}

		cout << "\nChoose way for input data (first number):\n1)Generation       2)Enter\n" << endl;
		cin >> flag_data1;
		if (flag_data1 == 1) {
			flag_d1 = true;
		} else if (flag_data1 == 2) {
			flag_d1 = false;
		} else {
			cout << "\nYou choose exit\n" << endl;
			return 0;
		}

		cout << "\nChoose way for input data (second number):\n1)Generation       2)Enter\n" << endl;
                cin >> flag_data2;
                if (flag_data2 == 1) {
                        flag_d2 = true;
                } else if (flag_data2 == 2) {
                        flag_d2 = false;
                } else {
                        cout << "\nYou choose exit\n" << endl;
                        return 0;
                }

		CIntN num1, num2;
		if (flag_d1) {
			cout << "\nEnter length of string:\n" << endl;
			int n;
			cin >> n;
			if (n < 0 || n > N) {
				cout << "\nIncorrect data.\n" << endl;
				return 1;
			}

			num1.get(n);
		} else {
			cout << "\nEnter first number:\n" << endl;
			char* tmp = new char[N];
			cin >> tmp;
			
			if (strlen(tmp) > N) {
				cout << "\nIncorrecrt data\n" << endl;
				return 1;
			}
			for (size_t i = 0; i < strlen(tmp); i++) {
				if (!isdigit(tmp[i])) {
					cout << "\nIncorrecrt data\n" << endl;
 	                        	return 1;
				}
			}
			CIntN p(tmp);
			delete[] tmp;
			num1 = p;
		}

		if (flag_d2) {
			cout << "\nEnter length of string:\n" << endl;
                        int n;
                        cin >> n;
                        if (n < 0 || n > N) {
                                cout << "\nIncorrect data.\n" << endl;
                                return 1;
                        }

                        num2.get(n);
                } else {
                        cout << "\nEnter second number:\n" << endl;
                        char* tmp = new char[N] {};
                        cin >> tmp;
			if (strlen(tmp) > N) {
                                cout << "\nIncorrecrt data\n" << endl;
                                return 1;
                        }

                        for (size_t i = 0; i < strlen(tmp); i++) {
                                if (!isdigit(tmp[i])) {
                                        cout << "\nIncorrecrt data\n" << endl;
                                        return 1;
                                }
                        }
                        CIntN p(tmp);
                        delete[] tmp;
                        num2 = p;

                }
		char *tmp = num1.print();
		cout << tmp << (flag_a ? "+" : "-");
		delete[] tmp;
		tmp = num2.print();
		cout << tmp << "=";
	       	delete[] tmp;
		tmp = (flag_a ? num1+num2 : num1-num2).print();
		cout << tmp << endl;
		delete[] tmp;

	}
}
