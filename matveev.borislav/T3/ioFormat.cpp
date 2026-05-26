#include "ioFormat.hpp"

#include <istream>

std::istream& matveev::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  char symbol = 0;
  in >> symbol;

  if (in && symbol != dest.expected)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}
