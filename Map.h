#ifndef MAP_H
#define MAP_H

#include <string>
#include <boost/utility.hpp>
#include "TmxMap.h"
#include "Math.h"

class Engine;

class Map : boost::noncopyable
{
 public:
  Map();
  bool loadFromFile(const std::string& filename);

  void draw(Engine *e);

 private:
  void drawLayer(Engine *e, const tmx::Layer& layer);
  Rect<int> rectForTile(unsigned globalId);

  tmx::Map m_tmxMap;
};

#endif
