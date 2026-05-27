#include "commands.hpp"
#include "geometry.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace
{
struct Command
{
  Command();

  std::string name;
  std::string arg;
};

Command::Command():
  name(),
  arg()
{}

std::istream& operator>>(std::istream& in, Command& command)
{
  Command input;
  in >> input.name;

  if (!in)
  {
    return in;
  }

  std::getline(in, input.arg);

  size_t first_symbol = input.arg.find_first_not_of(' ');

  if (first_symbol == std::string::npos)
  {
    input.arg.clear();
  }
  else
  {
    input.arg.erase(0, first_symbol);
  }

  command = input;
  return in;
}
}

class CommandOutputIterator
{
public:
  using iterator_category = std::output_iterator_tag;
  using value_type = void;
  using difference_type = void;
  using pointer = void;
  using reference = void;

  CommandOutputIterator(std::ostream& out, const matveev::data_t& data):
    out_(out),
    data_(data)
  {}

  CommandOutputIterator& operator=(const Command& command)
  {
    matveev::executeCommand(out_, data_, command.name, command.arg);
    return *this;
  }

  CommandOutputIterator& operator*()
  {
    return *this;
  }

  CommandOutputIterator& operator++()
  {
    return *this;
  }

  CommandOutputIterator operator++(int)
  {
    return *this;
  }

private:
  std::ostream& out_;
  const matveev::data_t& data_;
};

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }

  std::ifstream input(argv[1]);

  if (!input)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }

  matveev::data_t polygons;
  using polygon_input_t = std::istream_iterator< matveev::Polygon >;
  std::copy(polygon_input_t{ input }, polygon_input_t{}, std::back_inserter(polygons));
  std::vector< Command > commands;
  using command_input_t = std::istream_iterator< Command >;
  std::copy(command_input_t{ std::cin }, command_input_t{}, std::back_inserter(commands));
  std::copy(commands.begin(), commands.end(), CommandOutputIterator(std::cout, polygons));

  return 0;
}
