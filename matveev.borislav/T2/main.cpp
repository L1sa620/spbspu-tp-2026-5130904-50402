#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace matveev
{
struct DataStruct
{
  unsigned long long key1;
  char key2;
  std::string key3;
};

std::istream& operator>>(std::istream& in, DataStruct& data)
{
  unsigned long long key1 = 0;
  char key2 = 0;
  std::string key3;

  in >> key1 >> key2 >> key3;

  if (in)
  {
    data = DataStruct{ key1, key2, key3 };
  }

  return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data)
{
  out << "(:";
  out << "key1 " << data.key1 << "ull";
  out << ":key2 '" << data.key2 << "'";
  out << ":key3 \"" << data.key3 << "\"";
  out << ":)";

  return out;
}

bool operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }

  if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }

  return lhs.key3.size() < rhs.key3.size();
}
}

int main()
{
  std::vector< matveev::DataStruct > data;

  using input_iterator_t = std::istream_iterator< matveev::DataStruct >;
  std::copy(input_iterator_t{ std::cin }, input_iterator_t{}, std::back_inserter(data));

  std::sort(data.begin(), data.end());

  using output_iterator_t = std::ostream_iterator< matveev::DataStruct >;
  std::copy(data.begin(), data.end(), output_iterator_t{ std::cout, "\n" });
}
