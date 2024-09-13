#include "1.h"
CintN::CintN(char arr[N]) {
	for (int i = 0; i < N; i++) {
		val[i] = arr[i];
	}
}

CintN::CintN(int value = 0) {
	int k {};
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
	char sum = 0; char arr[N] {};
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
	int dif = 0; char arr[N] {};
	if ((v > *this) || (v == *this)) {
		for(int i = 0; i < N; i++){
			arr[i] = 0;
		}
	}
	else {
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
		
bool CintN::operator > (const CintN &v) const {
	// val > v.val  return true
	for (int i = N-1; i >= 0; i++) {
		if (val[i] < v.val[i]){
			return false;
		} else if (val[i] > v.val[i]) {
			return true;
		}
	}
	return false;	
}
		
bool CintN::operator == (const CintN &v) const {
	for (int i = 0; i < N; i++) {
		if(val[i] != v.val[i]) {
			return false;
		}
	}
	return true;
} 
		
std::string CintN::print() const {
	bool flag = true;
	std::string res;

	for (int i = N-1; i >= 0; i--) {
		if (flag) {
			if (val[i] != 0) {
				flag = false;
				res.push_back(val[i]+48);
			}
		} else {
			res.push_back(val[i]+48);
		}
	}
	
	return res;
}
		

