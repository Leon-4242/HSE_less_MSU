#ifndef Polynom
#define Polynom

#include "DOUB.h"
#include "LIST.h"

namespace poly {
    using namespace LIST;
    using namespace DOUB;
    class polynom {
        List<doub> coef;
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

        polynom(void): coef() {}

        polynom(polynom&& P) {
            coef = P.coef;
            P.coef.clear();
        }

        polynom(const List<doub>& lst) {
            bool flag = true;
            for (auto iter = lst.begin(); iter != lst.end(); ++iter) {
                if (flag) {
                    if (*iter != 0.0) {
                        flag = false;
                        coef.pushBack(*iter);
                    }
                } else coef.pushBack(*iter);
            }
            if (coef.empty()) coef.pushBack(0);
        }

        polynom(const doub& x, int n = 0) {
            coef.pushBack(x);
            if (x != 0) for (int i = 0; i < n; i++) coef.pushBack(0);
        }

        polynom(const polynom& P): coef(P.coef) {}

        const List<doub>& getCoef(void) const {return coef;}
        List<doub>& getCoef(void) {return coef;}

        polynom& operator= (const polynom& P) {
            coef = P.coef;
            return *this;
        }

        polynom& operator= (polynom&& P) {
            coef = P.coef;
            P.coef.clear();
            return *this;
        }

        polynom& operator += (const polynom& P) {
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

        doub operator() (const doub& x) const;
    };
}
#endif
