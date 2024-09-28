#include "2.h"

namespace cint {
	 char* print (const CintN& num) {
                char* tmp = num.getVal();
                int n = num.getN();
                char* res = new char[n];
                for (int i = 1; i < n-1; ++i) {
                        res[i] = tmp[n-2-i];
                }
		res[0] = tmp[n-2];
                res[n-1] = 0;
                delete[] tmp;
                return res;
	}

	CintN operator+ (const CintN& n1, const CintN& n2) {
		bool sgn1 = sgn(n1), sgn2 = sgn(n2);
		if (!(sgn1 && sgn2)) {
			if (sgn1 == sgn2) {
				return -((-n1) + (-n2));
			} 
			if (sgn2) {
				return n2-(-n1);
			} else {
				return n1-(-n2);
			}
		}

		int n = ((n1.getN() > n2.getN()) ? n1.getN() : n2.getN()) + 1;
	       	
		char* tmp = new char[n];
		char* num1 = print(n1), *num2 = print(n2);
		tmp[0] = 43; tmp[1] = 0; tmp[n-1] = 0; int t = n-2, buff = 0;
		int i = n1.getN()-2, k = n2.getN()-2;
		while (i >= 1 && k >= 1) {
			tmp[t] = num1[i] - 48 + num2[k] - 48 + buff;
			buff  = tmp[t]/10;
			tmp[t] %= 10;
			tmp[t] += 48;
			--t; --i; --k;
		}
		if (n1.getN() == n2.getN()) {
			if (buff) {
				tmp[1] = 49;
			}
		}

		while (i >= 1) {
			tmp[t] = num1[i] - 48 + buff;
			buff = tmp[t]/10;
			tmp[t] %= 10;
			tmp[t] += 48;
			--t; --i;
		}
		while (k >= 1) {
			tmp[t] = num2[k] - 48 + buff;
                        buff = tmp[t]/10;
                        tmp[t] %= 10;
                        tmp[t] += 48;
                        --t; --k;

		}

		delete[] num1; delete[] num2;
		if (tmp[1] == 0) {
			char* buff = new char[n-1];
			buff[0] = 43;
			for (int i = 1; i < n-1; ++i) {
				buff[i] = tmp[i+1];
			}

			delete[] tmp;
			tmp = buff;
		}
		CintN res(tmp);
		delete[] tmp;

		return res;
	}
	
	CintN operator- (const CintN& num1, const CintN& num2) {
		bool sgn1 = sgn(num1), sgn2 = sgn(num2);
		if (!(sgn1 && sgn2)) {
			if (sgn1 == sgn2) {
				return -((-num1) + (-num2));
			}
			if (sgn1) {
				return num1+(-num2);
			} else {
				return -((-num1)+num2);
			}
		}

		if (num1 == num2) return CintN();
		if (num1 < num2) return -(num2-num1);

		int n = num1.getN();
		char* tmp = new char[n];
		for (int i = 1; i < n; ++i) tmp[i] = 0;
		tmp[0] = 43;
		char* str1 = print(num1), *str2 = print(num2);
		int t = n-2; int buff = 0;
		int i = num1.getN() - 2, k = num2.getN() - 2;
		while (i >= 1 && k >= 1) {
			buff = str1[i] - str2[k] - buff;
		       	if (buff < 0) {
				tmp[t] = buff + 10;
				buff = 1;
			} else {
				tmp[t] = buff;
				buff = 0;
			}

			tmp[t] += 48;
			--t; --i; --k;
		}
		while (i >= 1) {
			if (buff) {
				tmp[t] = str1[i] -1;
				buff = 0;
				--i; --t;
				continue;
			}
			tmp[t] = str1[i];
			--i; --t;
		}
		delete[] str1; delete[] str2;

		int counter = 0;
		for (int i = 1; i < n-1; ++i) {
			if (tmp[i] == 0 || tmp[i] == '0') ++counter;
		       	else break;
		}
		
		if (counter) {
			char* buff = new char[n-counter];
			buff[0] = 43;
			for (int i = 1; i < n-counter; ++i) {
				buff[i] = tmp[i+counter];
			}
			delete[] tmp;
			tmp = buff;
		}	
		
		CintN res(tmp);
		delete[] tmp;

		return res;
	}

