#ifndef MATVEEV_NOTE_HPP
#define MATVEEV_NOTE_HPP
 
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
 
namespace matveev
{
  struct Note
  {
    std::string name;
    std::vector< std::string > lines;
  };
 
  using db_t = std::unordered_map< std::string, std::shared_ptr< Note > >;
}
 
#endif
