#include "Map.h"

#include <boost/lexical_cast.hpp>
#include <iostream>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace boost;

Map::Map(const std::string& filename)
{
  // File has to exist while working with XML
  rapidxml::file<> xmlfile(filename.c_str());
 
  using namespace rapidxml;
 
  xml_document<> doc;
  doc.parse<0>(xmlfile.data());

  xml_node<> *map_node = doc.first_node("map");

  try {
    version = map_node->first_attribute("version")->value();
    orientation = map_node->first_attribute("orientation")->value();
    width = lexical_cast<int>(map_node->first_attribute("width")->value());
    height = lexical_cast<int>(map_node->first_attribute("height")->value());
    tileWidth = lexical_cast<int>(map_node->first_attribute("tilewidth")->value());
    tileHeight = lexical_cast<int>(map_node->first_attribute("tileheight")->value());
  } catch (const bad_lexical_cast& e) {
    std::cerr << e.what() << std::endl;
  }
}
