#include "geometry.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }

  std::ifstream input(argv[1]);

  if (!input)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }

  std::vector< matveev::Polygon > polygons;

  using input_t = std::istream_iterator< matveev::Polygon >;
  std::copy(input_t{ input }, input_t{}, std::back_inserter(polygons));

  return 0;
}
