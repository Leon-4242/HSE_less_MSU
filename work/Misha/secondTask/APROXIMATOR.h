#ifndef InterLagr
#define InterLagr

#include "RBTREE.h"
#include "LIST.h"
#include <cmath>
#define EPS 1e-12

namespace APROXIMATOR {
    using namespace RBTREE;
    using namespace LIST;

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
    };

    template <typename T, typename L>
    class Pair {
        T val1; L val2;
        public:
        Pair(T v1 = T(), L v2 = L()): val1(v1), val2(v2) {}
        Pair(const Pair& p): val1(p.val1), val2(p.val2) {}
        Pair& operator= (const Pair& p) {val1 = p.val1; val2 = p.val2; return *this;}
        ~Pair(void) {}
        const T& first(void) const {return val1;}
        T& first(void) {return val1;}
        const L& second(void) const {return val2;}
        L& second(void) {return val2;}
        bool operator< (const Pair& p) const {return (val1 < p.val1) || (val1 == p.val1 && val2 < p.val2);}
        bool operator== (const Pair& p) const {return val1 == p.val1 && val2 == p.val2;}
        bool operator!= (const Pair& p) const {return !(*this == p);}
        bool operator<= (const Pair& p) const {return *this < p || *this == p;}
        bool operator> (const Pair& p) const {return !(*this <= p);}
        bool operator>= (const Pair& p) const {return !(*this < p);}
    };

    class Aproximator {
        RBTree<doub, doub> points;
        List<doub> coord;
        RBTree<Pair<size_t, size_t>, doub> coefNewton;
        
        public:
        Aproximator(void): points(), coord(), coefNewton() {}
        Aproximator(const Aproximator& polynom): points(polynom.points), coord(polynom.coord), coefNewton(polynom.coefNewton) {}
        Aproximator& operator= (const Aproximator& polynom) {points = polynom.points; coord = polynom.coord; coefNewton = polynom.coefNewton; return *this;}
        ~Aproximator(void) {}

        doub& operator[] (const doub& x) {
            doub y = 0.0;
            if (points.find(x, y)) return points[x];
            points.insert(x, y);
            coord.pushBack(x);
            return points[x];
        }

        void mkCoef(void) {
            for (size_t k = 0; k < points.Num()+1; ++k) {
                for (auto iter = coord.begin(), it = coord.begin() + k; it.Index() < points.Num()+1; ++iter, ++it) {
                    doub buff = 0;
                    if (k == 0) buff = points[*iter];
                    else buff = (coefNewton[Pair(k-1, iter.Index()+1)]-coefNewton[Pair(k-1, iter.Index())])/(*(it)-(*iter));
                    coefNewton.insert(Pair(k, iter.Index()) , buff);
                }
            }
        }
        friend std::ostream& operator<< (std::ostream& os, const Aproximator& polynom) {
            
        }
    };
}

#endif

