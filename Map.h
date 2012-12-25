#ifndef MAP_H
#define MAP_H

#include <string>

class Map {
 public:
  Map(const std::string& filename);

 private:
  Map(const Map& map); // no impl
  Map& operator=(const Map& map); // no impl

 public:
  std::string version;
  std::string orientation;
  int width;
  int height;
  int tileWidth;
  int tileHeight;
};

#endif
