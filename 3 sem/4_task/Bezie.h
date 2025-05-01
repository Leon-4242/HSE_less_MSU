#ifndef BEZIE
#define BEZIE

#include "R2Geometry.h"
#include <vector>

namespace Bezie {
    using namespace R2Geometry;

Point BezieLine(const std::vector<Point>, const size_t, const size_t, const double);
}

#endif
