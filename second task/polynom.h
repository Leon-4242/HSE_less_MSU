#ifndef LIB
#define LIB

#include <vector>
#include "../class/complex.h"

namespace poly {
	using namespace comp;

	class polynom {
		std::vector<Complex> coef;

		public:

		friend polynom operator+ (const polynom& P, const polynom& Q);
		friend polynom operator- (const polynom& P, const polynom& Q);
        	friend polynom operator* (const polynom& P, const polynom& Q);
        	friend polynom operator/ (const polynom& P, const polynom& Q);
        	friend polynom operator% (const polynom& P, const polynom& Q);
		
		friend bool operator== (const polynom& P, const polynom& Q);
        	friend bool operator!= (const polynom& P, const polynom& Q);
	
	        friend polynom operator+ (const polynom& P);
	        friend polynom operator- (const polynom& P);

	        friend polynom NOD (const polynom& P, const polynom& Q);
	        friend polynom der (const polynom& P);
        	friend polynom same_roots(const polynom& P);
	
		friend std::string print(const polynom& P);
		friend std::ostream& operator<< (std::ostream& os, const polynom& P);

		polynom() {
			coef.push_back(0);
		}

		polynom(std::vector<Complex> val) {
			bool flag = true;
			for (auto iter = val.begin(); iter != val.end(); ++iter) {
				if (flag) {
					if (*iter != 0) {
						flag = false;
						coef.push_back(*iter);
					}
				} else {
					coef.push_back(*iter);
				}
			}
			if (coef.empty()) {
				coef.push_back(0);
			}
		}

		polynom(Complex x, int n = 0) {
			coef.push_back(x);
			if (x != 0) {
				for (int i = 0; i < n; i++) {
					coef.push_back(0);
				}
			}
		}

		polynom(const polynom& P) {
			auto tmp = P.getCoef();
			coef.insert(coef.begin(), tmp.begin(), tmp.end());
		}

		std::vector<Complex> getCoef (void) const {
			std::vector<Complex> tmp;
		
			for (auto iter = coef.begin(); iter != coef.end(); ++iter) {
				tmp.push_back(*iter);
			}

			return tmp;
		}

		polynom& operator= (const polynom& P) {
			auto tmp = P.getCoef();
			coef.clear();
			coef.insert(coef.begin(), tmp.begin(), tmp.end());
			return *this;
		}

		polynom& operator+= (const polynom& P) {
			*this = *this + P;			
			return *this;
		}

		polynom& operator-= (const polynom& P) {
			*this = *this - P;
			return *this;
		}

		polynom& operator*= (const polynom& P) {
			*this = *this * P;
			return *this;
		}

		polynom& operator/= (const polynom& P) {
			*this = *this/P;
			return *this;
		}

		polynom& operator%= (const polynom& P) {
			*this  = *this % P;
			return *this;
		}

		friend std::istream& operator>> (std::istream& is, polynom& P);

		Complex operator() (Complex x) const;
	};
	
	polynom operator+ (const polynom& P, const polynom& Q) {
		auto i1 = P.coef.rbegin(), i2 = Q.coef.rbegin();
		std::vector<Complex> res; 
		while (i1 != P.coef.rend() && i2 != Q.coef.rend()) {
			res.insert(res.begin(), *i1+*i2);
			++i1; ++i2;
		}

		while (i1 != P.coef.rend()) {
			res.insert(res.begin(), *i1);
			++i1;	
		}

		while (i2 != Q.coef.rend()) {
                        res.insert(res.begin(), *i2);
                        ++i2;
                }
		
		return polynom(res);	
	}

	polynom operator- (const polynom& P, const polynom& Q) {
                auto i1 = P.coef.rbegin(), i2 = Q.coef.rbegin();
                
		std::vector<Complex> res;
                while (i1 != P.coef.rend() && i2 != Q.coef.rend()) {
                        res.insert(res.begin(), *i1-*i2);
                        ++i1; ++i2;
                }

                while (i1 != P.coef.rend()) {
                        res.insert(res.begin(), *i1);
                        ++i1;
                }

                while (i2 != Q.coef.rend()) {
                        res.insert(res.begin(), -*i2);
                        ++i2;
                }

		auto iter = res.begin();
		while (iter != res.end()) {
			if (*iter != 0) {
				break;
			} else {
				++iter;
			}		
		}
		if (iter != res.begin()) {
			if (iter == res.end()) {
				return polynom();
			} else {
				res.erase(res.begin(), iter-1);
			}
		}		
		return polynom(res);

	}

