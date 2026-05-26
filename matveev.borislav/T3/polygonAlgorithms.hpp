#ifndef MATVEEV_POLYGON_ALGORITHMS_HPP
#define MATVEEV_POLYGON_ALGORITHMS_HPP

#include "geometry.hpp"

namespace matveev
{
double getTriangleArea(const Point& first, const Point& second, const Point& third);
double getPolygonArea(const Polygon& polygon);
}

#endif
