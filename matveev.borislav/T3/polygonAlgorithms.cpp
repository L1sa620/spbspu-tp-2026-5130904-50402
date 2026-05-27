#include "polygonAlgorithms.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <vector>
#include <functional>
#include <stdexcept>

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

bool isLessX(const matveev::Point& lhs, const matveev::Point& rhs)
{
  return lhs.x < rhs.x;
}

bool isLessY(const matveev::Point& lhs, const matveev::Point& rhs)
{
  return lhs.y < rhs.y;
}

bool isLessFrameLeftX(const matveev::Frame& lhs, const matveev::Frame& rhs)
{
  return lhs.left_bottom.x < rhs.left_bottom.x;
}

bool isLessFrameBottomY(const matveev::Frame& lhs, const matveev::Frame& rhs)
{
  return lhs.left_bottom.y < rhs.left_bottom.y;
}

bool isLessFrameRightX(const matveev::Frame& lhs, const matveev::Frame& rhs)
{
  return lhs.right_top.x < rhs.right_top.x;
}

bool isLessFrameTopY(const matveev::Frame& lhs, const matveev::Frame& rhs)
{
  return lhs.right_top.y < rhs.right_top.y;
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

matveev::Frame matveev::getFrame(const Polygon& polygon)
{
  if (polygon.points.empty())
  {
    throw std::logic_error("empty polygon");
  }

  std::vector< Point >::const_iterator min_x = std::min_element(polygon.points.begin(), polygon.points.end(), isLessX);
  std::vector< Point >::const_iterator max_x = std::max_element(polygon.points.begin(), polygon.points.end(), isLessX);
  std::vector< Point >::const_iterator min_y = std::min_element(polygon.points.begin(), polygon.points.end(), isLessY);
  std::vector< Point >::const_iterator max_y = std::max_element(polygon.points.begin(), polygon.points.end(), isLessY);

  Frame frame = { Point(min_x->x, min_y->y), Point(max_x->x, max_y->y) };
  return frame;
}

matveev::Frame matveev::getFrame(const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("empty polygons");
  }

  std::vector< Frame > frames;
  frames.reserve(polygons.size());

  std::transform(
    polygons.begin(),
    polygons.end(),
    std::back_inserter(frames),
    static_cast< Frame(*)(const Polygon&) >(getFrame)
  );

  std::vector< Frame >::const_iterator min_x = std::min_element(frames.begin(), frames.end(), isLessFrameLeftX);
  std::vector< Frame >::const_iterator min_y = std::min_element(frames.begin(), frames.end(), isLessFrameBottomY);
  std::vector< Frame >::const_iterator max_x = std::max_element(frames.begin(), frames.end(), isLessFrameRightX);
  std::vector< Frame >::const_iterator max_y = std::max_element(frames.begin(), frames.end(), isLessFrameTopY);

  Frame frame = {
    Point(min_x->left_bottom.x, min_y->left_bottom.y),
    Point(max_x->right_top.x, max_y->right_top.y)
  };

  return frame;
}

bool matveev::isPointInFrame(const Frame& frame, const Point& point)
{
  return point.x >= frame.left_bottom.x
    && point.x <= frame.right_top.x
    && point.y >= frame.left_bottom.y
    && point.y <= frame.right_top.y;
}

bool matveev::isPolygonInFrame(const Frame& frame, const Polygon& polygon)
{
  using namespace std::placeholders;
  return std::all_of(polygon.points.begin(), polygon.points.end(), std::bind(isPointInFrame, frame, _1));
}
