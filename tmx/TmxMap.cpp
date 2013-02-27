/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "TmxMap.h"

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "base64/base64.h"

#include <sstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>

namespace tmx {

using namespace boost;

// gzip or zlib
std::string decompress(std::string data, std::string method)
{
    if (!method.empty()) {
        std::stringstream ss(data);
        iostreams::filtering_streambuf<iostreams::input> in;

        if (method == "gzip")
            in.push(iostreams::gzip_decompressor());
        else if (method == "zlib") 
            in.push(iostreams::zlib_decompressor());

        in.push(ss);
        std::stringstream dst;
        iostreams::copy(in, dst);
        return dst.str();
    } else
        return data;
}


std::vector<Property> loadProperties(rapidxml::xml_node<> *node)
{
    using namespace rapidxml;
    std::vector<Property> returnValue; 

    xml_node<> *properties_node = node->first_node("properties");
    if (properties_node) {
        xml_node<> *property_node = properties_node->first_node("property");
        while (property_node) {
            Property prop;
            prop.name  = property_node->first_attribute("name")->value();
            prop.value = property_node->first_attribute("value")->value();

            returnValue.push_back(prop);

            property_node = property_node->next_sibling("property");
        }
    }

    return returnValue;
}

//------------------------------------------------------------------------------

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

        xml_attribute<> *bgColor_attr = map_node->first_attribute("backgroundcolor");
        if (bgColor_attr) backgroundColor = bgColor_attr->value();

        properties = loadProperties(map_node);

        //---------- TILESETS ----------

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

        //---------- LAYERS ----------

        xml_node<> *layer_node = map_node->first_node("layer");

        while (layer_node) {
            Layer layer;

            layer.name   = layer_node->first_attribute("name")->value();
            layer.width  = lexical_cast<int>(layer_node->first_attribute("width")->value());
            layer.height = lexical_cast<int>(layer_node->first_attribute("height")->value());

            layer.visible = "1";
            xml_attribute<> *visible_attr = layer_node->first_attribute("visible");
            if (visible_attr)
                layer.visible = visible_attr->value();

            layer.properties = loadProperties(layer_node);

            // Data loading
            xml_node<> *data_node = layer_node->first_node("data");
            layer.dataEncoding = data_node->first_attribute("encoding")->value();

            xml_attribute<> *compression_attr = data_node->first_attribute("compression");
            if (compression_attr) layer.compression = compression_attr->value();

            if (layer.dataEncoding == "csv") {

                std::string data = data_node->value();

                boost::char_separator<char> sep(", \t\n\r");
                boost::tokenizer< boost::char_separator<char> > tok(data, sep);
                for (auto it = tok.begin(); it != tok.end(); ++it) {
                    layer.data.push_back(lexical_cast<unsigned>(*it));
                }
            }
            else if (layer.dataEncoding == "base64") {

                std::string nodevalue = data_node->value();
                boost::algorithm::trim(nodevalue);
                std::string datastr = decompress(base64_decode(nodevalue), layer.compression);

                const unsigned char *data = reinterpret_cast<const unsigned char*>(datastr.data());

                for (unsigned i = 0; i < datastr.size(); i += 4) {
                    unsigned global_tile_id = data[i]
                        | data[i + 1] << 8
                        | data[i + 2] << 16
                        | data[i + 3] << 24;
          
                    layer.data.push_back(global_tile_id);
                }
            }
            else {
                std::cerr << "WARNING: Map layer data loading from " << layer.dataEncoding
                          << " is not implemented." << std::endl;
            }

            layers.push_back(layer);
            layer_node = layer_node->next_sibling("layer");
        }
    
        //---------- OBJECTS ----------

        xml_node<> *objectGroup_node = map_node->first_node("objectgroup");

        while (objectGroup_node) {
            ObjectGroup objectGroup;

            objectGroup.name   = objectGroup_node->first_attribute("name")->value();
            objectGroup.width  = lexical_cast<int>(objectGroup_node->first_attribute("width")->value());
            objectGroup.height = lexical_cast<int>(objectGroup_node->first_attribute("height")->value());

            objectGroup.properties = loadProperties(objectGroup_node);

            xml_node<> *object_node = objectGroup_node->first_node("object");

            while (object_node) {
                Object object;

                xml_attribute<> *name_attr = object_node->first_attribute("name");
                if (name_attr) object.name = name_attr->value();

                xml_attribute<> *type_attr = object_node->first_attribute("type");
                if (type_attr) object.type = type_attr->value();

                xml_attribute<> *gid_attr = object_node->first_attribute("gid");
                object.gid = (gid_attr) ? lexical_cast<unsigned>(gid_attr->value()) : 0;

                object.x   = lexical_cast<int>(object_node->first_attribute("x")->value());
                object.y   = lexical_cast<int>(object_node->first_attribute("y")->value());

                xml_attribute<> *width_attr = object_node->first_attribute("width");
                object.width = (width_attr) ? lexical_cast<unsigned>(width_attr->value()) : 0;

                xml_attribute<> *height_attr = object_node->first_attribute("height");
                object.height = (height_attr) ? lexical_cast<unsigned>(height_attr->value()) : 0;

                object.visible = "1";
                xml_attribute<> *visible_attr = object_node->first_attribute("visible");
                if (visible_attr)
                    object.visible = visible_attr->value();

                object.properties = loadProperties(object_node);

                xml_node<> *shape_node;
                bool readPoints = false;
                if ((shape_node = object_node->first_node("ellipse"))) {
                    object.shape = "ellipse";
                } else if ((shape_node = object_node->first_node("polygon"))) {
                    object.shape = "polygon";
                    readPoints = true;
                } else if ((shape_node = object_node->first_node("polyline"))) {
                    object.shape = "polyline";
                    readPoints = true;
                }

                if (readPoints) {
                    std::string pointsStr = shape_node->first_attribute("points")->value();

                    boost::char_separator<char> sep(", ");
                    boost::tokenizer< boost::char_separator<char> > tok(pointsStr, sep);
                    for (auto it = tok.begin(); it != tok.end(); ++it) {
                        int x = lexical_cast<int>(*it);
                        ++it;
                        int y = lexical_cast<int>(*it);

                        object.points.push_back({x, y});
                    } 
                }

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

//------------------------------------------------------------------------------

const Tileset* Map::tilesetForTile(unsigned gid) const
{
    for (int i = tilesets.size() - 1; i >= 0; --i) {
        const tmx::Tileset& tileset = tilesets[i];
    
        if (tileset.firstGid <= gid) {
            return &tileset;
        }
    }

    // Should never happen
    std::cerr << "ERROR: Unable to find tileset for tile #" << gid << "\n";
    return nullptr;
}

} // namespace tmx
