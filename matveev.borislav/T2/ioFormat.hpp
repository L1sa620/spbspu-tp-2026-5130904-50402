#ifndef MATVEEV_IO_FORMAT_HPP
#define MATVEEV_IO_FORMAT_HPP

#include <ios>
#include <string>

namespace matveev
{
struct DelimiterIO
{
  char expected;
};

struct LabelIO
{
  const char* expected;
};

struct UllLitIO
{
  unsigned long long& ref;
};

struct ChrLitIO
{
  char& ref;
};

struct StringIO
{
  std::string& ref;
};

struct UllLitO
{
  const unsigned long long& ref;
};

struct ChrLitO
{
  const char& ref;
};

struct StringO
{
  const std::string& ref;
};

class IOGuard
{
public:
  explicit IOGuard(std::basic_ios< char >& stream);
  ~IOGuard();

private:
  std::basic_ios< char >& stream_;
  std::basic_ios< char >::fmtflags flags_;
  char fill_;
  std::streamsize precision_;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, LabelIO&& dest);
std::istream& operator>>(std::istream& in, UllLitIO&& dest);
std::istream& operator>>(std::istream& in, ChrLitIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);

std::ostream& operator<<(std::ostream& out, const UllLitO& dest);
std::ostream& operator<<(std::ostream& out, const ChrLitO& dest);
std::ostream& operator<<(std::ostream& out, const StringO& dest);
}

#endif
