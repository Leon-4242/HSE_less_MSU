#include "polynom.h"

namespace poly {
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

}
