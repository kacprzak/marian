#ifndef MAP_H
#define MAP_H

#include <string>
#include <boost/utility.hpp>

class Map : boost::noncopyable
{
 public:
  Map();
  bool loadFromFile(const std::string& filename);

  std::string version;
  std::string orientation;
  int width;
  int height;
  int tileWidth;
  int tileHeight;
};

#endif
