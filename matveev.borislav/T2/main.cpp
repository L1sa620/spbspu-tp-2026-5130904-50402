#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

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

DataStruct::DataStruct():
  key1(0),
  key2(0),
  key3()
{}

DataStruct::DataStruct(unsigned long long key1_value, char key2_value, const std::string& key3_value):
  key1(key1_value),
  key2(key2_value),
  key3(key3_value)
{}

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

struct KeyIO
{
  int& ref;
};

class IOGuard
{
public:
  explicit IOGuard(std::basic_ios< char >& stream):
    stream_(stream),
    flags_(stream.flags()),
    fill_(stream.fill()),
    precision_(stream.precision())
  {}

  ~IOGuard()
  {
    stream_.flags(flags_);
    stream_.fill(fill_);
    stream_.precision(precision_);
  }

private:
  std::basic_ios< char >& stream_;
  std::basic_ios< char >::fmtflags flags_;
  char fill_;
  std::streamsize precision_;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
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

std::istream& operator>>(std::istream& in, LabelIO&& dest)
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

std::istream& operator>>(std::istream& in, UllLitIO&& dest)
{
  unsigned long long value = 0;

  in >> value >> LabelIO{ "ull" };

  if (in)
  {
    dest.ref = value;
  }

  return in;
}

std::istream& operator>>(std::istream& in, ChrLitIO&& dest)
{
  IOGuard guard(in);

  in >> DelimiterIO{ '\'' };

  char value = 0;
  in >> std::noskipws >> value;

  in >> DelimiterIO{ '\'' };

  if (in)
  {
    dest.ref = value;
  }

  return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
  in >> DelimiterIO{ '"' };

  std::string value;
  std::getline(in, value, '"');

  if (in)
  {
    dest.ref = value;
  }

  return in;
}

std::istream& operator>>(std::istream& in, KeyIO&& dest)
{
  in >> LabelIO{ "key" };

  char number = 0;
  in >> number;

  if (!in || number < '1' || number > '3')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  dest.ref = number - '0';
  return in;
}

std::istream& operator>>(std::istream& in, DataStruct& data)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  DataStruct input;
  bool key1 = false;
  bool key2 = false;
  bool key3 = false;

  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

  for (size_t i = 0; i < 3 && in; ++i)
  {
    int key = 0;
    in >> KeyIO{ key };

    if (key == 1 && !key1)
    {
      in >> UllLitIO{ input.key1 };
      key1 = true;
    }
    else if (key == 2 && !key2)
    {
      in >> ChrLitIO{ input.key2 };
      key2 = true;
    }
    else if (key == 3 && !key3)
    {
      in >> StringIO{ input.key3 };
      key3 = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }

    in >> DelimiterIO{ ':' };
  }

  in >> DelimiterIO{ ')' };

  if (in && key1 && key2 && key3)
  {
    data = input;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data)
{
  IOGuard guard(out);

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

  using input_t = std::istream_iterator< matveev::DataStruct >;
  std::copy(input_t{ std::cin }, input_t{}, std::back_inserter(data));

  std::sort(data.begin(), data.end());

  using output_t = std::ostream_iterator< matveev::DataStruct >;
  std::copy(data.begin(), data.end(), output_t{ std::cout, "\n" });
}
