#include "commands.hpp"
#include <stdexcept>
 
void matveev::create_note(std::istream& in, std::ostream&, db_t& db)
{
  std::string name;
  in >> name;
  if (db.count(name))
  {
    throw std::logic_error("note already exists");
  }
  auto note = std::make_shared< Note >();
  note->name = name;
  db[name] = note;
}
 
void matveev::show_note(std::istream& in, std::ostream& out, db_t& db)
{
  std::string name;
  in >> name;
  auto it = db.find(name);
  if (it == db.end())
  {
    throw std::logic_error("note not found");
  }
  for (const auto& line : it->second->lines)
  {
    out << line << "\n";
  }
}
 
void matveev::drop_note(std::istream& in, std::ostream&, db_t& db)
{
  std::string name;
  in >> name;
  if (!db.erase(name))
  {
    throw std::logic_error("note not found");
  }
}
