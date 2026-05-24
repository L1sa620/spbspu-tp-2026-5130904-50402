#ifndef MATVEEV_DATA_STRUCT_HPP
#define MATVEEV_DATA_STRUCT_HPP

#include <iosfwd>
#include <string>

namespace matveev
{
struct DataStruct
{
  DataStruct();
  DataStruct(unsigned long long key1_value, char key2_value, const std::string& key3_value);

  unsigned long long key1;
  char key2;
  std::string key3;
};

struct Line
{
  Line();

  std::string value;
};

std::istream& operator>>(std::istream& in, Line& line);

bool compareDataStruct(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
