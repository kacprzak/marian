#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <boost/utility.hpp>
#include "TmxMap.h"
#include "Math.h"
#include "Sprite.h"

class Engine;
class Layer;

class Map : boost::noncopyable
{
 public:
  Map();
  ~Map();
  bool loadFromFile(const std::string& filename);

  Vector2<int> pixelSize() const;
  Vector2<int> tileSize() const;

  int tileWidth() const { return m_tmxMap.tileWidth; }
  int tileHeight() const { return m_tmxMap.tileHeight; }

  void draw(Engine *e, int xFrom, int xTo, int yFrom, int yTo) const;
  void drawLayer(Engine *e, const std::string& layer,
                 int xFrom, int xTo, int yFrom, int yTo) const;

  void getObjects(std::vector<Sprite>& v);
  unsigned getTileGidAt(int x, int y, const std::string& layer);
  Rect<int> getTileRectAt(int x, int y);

  std::vector<std::string> externalImages() const;

  std::string imageForTile(unsigned globalId) const;
  Rect<int> rectForTile(unsigned globalId) const;

 private:
  tmx::Map m_tmxMap;  
  std::vector<Layer *> m_layers;
};

#endif
