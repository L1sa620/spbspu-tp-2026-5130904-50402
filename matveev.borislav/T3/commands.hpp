#ifndef MATVEEV_COMMANDS_HPP
#define MATVEEV_COMMANDS_HPP

#include "geometry.hpp"

#include <iosfwd>
#include <string>
#include <vector>

namespace matveev
{
using data_t = std::vector< Polygon >;

void doArea(std::ostream& out, const data_t& data, const std::string& arg);
void doCount(std::ostream& out, const data_t& data, const std::string& arg);
void executeCommand(std::ostream& out, const data_t& data, const std::string& command, const std::string& arg);
}

#endif
