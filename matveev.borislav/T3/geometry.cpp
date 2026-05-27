#include "geometry.hpp"
#include "ioFormat.hpp"

#include <algorithm>
#include <istream>
#include <iterator>
#include <stdexcept>
#include <sstream>

namespace
{
struct PointReader
{
  std::istream& in;

  matveev::Point operator()() const
  {
    matveev::Point point;
    in >> point;

    if (!in)
    {
      throw std::logic_error("invalid point");
    }

    return point;
  }
};
}

matveev::Point::Point():
  x(0),
  y(0)
{}

matveev::Point::Point(int x_value, int y_value):
  x(x_value),
  y(y_value)
{}

matveev::Polygon::Polygon():
  points()
{}

std::istream& matveev::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  int x = 0;
  int y = 0;

  in >> DelimiterIO{ '(' };
  in >> x;
  in >> DelimiterIO{ ';' };
  in >> y;
  in >> DelimiterIO{ ')' };

  if (in)
  {
    point = Point(x, y);
  }

  return in;
}

std::istream& matveev::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  size_t count = 0;
  in >> count;

  if (!in || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  Polygon input;
  input.points.reserve(count);

  try
  {
    std::generate_n(std::back_inserter(input.points), count, PointReader{ in });
  }
  catch (const std::logic_error&)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (in)
  {
    polygon = input;
  }

  return in;
}

matveev::Line::Line():
  value()
{}

std::istream& matveev::operator>>(std::istream& in, Line& line)
{
  line.value.clear();
  std::getline(in >> std::ws, line.value);
  return in;
}

bool matveev::readPolygonFromLine(const Line& line, Polygon& polygon)
{
  std::istringstream input(line.value);

  input >> polygon;
  input >> std::ws;

  return input && input.eof();
}
