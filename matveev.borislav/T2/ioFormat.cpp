#include "ioFormat.hpp"

#include <istream>
#include <ostream>

namespace
{
bool isUnsignedSuffix(char c)
{
  return c == 'u' || c == 'U';
}

bool isLongSuffix(char c)
{
  return c == 'l' || c == 'L';
}
}

matveev::IOGuard::IOGuard(std::basic_ios< char >& stream):
  stream_(stream),
  flags_(stream.flags()),
  fill_(stream.fill()),
  precision_(stream.precision())
{}

matveev::IOGuard::~IOGuard()
{
  stream_.flags(flags_);
  stream_.fill(fill_);
  stream_.precision(precision_);
}

std::istream& matveev::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  char c = 0;
  in >> c;

  if (in && c != dest.expected)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& matveev::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  for (const char* i = dest.expected; *i != '\0'; ++i)
  {
    char c = 0;
    in >> c;

    if (!in || c != *i)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  return in;
}

std::istream& matveev::operator>>(std::istream& in, UllLitIO&& dest)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  in >> std::ws;

  if (in.peek() == '-')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  unsigned long long value = 0;
  in >> value;

  if (!in)
  {
    return in;
  }

  IOGuard guard(in);
  in >> std::noskipws;

  char first = 0;
  char second = 0;
  char third = 0;

  in >> first >> second >> third;

  if (!in || !isUnsignedSuffix(first) || !isLongSuffix(second) || !isLongSuffix(third))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  dest.ref = value;
  return in;
}

std::istream& matveev::operator>>(std::istream& in, ChrLitIO&& dest)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  IOGuard guard(in);

  in >> DelimiterIO{ '\'' };

  if (!in)
  {
    return in;
  }

  char value = 0;
  in >> std::noskipws >> value;

  if (!in)
  {
    return in;
  }

  in >> DelimiterIO{ '\'' };

  if (in)
  {
    dest.ref = value;
  }

  return in;
}

std::istream& matveev::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  in >> DelimiterIO{ '"' };

  if (!in)
  {
    return in;
  }

  std::string value;
  std::getline(in, value, '"');

  if (in)
  {
    dest.ref = value;
  }

  return in;
}

std::ostream& matveev::operator<<(std::ostream& out, const UllLitO& dest)
{
  out << dest.ref << "ull";
  return out;
}

std::ostream& matveev::operator<<(std::ostream& out, const ChrLitO& dest)
{
  out << '\'' << dest.ref << '\'';
  return out;
}

std::ostream& matveev::operator<<(std::ostream& out, const StringO& dest)
{
  out << '"' << dest.ref << '"';
  return out;
}
