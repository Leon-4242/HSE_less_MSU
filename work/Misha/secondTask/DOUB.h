#ifndef DoubleNew
#define DoubleNew

#define EPS 1e-12
#include <cmath>
#include <iostream>

namespace DOUB {

    class doub {
        double val;
        public:
        doub(const double& v = 0.0) {
            if (fabs(v) < EPS) val = 0.0;
            else val = v;
        }
        doub(const doub& d): val(d.val){}
        ~doub(void) {}
        doub& operator=(const doub& d) {val = d.val; return *this;}

        doub operator+ (const doub& d) const {return doub(val+d.val);}
        doub operator- (const doub& d) const {return doub(val-d.val);}
        doub operator+ (void) const {return doub(val);}
        doub operator- (void) const {return doub(-val);}

        doub operator* (const doub& d) const {return doub(val*d.val);}
        doub operator/ (const doub& d) const {return doub(val/d.val);}

        doub& operator+= (const doub& d) {*this = *this + d; return *this;}
        doub& operator-= (const doub& d) {*this = *this - d; return *this;}
        doub& operator*= (const doub& d) {*this = *this * d; return *this;}
        doub& operator/= (const doub& d) {*this = *this / d; return *this;}

        bool operator== (const doub& d) const {return fabs(val - d.val) < EPS;}
        bool operator!= (const doub& d) const {return !(*this == d);}
        bool operator< (const doub& d) const {return *this != d && val < d.val;}
        bool operator> (const doub& d) const {return *this != d && val > d.val;}
        bool operator<= (const doub& d) const {return !(*this > d);}
        bool operator>= (const doub& d) const {return !(*this < d);}
        std::string print (void) const {return std::to_string(val);}
        friend doub powD(doub base, size_t exp);
        friend std::ostream& operator<< (std::ostream& os, const doub& d);
        friend std::istream& operator>> (std::istream& is, doub& d);
        double operator()(void) const {return val;}
    };
}

#endif
