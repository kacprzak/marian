#ifndef TMX_MAP_H
#define TMX_MAP_H

#include <string>
#include <vector>
#include <boost/utility.hpp>

namespace tmx
{

class Tileset
{
 public:
  std::string name;
  int firstGid;
  int tileWidth;
  int tileHeight;
  std::string imageSource;
};

class Layer 
{
 public:
  std::string name;
  int width;
  int height;
  std::string dataEncoding;
  std::vector<int> data;
};

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

  std::vector<Tileset> tilesets;
  std::vector<Layer> layers;
};

} // namespace tmx

#endif
