#include "1.h"
CintN::CintN(char arr[N]) {
	for (int i = 0; i < N; i++) {
		val[i] = arr[i];
	}
	
}

CintN::CintN (std::string str) {
	bool flag = true;
	int k = str.length(); 
	int t = 0;
	for (int i = k-1; i >= 0; i--) {
		if (flag) {
			if (str[i] != 0) {
				flag = false;
			} else {
				continue;
			}
		}
		if (str[i] < 48 || str[i] > 57) {
			std::cout << "Wrong data." << std::endl;
			break;
		}
		if (t > N-1) {
			break;
		}
		val[t] = str[i] - 48;
		t++;
	}

	for (int i = t; i < N; i++) {
		val[i] = 0;
	}
}

CintN::CintN(int value = 0) {
	int k = 0;
	while (k < N) {
		val[k] = value%10;
		value /= 10;
		k++;
	}
}
		
CintN::~CintN(){}
		
CintN::CintN(const CintN &v) {
	for (int i = 0; i < N; i++) {
		val[i] = v.val[i];
	}
}

CintN CintN::operator+ (const CintN &v) const {
	char sum = 0; char arr[N];
	for (int i = 0; i < N; i++) {
		arr[i] = 0;
	}
	for (int i = 0; i < N; i++) {
		sum = val[i] + v.val[i] + arr[i];
		if (sum > 9 && i < N-1) {
			sum %= 10;
			arr[i+1] += 1;
		}
		if (sum > 9 && i == N-1) {
			sum %= 10;
		}
		arr[i] = sum;
	}
	return CintN(arr);
}
	
CintN CintN::operator- (const CintN &v) const{
	int dif = 0; char arr[N]; 
	for (int i = 0; i < N; i++) {
		arr[i] = 0;	
	}
	if (!((v > *this) || (v == *this))) {
		for (int i = 0; i < N; i++) {
			dif = val[i] - v.val[i] + arr[i];
			if (dif < 0 && i < N - 1){
				dif += 10;
				arr[i+1]--;		
			}
			arr[i] = dif;
		}
	}

	return CintN(arr);
}
		
bool CintN::operator> (const CintN &v) const {
	// val > v.val  return true
	for (int i = N-1; i >= 0; i--) {
		if (val[i] < v.val[i]){
			return false;
		} else if (val[i] > v.val[i]) {
			return true;
		}
	}
	return false;	
}
		
bool CintN::operator== (const CintN &v) const {
	for (int i = 0; i < N; i++) {
		if(val[i] != v.val[i]) {
			return false;
		}
	}
	return true;
} 

CintN& CintN::operator= (const CintN &v) {
	for (int i = 0; i < N; i++) {
		val[i] = v.val[i];
	}
	
	return *this;
}
		
std::string CintN::print() const{
	char number[N]; int k = 0; bool flag = true;
	for (int i = N-1; i >= 0; i--) {
		if (flag) {
			if (val[i] != 0) {
				flag = false;
			} else {
				continue;
			}
		}
		number[k] = val[i] + 48;
		k++;
	}
	for (int i = k; i < N; i++) {
		number[i] = 0;
	}
	std::string res(number);
	if(res.empty()) {
		res = "0";
	}
	return res;
}
		
CintN& CintN::generate(int n) {
	if (n < 0 || n > N) {
		std::cout << "\nIncorrect data.\n" << std::endl;
		return *this;
	}
	
	for (int i = 0; i < N; i++) {
		if (i > n-1) {
			val[i] = 0;
			continue;
		} else if (i == n-1) {
			val[i] = rand()%9 + 1;
			continue;
		}
		
		val[i] = rand()%10;
	}
	
	return *this;
}

std::ostream& operator<< (std::ostream& os, CintN ob) {
	return os << ob.print();
}
