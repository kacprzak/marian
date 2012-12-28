#include "TmxMap.h"

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

using namespace boost;

namespace tmx {

Map::Map()
  : width(0)
  , height(0)
  , tileWidth(0)
  , tileHeight(0)
{
  //
}

bool Map::loadFromFile(const std::string& filename)
{
  // File has to exist while working with XML
  rapidxml::file<> xmlfile(filename.c_str());
 
  using namespace rapidxml;
 
  xml_document<> doc;
  doc.parse<0>(xmlfile.data());

  xml_node<> *map_node = doc.first_node("map");

  try {
    version     = map_node->first_attribute("version")->value();
    orientation = map_node->first_attribute("orientation")->value();
    width       = lexical_cast<int>(map_node->first_attribute("width")->value());
    height      = lexical_cast<int>(map_node->first_attribute("height")->value());
    tileWidth   = lexical_cast<int>(map_node->first_attribute("tilewidth")->value());
    tileHeight  = lexical_cast<int>(map_node->first_attribute("tileheight")->value());

    xml_node<> *tileset_node = map_node->first_node("tileset");
    
    while (tileset_node) {
        Tileset tileset;
        
        tileset.firstGid    = lexical_cast<unsigned>(tileset_node->first_attribute("firstgid")->value());
        tileset.name        = tileset_node->first_attribute("name")->value();
        tileset.tileWidth   = lexical_cast<int>(tileset_node->first_attribute("tilewidth")->value());
        tileset.tileHeight  = lexical_cast<int>(tileset_node->first_attribute("tileheight")->value());
        tileset.imageSource = tileset_node->first_node("image")->first_attribute("source")->value();

        xml_node<> *image_node = tileset_node->first_node("image");
        tileset.imageSource = image_node->first_attribute("source")->value();
        tileset.imageWidth  = lexical_cast<int>(image_node->first_attribute("width")->value());
        tileset.imageHeight = lexical_cast<int>(image_node->first_attribute("height")->value());
        
        tilesets.push_back(tileset);       
        tileset_node = tileset_node->next_sibling("tileset");
    }

    xml_node<> *layer_node = map_node->first_node("layer");

    while (layer_node) {
      Layer layer;

      layer.name   = layer_node->first_attribute("name")->value();
      layer.width  = lexical_cast<int>(layer_node->first_attribute("width")->value());
      layer.height = lexical_cast<int>(layer_node->first_attribute("height")->value());

      xml_node<> *data_node = layer_node->first_node("data");
      layer.dataEncoding = data_node->first_attribute("encoding")->value();

      if (layer.dataEncoding == "csv") {
        //std::cout << data_node->value();
        std::string data = data_node->value();

        boost::char_separator<char> sep(", \t\n\r");
        boost::tokenizer< boost::char_separator<char> > tok(data, sep);
        for (auto it = tok.begin(); it != tok.end(); ++it) {
          layer.data.push_back(lexical_cast<unsigned>(*it));
        }
      } else {
        std::cerr << "WARNING: Map layer data loading from " << layer.dataEncoding
                  << " is not implemented." << std::endl;
      }

      layers.push_back(layer);
      layer_node = layer_node->next_sibling("layer");
    }
    
    xml_node<> *objectGroup_node = map_node->first_node("objectgroup");

    while (objectGroup_node) {
      ObjectGroup objectGroup;

      objectGroup.name   = objectGroup_node->first_attribute("name")->value();
      objectGroup.width  = lexical_cast<int>(objectGroup_node->first_attribute("width")->value());
      objectGroup.height = lexical_cast<int>(objectGroup_node->first_attribute("height")->value());

      xml_node<> *object_node = objectGroup_node->first_node("object");

      while (object_node) {
        Object object;

        object.gid = lexical_cast<unsigned>(object_node->first_attribute("gid")->value());
        object.x   = lexical_cast<int>(object_node->first_attribute("x")->value());
        object.y   = lexical_cast<int>(object_node->first_attribute("y")->value());

        objectGroup.objects.push_back(object);
        object_node = object_node->next_sibling("object");
      }

      objectGroups.push_back(objectGroup);
      objectGroup_node = objectGroup_node->next_sibling("objectgroup");
    }

  } catch (const bad_lexical_cast& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  
  return true;
}

} // namespace tmx