	polynom operator* (const polynom& P, const polynom& Q) {
		auto val1 = P.coef, val2 = Q.coef;

		if (val1.size() == 1) {
			for (auto iter = val2.begin(); iter != val2.end(); ++iter) {
				(*iter) *= val1[0];
			}
			return polynom(val2);
		}

		for (auto iter = val2.begin(); iter != val2.end(); ++iter) {
			(*iter) *= val1[0];
		}
		for (size_t i = 0; i < val1.size()-1; ++i) {
			val2.push_back(0);
		}
		val1.erase(val1.begin());
		return polynom(val2) + polynom(val1)*Q;
	}

	polynom operator/ (const polynom& P, const polynom& Q) {
		size_t deg1 = P.coef.size()-1, deg2 = Q.coef.size()-1;
		if (deg1 < deg2) {
			return polynom();
		}

		polynom q1(P.coef[0]/Q.coef[0], deg1-deg2);
		polynom r1 = P-Q*q1;
		return q1+(r1/Q);
	}

	polynom operator% (const polynom& P, const polynom& Q) {
		return P- (P/Q)*Q;
	}
	
	bool operator== (const polynom& P, const polynom& Q) {
		if (P.coef.size() != Q.coef.size()) {
			return false;
		}

		for (size_t i = 0; i < P.coef.size(); ++i) {
			if (P.coef[i] != Q.coef[i]) {
				return false;
			}
		}

		return true;
	}

	bool operator!= (const polynom& P, const polynom& Q) {
		return !(P==Q);
	}

	polynom operator+ (const polynom& P) {
		return polynom(P.coef);
	}
        polynom operator- (const polynom& P) {
		auto val = P.coef;
		for (auto iter = val.begin(); iter != val.end(); ++iter) {
			*iter *= -1.;
		}

		return polynom(val);
	}

	polynom NOD (const polynom& P, const polynom& Q) {
		if (P.coef.size() < Q.coef.size()) {
			return NOD(Q, P);
		}
		if (P%Q == polynom()) {
			return polynom(Q.coef);
		}
		// P = (P/Q) *Q + (P%Q)
		// NOD(P, Q) = NOD (P%Q, Q);
		return NOD(Q, P%Q);
	}

	polynom der (const polynom& P) {
		auto val = P.coef;
		if (val.size() <= 1) {
			return polynom();
		}
		val.pop_back();
		for (size_t i = 0; i < val.size(); ++i) {
			val[i] *= (int)(val.size()-i);
		}

		return polynom(val);
	}

	polynom same_roots(const polynom& P) {
		return P/NOD(P, der(P));
	}

	Complex polynom::operator() (Complex x) const{
		Complex res {};
		for (size_t i = 0; i < coef.size(); ++i) {
			res += coef[i]*powCI(x, coef.size()-1-i);
		}
		return res;
	}

	std::string print(const polynom& P) {
		std::string str;

		for (auto iter = P.coef.begin(); iter != P.coef.end(); ++iter) {
			if (iter != P.coef.begin()) {
				str += "+";
			}
			if ((P.coef.end()-iter) == 2) {
				str += "("+print(*iter) + ")*z";
			} else if ((P.coef.end()-iter) == 1) {
				str += "("+print(*iter) + ")";
			} else {
				str += "("+print(*iter) + ")*z^" + std::to_string((P.coef.end()-iter)-1);
			}
		}

		return str;
	}

	std::ostream& operator<< (std::ostream& os, const polynom& P) {
		return os << print(P);
	}

	std::istream& operator>> (std::istream& is, polynom& P) {
		int deg;
		is >> deg;
		P.coef.clear();
		if (deg == 0) {
			P.coef.push_back(0);
			return is;
		}
		Complex tmp;
		for (int i = 0; i < deg+1; ++i) {
			is >> tmp;
		       	P.coef.push_back(tmp);
		}
		size_t counter = 0;
		for(auto iter = P.coef.begin(); iter != P.coef.end(); ++iter) {
			if (*iter == 0) {
				counter++;
			} else {
				break;
			}
		}
		if (counter == P.coef.size()) {
			P.coef.clear();
			P.coef.push_back(0);
			return is;
		}

		std::vector<Complex> buff;
		for (auto iter = P.coef.begin() +counter; iter != P.coef.end(); ++iter) {
			buff.push_back(*iter);
		}
		buff.swap(P.coef);
		buff.clear();

		return is;
	}
};

#endif
