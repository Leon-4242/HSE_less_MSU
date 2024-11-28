#ifndef Polynom
#define Polynom

#include "DOUB.h"
#include "LIST.h"

namespace poly {
    using namespace LIST;
    using namespace DUOB;
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
                    if (*iter != 0) {
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

    polynom operator+ (const polynom& P, const polynom& Q) {
        auto i1 = P.coef.rbegin(), i2 = Q.coef.rbegin();
        List<doub> res; 
        while (i1 != P.coef.rend() && i2 != Q.coef.rend()) {
            res.pushHead(*i1+*i2);
            ++i1; ++i2;
        }

        while (i1 != P.coef.rend()) {
            res.pushHead(*i1);
            ++i1;
        }

        while (i2 != Q.coef.rend()) {
            res.pushHead(*i2);
            ++i2;
        }
        return polynom(res);
    }

    polynom operator- (const polynom& P, const polynom& Q) {
        auto i1 = P.coef.rbegin(), i2 = Q.coef.rbegin();
        List<doub> res;

        while (i1 != P.coef.rend() && i2 != Q.coef.rend()) {
            res.pushHead(*i1-*i2);
            ++i1; ++i2;
        }

        while (i1 != P.coef.rend()) {
            res.pushHead(*i1);
            ++i1;
        }

        while (i2 != Q.coef.rend()) {
            res.pushHead(-*i2);
            ++i2;
        }
        return polynom(res);
    }

    polynom operator* (const polynom& P, const polynom& Q) {
        auto val1 = P.coef, val2 = Q.coef;
        doub buff = *(val1.begin());

        for (auto iter = val2.begin(); iter != val2.end(); ++iter) (*iter) *= buff;
        if (val1.length() == 1) return polynom(val2);

        for (size_t i = 0; i < val1.length()-1; ++i) val2.pushBack(0);
        val1.popHead();
        return polynom(val2) + polynom(val1)*Q;
    }

    polynom operator/ (const polynom& P, const polynom& Q) {
        size_t deg1 = P.coef.length()-1, deg2 = Q.coef.length()-1;
        if (deg1 < deg2) return polynom();

        polynom q1(*(P.coef.begin())/ *(Q.coef.begin()), deg1-deg2);
        polynom r1 = P-Q*q1;
        return q1+(r1/Q);
    }

    polynom operator% (const polynom& P, const polynom& Q) {return P- (P/Q)*Q;}

    bool operator== (const polynom& P, const polynom& Q) {
        if (P.coef.length() != Q.coef.length()) return false;
        for (auto iter = P.coef.begin(), it = Q.coef.begin(); iter != P.coef.end(); ++iter, ++it) if (*iter != *it) return false;
        return true;
    }

    bool operator!= (const polynom& P, const polynom& Q) {return !(P==Q);}

    polynom operator+ (const polynom& P) {return polynom(P.coef);}

    polynom operator- (const polynom& P) {
        List<doub> tmp = P.coef;
        for (auto iter = tmp.begin(); iter != tmp.end(); ++iter) *iter *= -1.;
        return polynom(tmp);
    }

    polynom NOD (const polynom& P, const polynom& Q) {
        if (P.coef.length() < Q.coef.length()) return NOD(Q, P);
        if (P%Q == polynom()) return polynom(Q.coef);
        // P = (P/Q) *Q + (P%Q)
        // NOD(P, Q) = NOD (P%Q, Q);
        return NOD(Q, P%Q);
    }

    polynom der (const polynom& P) {
        auto val = P.coef;
        if (val.length() <= 1) return polynom();
        val.popBack();
        for (auto iter = val.begin(); iter != val.end(); ++iter) *iter *= (int)(val.length()-iter.Index());
        return polynom(val);
    }

    polynom same_roots(const polynom& P) {return P/NOD(P, der(P));}

    doub polynom::operator() (const doub& x) const{
        doub res{};
        for (auto iter = coef.begin(); iter != coef.end(); ++iter) res += (*iter)*powD(x, coef.length()-iter.Index()-1);
        return res;
    }

    std::string print(const polynom& P) {
        std::string str;
        for (auto iter = P.coef.begin(); iter != P.coef.end(); ++iter) {
            if (iter != P.coef.begin()) str += "+";
            if ((P.coef.end()-iter) == 2) str += "("+ (*iter).print() + ")*z";
            else if ((P.coef.end()-iter) == 1) str += "("+(*iter).print() + ")";
            else str += "("+(*iter).print() +"*z^" + std::to_string((P.coef.end()-iter)-1);
        }

        return str;
    }

    std::ostream& operator<< (std::ostream& os, const polynom& P) {return os << print(P);}
};
#endif
