#include "dataStruct.hpp"
#include "dataStructIO.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
  std::vector< matveev::Line > lines;
  std::vector< matveev::DataStruct > data;

  using line_input_t = std::istream_iterator< matveev::Line >;
  std::copy(line_input_t{ std::cin }, line_input_t{}, std::back_inserter(lines));

  std::for_each(
    lines.begin(),
    lines.end(),
    [&data](const matveev::Line& line)
    {
      matveev::DataStruct item;

      if (matveev::readDataStructFromLine(line, item))
      {
        data.push_back(item);
      }
    }
  );

  std::sort(data.begin(), data.end(), matveev::compareDataStruct);

  using output_t = std::ostream_iterator< matveev::DataStruct >;
  std::copy(data.begin(), data.end(), output_t{ std::cout, "\n" });

  return 0;
}
