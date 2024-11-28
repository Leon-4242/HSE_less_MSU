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

        doub& operator[] (const doub& x);

        bool operator== (const Aproximator& app) const {return points == app.points && coord == app.coord;}
        bool operator!= (const Aproximator& app) const {return *this != app;}

        void remove(const doub& d);

        void mkCoef(void);

        void mkPoly(void);

        void operator() (const doub& x, const doub &y);

        double operator() (const doub& x);

        friend std::ostream& operator<< (std::ostream& os, Aproximator& app) {
            if (!app.actual) {
                app.mkCoef();
                app.mkPoly();
            }
            return os << "polynom by Newton: " << app.p;
        }
    };
}

#endif

