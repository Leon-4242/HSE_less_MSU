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
	
	
	polynom operator+ (const polynom& P);
	polynom operator- (const polynom& P);

	class polynom {
		std::vector<Complex> coef;

		public:
		
		polynom() {
			coef.push_back(0);
		}

		polynom(std::vector<Complex> val) {
			coef.insert(coef.begin(), val.begin(), val.end()); 
		}

		polynom(Complex x) {
			coef.push_back(x);
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

};

#endif
