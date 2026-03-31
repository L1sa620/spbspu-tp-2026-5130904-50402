#ifndef MATVEEV_COMMANDS_HPP
#define MATVEEV_COMMANDS_HPP
 
#include <iostream>
#include "note.hpp"
 
namespace matveev
{
  using cmd_t = void(*)(std::istream&, std::ostream&, db_t&);
 
  void create_note(std::istream&, std::ostream&, db_t&);
  void add_line(std::istream&, std::ostream&, db_t&);
  void show_note(std::istream&, std::ostream&, db_t&);
  void drop_note(std::istream&, std::ostream&, db_t&);
 
  void link_note(std::istream&, std::ostream&, db_t&);
  void mind_note(std::istream&, std::ostream&, db_t&);
  void halt_note(std::istream&, std::ostream&, db_t&);
 
  void expired_note(std::istream&, std::ostream&, db_t&);
}
 
#endif
