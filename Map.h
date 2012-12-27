#ifndef MAP_H
#define MAP_H

#include <string>
#include <boost/utility.hpp>
#include "TmxMap.h"

class Engine;

class Map : boost::noncopyable
{
 public:
  Map();
  bool loadFromFile(const std::string& filename);

  void draw(Engine *e);

 private:
  tmx::Map m_tmxMap;
};

#endif
