#include "commands.hpp"
#include "polygonAlgorithms.hpp"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <ostream>
#include <stdexcept>

namespace
{
class IOGuard
{
public:
  explicit IOGuard(std::basic_ios< char >& stream):
    stream_(stream),
    precision_(stream.precision()),
    flags_(stream.flags())
  {}

  ~IOGuard()
  {
    stream_.precision(precision_);
    stream_.flags(flags_);
  }

private:
  std::basic_ios< char >& stream_;
  std::streamsize precision_;
  std::basic_ios< char >::fmtflags flags_;
};

bool isEvenVertexes(const matveev::Polygon& polygon)
{
  return polygon.points.size() % 2 == 0;
}

bool isOddVertexes(const matveev::Polygon& polygon)
{
  return polygon.points.size() % 2 == 1;
}

bool hasVertexCount(size_t count, const matveev::Polygon& polygon)
{
  return polygon.points.size() == count;
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

size_t readSize(const std::string& text)
{
  size_t pos = 0;
  size_t value = std::stoull(text, &pos);

  if (pos != text.size())
  {
    throw std::logic_error("invalid number");
  }

  return value;
}

void printArea(std::ostream& out, double value)
{
  IOGuard guard(out);
  out << std::fixed << std::setprecision(1) << value << '\n';
}
}

void matveev::doArea(std::ostream& out, const data_t& data, const std::string& arg)
{
  data_t selected;

  if (arg == "EVEN")
  {
    std::copy_if(data.begin(), data.end(), std::back_inserter(selected), isEvenVertexes);
    printArea(out, getAreaSum(selected));
    return;
  }

  if (arg == "ODD")
  {
    std::copy_if(data.begin(), data.end(), std::back_inserter(selected), isOddVertexes);
    printArea(out, getAreaSum(selected));
    return;
  }

  if (arg == "MEAN")
  {
    if (data.empty())
    {
      throw std::logic_error("empty data");
    }

    printArea(out, getAreaSum(data) / data.size());
    return;
  }

  size_t count = readSize(arg);

  if (count < 3)
  {
    throw std::logic_error("invalid vertex count");
  }

  using namespace std::placeholders;
  std::copy_if(data.begin(), data.end(), std::back_inserter(selected), std::bind(hasVertexCount, count, _1));
  printArea(out, getAreaSum(selected));
}

void matveev::doCount(std::ostream& out, const data_t& data, const std::string& arg)
{
  if (arg == "EVEN")
  {
    out << std::count_if(data.begin(), data.end(), isEvenVertexes) << '\n';
    return;
  }

  if (arg == "ODD")
  {
    out << std::count_if(data.begin(), data.end(), isOddVertexes) << '\n';
    return;
  }

  size_t count = readSize(arg);

  if (count < 3)
  {
    throw std::logic_error("invalid vertex count");
  }

  using namespace std::placeholders;
  out << std::count_if(data.begin(), data.end(), std::bind(hasVertexCount, count, _1)) << '\n';
}

void matveev::executeCommand(
  std::ostream& out,
  const data_t& data,
  const std::string& command,
  const std::string& arg
)
{
  try
  {
    if (command == "AREA")
    {
      doArea(out, data, arg);
      return;
    }

    if (command == "COUNT")
    {
      doCount(out, data, arg);
      return;
    }

    out << "<INVALID COMMAND>\n";
  }
  catch (const std::exception&)
  {
    out << "<INVALID COMMAND>\n";
  }
}
