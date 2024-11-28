#ifndef InterLagr
#define InterLagr

#include "RBTREE.h"
#include "LIST.h"
#include "polynom.h"

namespace APROXIMATOR {
    using namespace RBTREE;
    using namespace LIST;
    using namespace poly;

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
        RBTree< Pair<size_t, size_t> /*KEY*/, doub /*VALUE*/> coefNewton;
        polynom p;
        bool actual;

        public:
        Aproximator(void): points(), coord(), coefNewton(), p(), actual(true) {}
        Aproximator(const Aproximator& app): points(app.points), coord(app.coord), coefNewton(app.coefNewton), p(app.p), actual(app.actual) {}
        Aproximator& operator= (const Aproximator& app) {points = app.points; coord = app.coord; coefNewton = app.coefNewton; p = app.p; actual = app.actual; return *this;}
        ~Aproximator(void) {}

        doub& operator[] (const doub& x) {
            doub y = 0.0; actual = false;
            if (points.find(x, y)) return points[x];
            points.insert(x, y);
            coord.pushBack(x);
            return points[x];
        }

        void mkCoef(void) {
            for (size_t k = 0; k < points.Num(); ++k) {
                for (auto iter = coord.begin(), it = iter + k; it.Index() < points.Num(); ++iter, ++it) {
                    doub buff = 0;
                    if (k == 0) buff = points[*iter];
                    else buff = (coefNewton[Pair(k-1, iter.Index()+1)]-coefNewton[Pair(k-1, iter.Index())])/(*(it)-(*iter));
                    coefNewton.insert(Pair(k, iter.Index()) , buff);
                }
            }
        }

        void mkPoly(void) {
            polynom tmp(1);
            p = polynom();
            for (auto iter = coord.begin(); iter != coord.end(); ++iter) {
                p += tmp * (coefNewton[Pair(iter.Index(), (size_t)0)]);
                tmp *= polynom(1, 1)-(*iter);
            }
        }

        void operator() (const doub& x, const doub &y) {
            doub tmp = y;
            if (!points.find(x, tmp)) coord.pushBack(x);
            points.insert(x, y);
            mkCoef();
            mkPoly();
            actual = true;
        }

        double operator() (const doub& x) {
            if (!actual) {
                mkCoef(); mkPoly();
            }
            return (p(x))();
        }

        friend std::ostream& operator<< (std::ostream& os, Aproximator& app) {
            if (!app.actual) {
                app.mkCoef();
                app.mkPoly();
            }
            return os << app.p;
        }
    };
}

#endif

