#include "commands.hpp"
#include <stdexcept>
#include <iomanip>
#include <algorithm>

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

void matveev::add_line(std::istream& in, std::ostream&, db_t& db)
{
  std::string name;
  in >> name;
  auto it = db.find(name);
  if (it == db.end())
  {
    throw std::logic_error("note not found");
  }
  std::string text;
  in >> std::quoted(text);
  it->second->lines.push_back(text);
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

void matveev::link_note(std::istream& in, std::ostream&, db_t& db)
{
  std::string from, to;
  in >> from >> to;
  auto it_from = db.find(from);
  auto it_to = db.find(to);
  if (it_from == db.end() || it_to == db.end())
  {
    throw std::logic_error("note not found");
  }
  auto& links = it_from->second->links;
  auto target = it_to->second;
  auto dup = std::find_if(links.begin(), links.end(),
    [&target](const std::pair< std::string, std::weak_ptr< Note > >& p)
    {
      auto locked = p.second.lock();
      return locked && locked == target;
    }
  );
  if (dup != links.end())
  {
    throw std::logic_error("link already exists");
  }
  links.push_back({to, it_to->second});
}

void matveev::mind_note(std::istream& in, std::ostream& out, db_t& db)
{
  std::string name;
  in >> name;
  auto it = db.find(name);
  if (it == db.end())
  {
    throw std::logic_error("note not found");
  }
  for (const auto& link : it->second->links)
  {
    if (auto locked = link.second.lock())
    {
      out << locked->name << "\n";
    }
  }
}

void matveev::halt_note(std::istream& in, std::ostream&, db_t& db)
{
  std::string from, to;
  in >> from >> to;
  auto it = db.find(from);
  if (it == db.end())
  {
    throw std::logic_error("note not found");
  }
  auto& links = it->second->links;
  auto found = std::find_if(links.begin(), links.end(),
    [&to](const std::pair< std::string, std::weak_ptr< Note > >& p)
    {
      return p.first == to;
    }
  );
  if (found == links.end())
  {
    throw std::logic_error("link not found");
  }
  links.erase(found);
}
