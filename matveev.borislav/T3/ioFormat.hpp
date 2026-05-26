#ifndef MATVEEV_IO_FORMAT_HPP
#define MATVEEV_IO_FORMAT_HPP

#include <iosfwd>

namespace matveev
{
struct DelimiterIO
{
  char expected;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}

#endif
