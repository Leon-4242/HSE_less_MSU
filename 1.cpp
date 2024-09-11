#include "1.h"
#define N = 10

using namespace CintN {
	CintN(char* arr[N]) {
		for (int i = 0; i < N; i++) {
			val[i] = arr[i];
		}
	}

		CintN(int value = 0) {
			int k {};
			while (value > 0 && k < N) {
				val[k] = value%10;
				value /= 10;
				k++;
			}
		}
		
		~CintN(){}
		
		CintN(const CintN &v) {
			for (int i = 0; i < N; i++) {
				val[i] = v.val[i];
			}
		}
		
		CintN operator+ (const CintN &v) const {
			int sum = 0; char *arr[N] {};
			for (int i = 0; i < N; i++) {
				sum = val[i] + v.val[i] + arr[i];
				if (sum > 9 && i < N-1) {
					sum /= 10;
					arr[i+1] += 1;
				}
				if (sum > 9 && i == N-1) {
					sum /= 10;
				}
				arr[i] = sum;
			}


			return CintN(arr);
		}
		
		CintN operator- (const CintN &v) const{
			int dif = 0; int *arr[N] {};
			if (v > this* || v == this*) {
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
		
		bool operator > (const CintN &v) const {
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
		
		bool operator == (const CintN &v) const {
			for (int i = 0; i < N; i++) {
				if(val[i] != v.val[i]) {
					return false;
				}
			}
			return true;
		} 
		
		std::string print() const{
			std::string number;
			for (int i = 0; i < N; i++) {
				number.insert(0, val[i]+48);
			}

			return number;
		}
		
}

