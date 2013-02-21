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
  unsigned firstGid;
  int tileWidth;
  int tileHeight;
  std::string imageSource;
  int imageWidth;
  int imageHeight;
};

class Layer 
{
 public:
  std::string name;
  int width;
  int height;
  std::string dataEncoding;
  std::vector<unsigned> data;
};

class Object
{
 public:
  unsigned gid; //< 0 by default
  int x;
  int y;
};

// In fact it's ObjectLayer
class ObjectGroup
{
 public:
  std::string name;
  int width;
  int height;
  std::vector<Object> objects;
};

class Map : boost::noncopyable
{
 public:
  Map();
  bool loadFromFile(const std::string& filename);
  const tmx::Tileset* tilesetForTile(unsigned gid) const;
 
  std::string version;
  std::string orientation;
  int width;
  int height;
  int tileWidth;
  int tileHeight;

  std::vector<Tileset> tilesets;
  std::vector<Layer> layers;
  std::vector<ObjectGroup> objectGroups;
};

} // namespace tmx

#endif
