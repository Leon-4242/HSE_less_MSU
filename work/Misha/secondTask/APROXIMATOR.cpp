#include "APROXIMTOR.h"


namespace APROXIMATOR {
        doub& APROXIMATOR::operator[] (const doub& x) {
            doub y = 0.0; actual = false;
            if (points.find(x, y)) return points[x];
            points.insert(x, y);
            coord.pushBack(x);
            return points[x];
        }

        void APROXIMATOR::remove(const doub& d) {
            size_t i = points.Num()+1;
            while (i != 0) {
                coefNewton.remove(Pair((size_t)(i-1), points.Num()-i));
                --i;
            }
            for (auto iter = coord.begin(); iter != coord.end(); ++iter) {
                if (*iter == d) {
                    coord.popIn(iter);
                    break;
                }
            }
            points.remove(d);
            mkCoef(); mkPoly();
            actual = true;
        }

        void APROXIMATOR::mkCoef(void) {
            for (size_t k = 0; k < points.Num(); ++k) {
                for (auto iter = coord.begin(), it = iter + k; it.Index() < points.Num(); ++iter, ++it) {
                    doub buff = 0;
                    if (k == 0) {
                        buff = points[*iter];
                    } else buff = (coefNewton[Pair(k-1, iter.Index()+1)]-coefNewton[Pair(k-1, iter.Index())])/(*(it)-(*iter));
                    coefNewton.insert(Pair(k, iter.Index()) , buff);
                }
            }
        }

        void APROXIMATOR::mkPoly(void) {
            polynom tmp(1);
            p = polynom();
            for (auto iter = coord.begin(); iter != coord.end(); ++iter) {
                p += tmp * (coefNewton[Pair(iter.Index(), (size_t)0)]);
                tmp *= polynom(1, 1)-(*iter);
            }
        }

        void APROXIMATOR::operator() (const doub& x, const doub &y) {
            doub tmp = y;
            if (!points.find(x, tmp)) coord.pushBack(x);
            points.insert(x, y);
            mkCoef();
            mkPoly();
            actual = true;
        }

        double APROXIMATOR::operator() (const doub& x) {
            if (!actual) {
                mkCoef(); mkPoly();
            }
            return (p(x))();
        }
}
        std::ostream& operator<< (std::ostream& os, APROXIMATOR::Aproximator& app) {
            if (!app.actual) {
                app.mkCoef();
                app.mkPoly();
            }
            return os << "polynom by Newton: " << app.p;
        }