	CintN operator* (const CintN& num1, const CintN& num2) {
		bool sgn1 = sgn(num1), sgn2 = sgn(num2);
		char* str1 = print((sgn1) ? num1 : -num1), *str2 = print((sgn2) ? num2 : -num2);
		int a = str1[num1.getN()-2];
		CintN s;
		for (int i = 0; i < a - 48; ++i) {
			s += (sgn2) ? num2 : -num2;	
		}

		if (num1.getN() == 3) {
			delete[] str1; delete[] str2;
			return (sgn1 == sgn2) ? s : -s;
		}
		char* str = new char[num1.getN()-1];
		for (int i = 0; i < num1.getN()-2; ++i) {
			str[i] = str1[i];
		}
		str[num1.getN()-2] = 0;
		delete[] str1;
		str1 = str;

		str = new char[num2.getN()+1];
		for (int i = 0; i < num2.getN()-1; ++i) {
			str[i] = str2[i];
		}
		str[num2.getN()-1] = 48;
		str[num2.getN()] = 0;
		delete[] str2;
		str2 = str;
		//std::cout << strlen(str1) << "   " << str2 << std::endl;	
		CintN res = CintN(str1)*CintN(str2) + s;
		delete[] str1; delete[] str2;

		return (sgn1 == sgn2) ? res : -res;
	}

	CintN operator- (const CintN& num) {
		char* tmp = print(num);
		tmp[0] = 88-tmp[0];
		CintN res(tmp);
		delete[] tmp;

		return res;
	}
	
	CintN operator+ (const CintN& num) {
		return CintN(num);
	}

	bool operator== (const CintN& num1, const CintN& num2) {
		int n1 = num1.getN(), n2 = num2.getN();
		if (n1 != n2) {
			return false;
		}

		char* str1 = num1.getVal(), *str2 = num2.getVal();

		for (int i = 0; i < n1; ++i) {
			if (str1[i] != str2[i]){
				delete[] str1; delete[] str2;       
				return false;
			}
		}

		return true;
	}

        bool operator!= (const CintN& num1, const CintN& num2) {
		return !(num1 == num2);
	}

        bool operator> (const CintN& num1, const CintN& num2) {
		bool sgn1 = sgn(num1), sgn2 = sgn(num2);
		if (sgn1 && !sgn2) {
			return true;
		} else if (!sgn1 && sgn2) {
			return false;
		} else if (!sgn1 && !sgn2) {
			return -num2 > -num1;
		}

		int n1 = num1.getN(), n2 = num2.getN();
		if (n1 > n2) {
			return true;
		} else if (n1 < n2) {
			return false;
		}

		char* str1 = num1.getVal(), *str2 = num2.getVal();

		for (int i = n1-3; i >= 0; --i) {
			if (str1[i] > str2[i]) {
				delete[] str1; delete[] str2;
				return true;
			} else if (str1[i] < str2[i]) {
				delete[] str1; delete[] str2;
				return false;
			}
		}
		
		delete[] str1; delete[] str2;
		return false;	
	}

        bool operator< (const CintN& num1, const CintN& num2) {
		return num1 <= num2 && !(num1 == num2);
	}
        bool operator>= (const CintN& num1, const CintN& num2) {
		return !(num1 < num2);
	}
        bool operator<= (const CintN& num1, const CintN& num2) {
		return !(num1 > num2);
	}

	std::ostream& operator<< (std::ostream& os, const CintN& num) {
		char *tmp = print(num);
		os << '(' << tmp << ')';
		delete[] tmp;
		return os;
	}

	std::istream& operator>> (std::istream& is, CintN& num) {
		char c, *str; int k = 1;
		str  = new char[k];
		str[0] = 0;
		is >> c;
		while (c != 0 && c != ' ' && c != '\n') {
			char* buff = new char[k+1];
			for (int i = 0; i < k-1; ++i) {
				buff[i] = str[i];
			}
			buff[k-1] = c;
			buff[k] = 0;
			delete[] str;
			str = buff;
			++k;
			buff = nullptr;
			is.get(c);
		}
		num = CintN(str);
		delete[] str;
		return is;
	}
//constructors

