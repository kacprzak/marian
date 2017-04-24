/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef TMX_MAP_H
#define TMX_MAP_H

#include <string>
#include <vector>
#include <boost/utility.hpp> // noncopyable
#include "rapidxml/rapidxml.hpp"

namespace tmx {

struct Property
{
    std::string name;
    std::string value;
};

//------------------------------------------------------------------------------

struct Tileset
{
    std::string name;
    unsigned firstGid;
    int tileWidth;
    int tileHeight;
    std::string imageSource;
    int imageWidth;
    int imageHeight;
    int spacing;
    int margin;
};

//------------------------------------------------------------------------------

struct Layer
{
    std::string name;
    int width;
    int height;
    std::string visible; //!< "1" by default
    std::string dataEncoding;
    std::string compression;
    std::vector<unsigned> data;

    std::vector<Property> properties;
};

//------------------------------------------------------------------------------

struct Object
{
    std::string name;
    std::string type;
    unsigned gid; //!< 0 by default
    int x;
    int y;
    int width;    //!< 0 by default
    int height;   //!< 0 by default
    std::string visible; //!< "1" by default
    std::string shape;   //!< ellipse, polygon, polyline or null
    std::vector<std::pair<int, int> > points; //!< polygon or polyline points

    std::vector<Property> properties;
};

//------------------------------------------------------------------------------

// In fact it's ObjectLayer
struct ObjectGroup
{
    std::string name;
    int width;
    int height;
    std::vector<Object> objects;

    std::vector<Property> properties;
};

//------------------------------------------------------------------------------

class Map : private boost::noncopyable
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
    std::string backgroundColor;

    std::vector<Tileset> tilesets;
    std::vector<Layer> layers;
    std::vector<ObjectGroup> objectGroups;

    std::vector<Property> properties;

 private:
    void loadTilesets(rapidxml::xml_node<> *tileset_node);
    void loadLayers(rapidxml::xml_node<> *layer_node);
    void loadObjectGroups(rapidxml::xml_node<> *objectGroup_node);

    std::vector<Property> loadProperties(rapidxml::xml_node<> *node) const;
};

} // namespace tmx

#endif
