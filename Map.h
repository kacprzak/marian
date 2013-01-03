#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <boost/utility.hpp>
#include "TmxMap.h"
#include "Math.h"
#include "Sprite.h"

class Engine;

class Layer : boost::noncopyable
{
 public:
  Layer(const tmx::Map& map, const tmx::Layer& layer);
  ~Layer();

  void draw(Engine *e) const;  

  std::string name;
  int width;
  int height;
  std::vector<Sprite *> sprites;
};

class Map : boost::noncopyable
{
 public:
  Map();
  ~Map();
  bool loadFromFile(const std::string& filename);

  Vector2<int> pixelSize() const;
  Vector2<int> tileSize() const;

  void draw(Engine *e) const;
  void drawLayer(Engine *e, const std::string& layer) const;
  void getObjects(std::vector<Sprite>& v);
  unsigned getTileGidAt(int x, int y, const std::string& layer);
  Rect<int> getTileRectAt(int x, int y);

  static Rect<int> rectForTile(const tmx::Map& map, unsigned globalId);

 private:
  tmx::Map m_tmxMap;  
  std::vector<Layer *> m_layers;
};

#endif
