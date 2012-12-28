#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <boost/utility.hpp>
#include "TmxMap.h"
#include "Math.h"
#include "Sprite.h"

class Engine;

class Map : boost::noncopyable
{
 public:
  Map();
  bool loadFromFile(const std::string& filename);

  void draw(Engine *e) const;
  void getObjects(std::vector<Sprite>& v);

 private:
  void drawLayer(Engine *e, const tmx::Layer& layer) const;
  Rect<int> rectForTile(unsigned globalId) const;

  tmx::Map m_tmxMap;
};

#endif
