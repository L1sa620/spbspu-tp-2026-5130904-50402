#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <vector>

#include "geometry.hpp"

namespace matveev
{
  using data_t = std::vector< Polygon >;

  void doArea(std::istream& in, std::ostream& out, const data_t& data);
  void doCount(std::istream& in, std::ostream& out, const data_t& data);
  void doMax(std::istream& in, std::ostream& out, const data_t& data);
  void doMin(std::istream& in, std::ostream& out, const data_t& data);
  void doInFrame(std::istream& in, std::ostream& out, const data_t& data);
  void doIntersections(std::istream& in, std::ostream& out, const data_t& data);
}

#endif
