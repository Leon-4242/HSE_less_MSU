#ifndef LIB
#define LIB

#include <vector>
#include "../class/complex.h"

namespace poly {
	using namespace comp;
	class polynom;

	polynom operator+ (const polynom& P, const polynom& Q);
	polynom operator- (const polynom& P, const polynom& Q);
	polynom operator* (const polynom& P, const polynom& Q);
	polynom operator/ (const polynom& P, const polynom& Q);
	polynom operator% (const polynom& P, const polynom& Q);	
	
	bool operator== (const polynom& P, const polynom& Q);
	bool operator!= (const polynom& P, const polynom& Q);
	
	polynom operator+ (const polynom& P);
	polynom operator- (const polynom& P);

	polynom NOD (const polynom& P, const polynom& Q);
	polynom der (const polynom& P);
	class polynom {
		std::vector<Complex> coef;

		public:
		
		polynom() {
			coef.push_back(0);
		}

		polynom(std::vector<Complex> val) {
			coef.insert(coef.begin(), val.begin(), val.end()); 
		}

		polynom(Complex x, int n = 0) {
			coef.push_back(x);
			for (int i = 0; i < n; i++) {
				coef.push_back(0);
			}
		}

		std::vector<Complex> getCoef (void) const {
			return coef;
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
	};

	polynom operator+ (const polynom& P, const polynom& Q) {
		auto i1 = P.getCoef().rbegin(), i2 = Q.getCoef().rbegin();
		std::vector<Complex> res; 
		while (i1 != P.getCoef().rend() && i2 != Q.getCoef().rend()) {
			res.insert(res.begin(), *i1+*i2);
			++i1; ++i2;
		}

		while (i1 != P.getCoef().rend()) {
			res.insert(res.begin(), *i1);
			++i1;	
		}

		while (i2 != Q.getCoef().rend()) {
                        res.insert(res.begin(), *i2);
                        ++i2;
                }
		
		return polynom(res);	
	}

	polynom operator- (const polynom& P, const polynom& Q) {
		auto i1 = P.getCoef().rbegin(), i2 = Q.getCoef().rbegin();
                std::vector<Complex> res;
                while (i1 != P.getCoef().rend() && i2 != Q.getCoef().rend()) {
                        res.insert(res.begin(), *i1-*i2);
                        ++i1; ++i2;
                }

                while (i1 != P.getCoef().rend()) {
                        res.insert(res.begin(), *i1);
                        ++i1;
                }

                while (i2 != Q.getCoef().rend()) {
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
		auto val1 = P.getCoef(), val2 = Q.getCoef();

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
		size_t deg1 = P.getCoef().size(), deg2 = Q.getCoef().size();
		if (deg1 < deg2) {
			return polynom();
		}

		polynom q1(P.getCoef()[0]/Q.getCoef()[0], deg1-deg2);
		polynom r1 = P-Q*q1;
		return q1+(r1/Q);
	}

	polynom operator% (const polynom& P, const polynom& Q) {
		return P- (P/Q)*Q;
	}
	
	bool operator== (const polynom& P, const polynom& Q) {
		if (P.getCoef().size() != Q.getCoef().size()) {
			return false;
		}

		for (size_t i = 0; i < P.getCoef().size(); ++i) {
			if (P.getCoef()[i] != Q.getCoef()[i]) {
				return false;
			}
		}

		return true;
	}

	bool operator!= (const polynom& P, const polynom& Q) {
		return !(P==Q);
	}

	polynom operator+ (const polynom& P) {
		return polynom(P.getCoef());
	}
        polynom operator- (const polynom& P) {
		auto val = P.getCoef();
		for (auto iter = val.begin(); iter != val.end(); ++iter) {
			*iter *= -1.;
		}

		return polynom(val);
	}

	polynom NOD (const polynom& P, const polynom& Q) {
		if (P.getCoef().size() < Q.getCoef().size()) {
			return NOD(Q, P);
		}
		if (P%Q == polynom()) {
			return polynom(Q.getCoef());
		}
		// P = (P/Q) *Q + (P%Q)
		// NOD(P, Q) = NOD (P%Q, Q);
		return NOD(Q, P%Q);
	}

	polynom der (const polynom& P) {
		auto val = P.getCoef();
		if (val.size() == 1) {
			return polynom();
		}
		val.pop_back();
		for (size_t i = 0; i < val.size(); ++i) {
			val[i] *= i;
		}

		return polynom(val);
	}
};

#endif
