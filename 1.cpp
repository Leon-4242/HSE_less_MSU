#include "1.h"
#define N = 10

class CintN {
	char * val[N] {};
	
	public:
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
		
		CintN operator+ (const CintN &v) {
			int sum = 0;
			for (int i = 0; i < N; i++) {
				sum = val[i] + v.val[i];
				if (sum > 9 && i < N-1) {
					sum /= 10;
					val[i+1] += 1;
				}
				if (sum > 9 && i == N-1) {
					sum /= 10;
				}
				val[i] = sum;
			}
		}
		
		CintN operator- (const CintN &v) {
			int dif = 0;
			if (v > this* || v == this*) {
				for(int i = 0; i < N; i++){
					val[i] = 0;
				}
			}
			else {
				for (int i = 0; i < N; i++) {
					dif = val[i] - v.val[i];
					if (dif < 0 && i < N - 1){
						dif += 10;
						val[i+1]--;		
					}
					val[i] = dif;
				}
			}
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
		
		
		
};

