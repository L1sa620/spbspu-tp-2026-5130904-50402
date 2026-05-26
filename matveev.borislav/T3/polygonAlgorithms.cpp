#include "polygonAlgorithms.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <vector>

namespace
{
class TriangleAreaGenerator
{
public:
  TriangleAreaGenerator(const matveev::Point& base, std::vector< matveev::Point >::const_iterator current):
    base_(base),
    current_(current)
  {}

  double operator()()
  {
    std::vector< matveev::Point >::const_iterator next = current_;
    ++next;

    double area = matveev::getTriangleArea(base_, *current_, *next);

    ++current_;

    return area;
  }

private:
  const matveev::Point& base_;
  std::vector< matveev::Point >::const_iterator current_;
};
}

double matveev::getTriangleArea(const Point& first, const Point& second, const Point& third)
{
  long long first_x = second.x - first.x;
  long long first_y = second.y - first.y;
  long long second_x = third.x - first.x;
  long long second_y = third.y - first.y;

  long long double_area = first_x * second_y - first_y * second_x;

  return std::abs(static_cast< double >(double_area)) / 2.0;
}

double matveev::getPolygonArea(const Polygon& polygon)
{
  if (polygon.points.size() < 3)
  {
    return 0.0;
  }

  std::vector< double > areas;
  areas.reserve(polygon.points.size() - 2);

  std::generate_n(
    std::back_inserter(areas),
    polygon.points.size() - 2,
    TriangleAreaGenerator(polygon.points.front(), ++polygon.points.begin())
  );

  return std::accumulate(areas.begin(), areas.end(), 0.0);
}
