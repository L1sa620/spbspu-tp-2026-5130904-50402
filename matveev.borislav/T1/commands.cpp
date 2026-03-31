#include "commands.hpp"
#include <stdexcept>
#include <iomanip>
#include <algorithm>

namespace
{
  std::string readArg(std::istream& in)
  {
    std::string arg;
    if (!(in >> arg))
    {
      throw std::logic_error("missing argument");
    }
    return arg;
  }

  std::shared_ptr< matveev::Note >& findNote(const std::string& name, matveev::db_t& db)
  {
    auto it = db.find(name);
    if (it == db.end())
    {
      throw std::logic_error("note not found");
    }
    return it->second;
  }
}

void matveev::create_note(std::istream& in, std::ostream&, db_t& db)
{
  std::string name = readArg(in);
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
  std::string name = readArg(in);
  auto& note = findNote(name, db);
  std::string text;
  if (!(in >> std::quoted(text)))
  {
    throw std::logic_error("missing quoted text");
  }
  note->lines.push_back(text);
}

void matveev::show_note(std::istream& in, std::ostream& out, db_t& db)
{
  std::string name = readArg(in);
  auto& note = findNote(name, db);
  for (const auto& line : note->lines)
  {
    out << line << "\n";
  }
}

void matveev::drop_note(std::istream& in, std::ostream&, db_t& db)
{
  std::string name = readArg(in);
  if (!db.erase(name))
  {
    throw std::logic_error("note not found");
  }
}

void matveev::link_note(std::istream& in, std::ostream&, db_t& db)
{
  std::string from = readArg(in);
  std::string to = readArg(in);
  auto& note_from = findNote(from, db);
  auto& note_to = findNote(to, db);
  auto& links = note_from->links;
  auto dup = std::find_if(links.begin(), links.end(),
    [&note_to](const std::pair< std::string, std::weak_ptr< Note > >& p)
    {
      auto locked = p.second.lock();
      return locked && locked == note_to;
    }
  );
  if (dup != links.end())
  {
    throw std::logic_error("link already exists");
  }
  links.push_back({to, note_to});
}

void matveev::mind_note(std::istream& in, std::ostream& out, db_t& db)
{
  std::string name = readArg(in);
  auto& note = findNote(name, db);
  for (const auto& link : note->links)
  {
    if (auto locked = link.second.lock())
    {
      out << locked->name << "\n";
    }
  }
}

void matveev::halt_note(std::istream& in, std::ostream&, db_t& db)
{
  std::string from = readArg(in);
  std::string to = readArg(in);
  auto& note = findNote(from, db);
  auto& links = note->links;
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

void matveev::expired_note(std::istream& in, std::ostream& out, db_t& db)
{
  std::string name = readArg(in);
  auto& note = findNote(name, db);
  size_t count = 0;
  for (const auto& link : note->links)
  {
    if (link.second.expired())
    {
      ++count;
    }
  }
  out << count << "\n";
}

void matveev::refresh_note(std::istream& in, std::ostream&, db_t& db)
{
  std::string name = readArg(in);
  auto& note = findNote(name, db);
  auto& links = note->links;
  auto new_end = std::remove_if(links.begin(), links.end(),
    [](const std::pair< std::string, std::weak_ptr< Note > >& p)
    {
      return p.second.expired();
    }
  );
  links.erase(new_end, links.end());
}
