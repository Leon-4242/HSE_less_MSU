#include "APROXIMATOR.h"

namespace APROXIMATOR {
        doub& Aproximator::operator[] (const doub& x) {
            doub y = 0.0; actual = false;
            if (points.find(x, y)) return points[x];
            points.insert(x, y);
            coord.pushBack(x);
            return points[x];
        }

        void Aproximator::remove(const doub& d) {
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
            mkCoef(); mkPolyNewton(); mkPolyLagrange();
            actual = true;
        }

        void Aproximator::mkCoef(void) {
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

        void Aproximator::mkPolyNewton(void) {
            polynom tmp(1);
            pNewton = polynom();
            for (auto iter = coord.begin(); iter != coord.end(); ++iter) {
                pNewton += tmp * (coefNewton[Pair(iter.Index(), (size_t)0)]);
                tmp *= polynom(1, 1)-(*iter);
            }
        }

        void Aproximator::mkPolyLagrange(void) {
            pLagrange = polynom();
            polynom buff{1.0};
            for (auto iter = coord.begin(); iter != coord.end(); ++iter) {
                buff = polynom(1.0);
                for (auto it = coord.begin(); it != coord.end(); ++it) {
                    if (it != iter) {
                        buff *= (polynom(1, 1) - (*it)) * (doub(1.0)/(*iter - *it));
                    }
                }
                pLagrange += points[*iter]*buff;
            }
        }

        void Aproximator::operator() (const doub& x, const doub &y) {
            doub tmp = y;
            if (!points.find(x, tmp)) coord.pushBack(x);
            points.insert(x, y);
            mkCoef();
            mkPolyNewton();
            mkPolyLagrange();
            actual = true;
        }

        double Aproximator::operator() (const doub& x) {
            if (!actual) {
                mkCoef(); mkPolyNewton(); mkPolyLagrange();
            }
            return (pNewton(x))();
        }

        std::ostream& operator<< (std::ostream& os, Aproximator& app) {
            if (!app.actual) {
                app.mkCoef();
                app.mkPolyNewton();
                app.mkPolyLagrange();
            }
            return os << "polynom by Newton's method: " << app.pNewton <<"\npolynom by Lagrange's method: " << app.pLagrange ;
        }
}
