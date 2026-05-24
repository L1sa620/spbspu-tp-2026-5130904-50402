#ifndef MATVEEV_DATA_STRUCT_IO_HPP
#define MATVEEV_DATA_STRUCT_IO_HPP

#include "dataStruct.hpp"

#include <iosfwd>

namespace matveev
{
std::istream& operator>>(std::istream& in, DataStruct& data);
std::ostream& operator<<(std::ostream& out, const DataStruct& data);

bool readDataStructFromLine(const Line& line, DataStruct& data);
}

#endif
