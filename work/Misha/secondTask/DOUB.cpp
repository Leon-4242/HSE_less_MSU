#include "DOUB.h"
namespace DOUB {
    std::ostream& operator<< (std::ostream& os, const doub& d) {
        return os << d.val;
    }

    std::istream& operator>> (std::istream& is, doub& d) {
        is >> d.val;
        if (fabs(d.val) < EPS) d.val = 0.;
        return is;
    }


    doub powD(doub base, size_t exp) {
        doub res{1};
        while (exp != 0) {
            if (exp%2 == 0) {
                base *= base;
                exp /= 2;
            } else {
                --exp;
                res *= base;
            }
        }
        return res;
    }
}
