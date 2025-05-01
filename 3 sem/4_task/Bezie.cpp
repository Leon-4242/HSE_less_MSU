#include "Bezie.h"

namespace Bezie {
Point BezieLine(const std::vector<Point> points, const size_t k1, const size_t k2, const double t) {
        if (k1 == k2) {
            return points[k1];
        }
        return (1-t)*BezieLine(points, k1, k2-1, t)+t*BezieLine(points, k1+1, k2, t);
    }
}
