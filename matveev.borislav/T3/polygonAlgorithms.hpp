#ifndef MATVEEV_POLYGON_ALGORITHMS_HPP
#define MATVEEV_POLYGON_ALGORITHMS_HPP

#include "geometry.hpp"

#include <vector>

namespace matveev
{
struct Frame
{
  Point left_bottom;
  Point right_top;
};

double getTriangleArea(const Point& first, const Point& second, const Point& third);
double getPolygonArea(const Polygon& polygon);

Frame getFrame(const Polygon& polygon);
Frame getFrame(const std::vector< Polygon >& polygons);
bool isPointInFrame(const Frame& frame, const Point& point);
bool isPolygonInFrame(const Frame& frame, const Polygon& polygon);
}

#endif
