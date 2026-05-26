#ifndef MATVEEV_GEOMETRY_HPP
#define MATVEEV_GEOMETRY_HPP

#include <iosfwd>
#include <vector>

namespace matveev
{
struct Point
{
  Point();
  Point(int x_value, int y_value);

  int x;
  int y;
};

struct Polygon
{
  Polygon();

  std::vector< Point > points;
};

std::istream& operator>>(std::istream& in, Point& point);
std::istream& operator>>(std::istream& in, Polygon& polygon);
}

#endif
