#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <sstream>
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

enum class KeyType
{
  invalid,
  key1,
  key2,
  key3
};

struct FieldsState
{
  FieldsState();

  bool key1;
  bool key2;
  bool key3;
};

FieldsState::FieldsState():
  key1(false),
  key2(false),
  key3(false)
{}

struct FieldIO
{
  DataStruct& data;
  FieldsState& state;
};

struct KeyIO
{
  KeyType& ref;
};

struct Line
{
  Line();

  std::string value;
};

Line::Line():
  value()
{}

std::istream& operator>>(std::istream& in, Line& line)
{
  line.value.clear();
  std::getline(in >> std::ws, line.value);
  return in;
}

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

IOGuard::IOGuard(std::basic_ios< char >& stream):
  stream_(stream),
  flags_(stream.flags()),
  fill_(stream.fill()),
  precision_(stream.precision())
{}

IOGuard::~IOGuard()
{
  stream_.flags(flags_);
  stream_.fill(fill_);
  stream_.precision(precision_);
}

bool isUnsignedSuffix(char c)
{
  return c == 'u' || c == 'U';
}

bool isLongSuffix(char c)
{
  return c == 'l' || c == 'L';
}

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

std::istream& operator>>(std::istream& in, ChrLitIO&& dest)
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

std::istream& operator>>(std::istream& in, StringIO&& dest)
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

std::ostream& operator<<(std::ostream& out, const UllLitO& dest)
{
  out << dest.ref << "ull";
  return out;
}

std::ostream& operator<<(std::ostream& out, const ChrLitO& dest)
{
  out << '\'' << dest.ref << '\'';
  return out;
}

std::ostream& operator<<(std::ostream& out, const StringO& dest)
{
  out << '"' << dest.ref << '"';
  return out;
}

std::istream& operator>>(std::istream& in, KeyIO&& dest)
{
  std::istream::sentry sentry(in);

  if (!sentry)
  {
    return in;
  }

  in >> LabelIO{ "key" };

  char number = 0;
  in >> number;

  if (!in)
  {
    return in;
  }

  if (number == '1')
  {
    dest.ref = KeyType::key1;
  }
  else if (number == '2')
  {
    dest.ref = KeyType::key2;
  }
  else if (number == '3')
  {
    dest.ref = KeyType::key3;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& operator>>(std::istream& in, FieldIO&& dest)
{
  KeyType key = KeyType::invalid;
  in >> KeyIO{ key };

  if (key == KeyType::key1 && !dest.state.key1)
  {
    in >> UllLitIO{ dest.data.key1 };
    dest.state.key1 = true;
  }
  else if (key == KeyType::key2 && !dest.state.key2)
  {
    in >> ChrLitIO{ dest.data.key2 };
    dest.state.key2 = true;
  }
  else if (key == KeyType::key3 && !dest.state.key3)
  {
    in >> StringIO{ dest.data.key3 };
    dest.state.key3 = true;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

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
  FieldsState state;

  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

  std::array< int, 3 > fields{ { 0, 0, 0 } };

  std::for_each(
    fields.begin(),
    fields.end(),
    [&in, &input, &state](int)
    {
      if (in)
      {
        in >> FieldIO{ input, state } >> DelimiterIO{ ':' };
      }
    }
  );

  in >> DelimiterIO{ ')' };

  if (in && state.key1 && state.key2 && state.key3)
  {
    data = input;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

bool readDataStructFromLine(const Line& line, DataStruct& data)
{
  std::istringstream input(line.value);

  input >> data;
  input >> std::ws;

  return input && input.eof();
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data)
{
  IOGuard guard(out);

  out << "(:";
  out << "key1 " << UllLitO{ data.key1 };
  out << ":key2 " << ChrLitO{ data.key2 };
  out << ":key3 " << StringO{ data.key3 };
  out << ":)";

  return out;
}

bool compareDataStruct(const DataStruct& lhs, const DataStruct& rhs)
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
}
