#include <iostream>
#include <string>
#include <limits>
#include "commands.hpp"

int main()
{
  matveev::db_t db;
  auto cmds = matveev::initCommands();

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, db);
    }
    catch (const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
    catch (const std::logic_error&)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
