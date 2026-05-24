#include "dataStruct.hpp"
#include "ioFormat.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

namespace matveev
{
enum class KeyType
{
  invalid,
  key1,
  key2,
  key3
};

struct KeyIO
{
  KeyType& ref;
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

bool allFieldsRead(const FieldsState& state)
{
  return state.key1 && state.key2 && state.key3;
}

struct FieldIO
{
  DataStruct& data;
  FieldsState& state;
};

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

  if (in && allFieldsRead(state))
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

  return 0;
}
