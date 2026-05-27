#ifndef MATVEEV_GEOMETRY_HPP
#define MATVEEV_GEOMETRY_HPP

#include <iosfwd>
#include <vector>
#include <string>

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

struct Line
{
  Line();

  std::string value;
};

std::istream& operator>>(std::istream& in, Point& point);
std::istream& operator>>(std::istream& in, Polygon& polygon);
std::istream& operator>>(std::istream& in, Line& line);

bool readPolygonFromLine(const Line& line, Polygon& polygon);
}

#endif
