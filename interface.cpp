#include "interface.h"
#include "task1.h"

using namespace std;
int func(void) {
	int flag_action, flag_data1, flag_data2;
	bool flag_a, flag_d1, flag_d2;
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
			num1.get();
		} else {
			cout << "\nEnter first number:\n" << endl;
			int tmp;
			cin >> tmp;
			if (tmp < 0) {
				cout << "\nIncorrecrt data\n" << endl;
				return 1;
			}
			CIntN p(tmp);
			num1 = p;
		}

		if (flag_d2) {
                        num2.get();
                } else {
                        cout << "\nEnter second number:\n" << endl;
                        int tmp;
                        cin >> tmp;
                        if (tmp < 0) {
                                cout << "\nIncorrecrt data\n" << endl;
                                return 1;
                        }
                        CIntN p(tmp);
                        num2 = p;
                }

		cout << num1.print() << (flag_a ? "+" : "-") << num2.print() << "=" << (flag_a ? num1+num2 : num1-num2).print() << endl;

	}
}
