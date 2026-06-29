#include "commands.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <istream>
#include <iterator>
#include <map>
#include <numeric>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <ioGuard.hpp>
#include "polygonAlgorithms.hpp"

namespace
{
  bool isEvenVertexes(const matveev::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOddVertexes(const matveev::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 1;
  }

  bool hasVertexCount(std::size_t count, const matveev::Polygon& polygon)
  {
    return polygon.points.size() == count;
  }

  bool isLessArea(const matveev::Polygon& lhs, const matveev::Polygon& rhs)
  {
    return matveev::getPolygonArea(lhs) < matveev::getPolygonArea(rhs);
  }

  bool isLessVertexes(const matveev::Polygon& lhs, const matveev::Polygon& rhs)
  {
    return lhs.points.size() < rhs.points.size();
  }

  bool isIntersectWith(const matveev::Polygon& target, const matveev::Polygon& polygon)
  {
    return matveev::isPolygonIntersect(target, polygon);
  }

  double getArea(const matveev::Polygon& polygon)
  {
    return matveev::getPolygonArea(polygon);
  }

  double getAreaSum(const matveev::data_t& data)
  {
    std::vector< double > areas;
    areas.reserve(data.size());

    std::transform(data.begin(), data.end(), std::back_inserter(areas), getArea);

    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  std::size_t readSize(const std::string& text)
  {
    std::size_t pos = 0;
    std::size_t value = std::stoull(text, &pos);

    if (pos != text.size())
    {
      throw std::logic_error("invalid number");
    }

    return value;
  }

  void printArea(std::ostream& out, double value)
  {
    matveev::IOGuard guard(out);
    out << std::fixed << std::setprecision(1) << value;
  }

  double areaEven(const matveev::data_t& data)
  {
    matveev::data_t selected;
    std::copy_if(data.begin(), data.end(), std::back_inserter(selected), isEvenVertexes);
    return getAreaSum(selected);
  }

  double areaOdd(const matveev::data_t& data)
  {
    matveev::data_t selected;
    std::copy_if(data.begin(), data.end(), std::back_inserter(selected), isOddVertexes);
    return getAreaSum(selected);
  }

  double areaMean(const matveev::data_t& data)
  {
    if (data.empty())
    {
      throw std::logic_error("empty data");
    }

    return getAreaSum(data) / data.size();
  }

  void printMaxArea(std::ostream& out, const matveev::data_t& data)
  {
    printArea(out, matveev::getPolygonArea(*std::max_element(data.begin(), data.end(), isLessArea)));
  }

  void printMaxVertexes(std::ostream& out, const matveev::data_t& data)
  {
    out << std::max_element(data.begin(), data.end(), isLessVertexes)->points.size();
  }

  void printMinArea(std::ostream& out, const matveev::data_t& data)
  {
    printArea(out, matveev::getPolygonArea(*std::min_element(data.begin(), data.end(), isLessArea)));
  }

  void printMinVertexes(std::ostream& out, const matveev::data_t& data)
  {
    out << std::min_element(data.begin(), data.end(), isLessVertexes)->points.size();
  }
}

void matveev::doArea(std::istream& in, std::ostream& out, const data_t& data)
{
  std::string arg;
  in >> arg;

  if (!in)
  {
    throw std::logic_error("missing argument");
  }

  std::map< std::string, std::function< double(const data_t&) > > subcommands;
  subcommands["EVEN"] = areaEven;
  subcommands["ODD"] = areaOdd;
  subcommands["MEAN"] = areaMean;

  if (subcommands.find(arg) != subcommands.end())
  {
    printArea(out, subcommands.at(arg)(data));
    return;
  }

  std::size_t count = readSize(arg);

  if (count < 3)
  {
    throw std::logic_error("invalid vertex count");
  }

  data_t selected;
  using namespace std::placeholders;
  std::copy_if(data.begin(), data.end(), std::back_inserter(selected), std::bind(hasVertexCount, count, _1));
  printArea(out, getAreaSum(selected));
}

void matveev::doCount(std::istream& in, std::ostream& out, const data_t& data)
{
  std::string arg;
  in >> arg;

  if (!in)
  {
    throw std::logic_error("missing argument");
  }

  std::map< std::string, std::function< bool(const Polygon&) > > predicates;
  predicates["EVEN"] = isEvenVertexes;
  predicates["ODD"] = isOddVertexes;

  if (predicates.find(arg) != predicates.end())
  {
    out << std::count_if(data.begin(), data.end(), predicates.at(arg));
    return;
  }

  std::size_t count = readSize(arg);

  if (count < 3)
  {
    throw std::logic_error("invalid vertex count");
  }

  using namespace std::placeholders;
  out << std::count_if(data.begin(), data.end(), std::bind(hasVertexCount, count, _1));
}

void matveev::doMax(std::istream& in, std::ostream& out, const data_t& data)
{
  if (data.empty())
  {
    throw std::logic_error("empty data");
  }

  std::string arg;
  in >> arg;

  if (!in)
  {
    throw std::logic_error("missing argument");
  }

  std::map< std::string, std::function< void(std::ostream&, const data_t&) > > subcommands;
  subcommands["AREA"] = printMaxArea;
  subcommands["VERTEXES"] = printMaxVertexes;

  if (subcommands.find(arg) == subcommands.end())
  {
    throw std::logic_error("invalid max command");
  }

  subcommands.at(arg)(out, data);
}

void matveev::doMin(std::istream& in, std::ostream& out, const data_t& data)
{
  if (data.empty())
  {
    throw std::logic_error("empty data");
  }

  std::string arg;
  in >> arg;

  if (!in)
  {
    throw std::logic_error("missing argument");
  }

  std::map< std::string, std::function< void(std::ostream&, const data_t&) > > subcommands;
  subcommands["AREA"] = printMinArea;
  subcommands["VERTEXES"] = printMinVertexes;

  if (subcommands.find(arg) == subcommands.end())
  {
    throw std::logic_error("invalid min command");
  }

  subcommands.at(arg)(out, data);
}

void matveev::doInFrame(std::istream& in, std::ostream& out, const data_t& data)
{
  if (data.empty())
  {
    throw std::logic_error("empty data");
  }

  Polygon polygon;
  in >> polygon;

  if (!in)
  {
    throw std::logic_error("invalid polygon");
  }

  Frame frame = getFrame(data);

  if (isPolygonInFrame(frame, polygon))
  {
    out << "<TRUE>";
  }
  else
  {
    out << "<FALSE>";
  }
}

void matveev::doIntersections(std::istream& in, std::ostream& out, const data_t& data)
{
  Polygon polygon;
  in >> polygon;

  if (!in)
  {
    throw std::logic_error("invalid polygon");
  }

  using namespace std::placeholders;
  out << std::count_if(data.begin(), data.end(), std::bind(isIntersectWith, polygon, _1));
}
