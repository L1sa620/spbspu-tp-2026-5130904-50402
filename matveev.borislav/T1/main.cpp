#include <iostream>
#include <limits>
#include <string>
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
catch (const std::exception&)
{
std::cout << "<INVALID COMMAND>";
std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
}
std::cout << "\n";
}

return 0;
}