	CintN::CintN(void) {
		N = 3;
		val = new char[N];
		val[0] = 48;
		val[1] = 43;
		val[2] = 0;	
	}

	CintN::CintN(char* value) {
		if (value == nullptr) {
			std::cout << "A";
			exit(-1);
		}

		N = strlen(value) + 1;
		bool flag = (value[0] == 43 || value[0] == 45) ? true : false;
		if ((flag && N <= 2) || (!flag && N <= 1)) {
			std::cout << "B";
			exit(-1);
		}
		
		if (flag) {
			val = new char[N];
			val[N-2] = value[0];
		} else {
			N++;
			val = new char[N];
			val[N-2] = 43;
		}
		int i = N-3;
		for (size_t k = (flag) ? 1 : 0; k < strlen(value); ++k) {
			if (i < 0) break;
			if(!isdigit(value[k])) {
				delete[] val;
				std::cout << "C";
				exit(-1);
			} 
			val[i] = value[k];
			--i;
		}
		val[N-1]= 0;
			
	}

	CintN::CintN(int n) {
		if (n == 0) {
			N = 3;
			val = new char[3];
			val[0] = 48;
			val[1] = 43;
			val[2] = 0;
			return;
		}
		N = 3;
		val = new char[N];
		val[N-1] = 0;
		val[N-2] = (n > 0) ? 43 : 45;
		int num = n;
		if (n < 0) num *= -1;
		int t = 0;
		while ( num > 0) {
			val[t] = (num%10) + 48; ++t;
			num /= 10;
			if (num > 0) {
				++N;
				char *tmp = new char[N];
				tmp[N-1] = 0;
				tmp[N-2] = val[N-3];
				for (int i = 0; i < t; ++i) {
					tmp[i] = val[i];
				}
				delete[] val;
				val = tmp;
			}
		}


	}

	CintN::CintN(int n, bool sign) {
		if (n < 1) {
			exit(-1);
		}

		N = n+2;
		val = new char[N];
		val[N-2] = sign ? 43 : 45;
		val[N-1] = 0;
		val[N-3] = rand()%9 +1 + 48;
		for (int i = N-4; i >= 0; --i) {
			val[i] = rand()%10 + 48;
		}
	}

	CintN::CintN(const CintN& num) {
		N = num.getN();
		val = num.getVal();
	}

//destuctor

	CintN::~CintN(void) {
		delete[] val;
	}

//copy-constructor
	
	CintN& CintN::operator= (const CintN& num) {
		N = num.getN();
		delete[] val;
		val = num.getVal();
		return *this;
	}
	
	CintN& CintN::operator+= (const CintN& num) {
		*this = *this + num;
		return *this;
	}

        CintN& CintN::operator-= (const CintN& num) {
		*this = *this - num;
		return *this;
	}

	CintN& CintN::operator*= (const CintN& num) {
		*this = (*this) * num;
		return *this;
	}
	
	CintN& CintN::operator++ (void) {
		*this += 1;
		return *this;
	}

	CintN CintN::operator++ (int) {
		CintN copy(*this);
		++(*this);
		return copy;
	}

	CintN& CintN::operator-- (void) {
                *this -= 1;
                return *this;
        }

        CintN CintN::operator-- (int) {
                CintN copy(*this);
                --(*this);
                return copy;
        }

//getValue functions

	char* CintN::getVal(void) const {
		char* res = new char[N];
		for (int i = 0; i < N; ++i) {
			res[i] = val[i];
		}
		return res;
	}

	int CintN::getN(void) const{
		return N;
	}

	bool sgn(const CintN& num) {
		char* tmp = num.getVal();
		bool flag = (tmp[num.getN()-2] == 43) ? true : false;
		delete[] tmp;
		return flag;
	}
//

}

