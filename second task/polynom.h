#ifndef LIB
#define LIB

#include <vector>

namespace poly {
	class polynom;

	polynom operator+ (const polynom& P, const polynom& Q);
	
	class polynom {
		std::vector<double> coef;

		public:
		
		polynom() {
			coef.push_back(0);
		}

		polynom(std::vector<double> val) {
			coef.insert(coef.begin(), val.begin(), val.end()); 
		}

		polynom(double x) {
			coef.push_back(x);
		}

		std::vector<double> getCoef (void) const {
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
	};

	polynom operator+ (const polynom& P, const polynom& Q) {
		auto i1 = P.getCoef().rbegin(), i2 = Q.getCoef().rbegin();
		std::vector<double> res; 
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


};

#endif
