/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "TmxMap.h"

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include "rapidxml/rapidxml_utils.hpp"
#include "base64/base64.h"

#include <iostream>

static std::vector<unsigned char> decompress(std::string data, int expectedSize = 256);

namespace tmx {

using namespace boost;

//------------------------------------------------------------------------------

Map::Map()
  : width(0)
  , height(0)
  , tileWidth(0)
  , tileHeight(0)
{
  //
}

//------------------------------------------------------------------------------

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

        xml_node<> *tileset_node = map_node->first_node("tileset");  
        loadTilesets(tileset_node);

        xml_node<> *layer_node = map_node->first_node("layer");
        loadLayers(layer_node);
            
        xml_node<> *objectGroup_node = map_node->first_node("objectgroup");
        loadObjectGroups(objectGroup_node);

    } catch (const bad_lexical_cast& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
  
    return true;
}

//------------------------------------------------------------------------------

void Map::loadTilesets(rapidxml::xml_node<> *tileset_node)
{
    using namespace rapidxml;

    while (tileset_node) {
        Tileset tileset;

        tileset.firstGid    = lexical_cast<unsigned>(tileset_node->first_attribute("firstgid")->value());
        tileset.name        = tileset_node->first_attribute("name")->value();
        tileset.tileWidth   = lexical_cast<int>(tileset_node->first_attribute("tilewidth")->value());
        tileset.tileHeight  = lexical_cast<int>(tileset_node->first_attribute("tileheight")->value());
        tileset.imageSource = tileset_node->first_node("image")->first_attribute("source")->value();

        tileset.spacing = 0;
        xml_attribute<> *spacing_attr = tileset_node->first_attribute("spacing");
        if (spacing_attr)
            tileset.spacing = lexical_cast<int>(spacing_attr->value());

        tileset.margin = 0;
        xml_attribute<> *margin_attr = tileset_node->first_attribute("margin");
        if (margin_attr)
            tileset.margin = lexical_cast<int>(margin_attr->value());

        xml_node<> *image_node = tileset_node->first_node("image");
        tileset.imageSource = image_node->first_attribute("source")->value();
        tileset.imageWidth  = lexical_cast<int>(image_node->first_attribute("width")->value());
        tileset.imageHeight = lexical_cast<int>(image_node->first_attribute("height")->value());

        this->tilesets.push_back(tileset);       
        tileset_node = tileset_node->next_sibling("tileset");
    }
}

//------------------------------------------------------------------------------

void Map::loadLayers(rapidxml::xml_node<> *layer_node)
{
    using namespace rapidxml;

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

            std::vector<unsigned char> data;
            if (layer.compression.empty()) {
                std::string decoded = base64_decode(nodevalue);
                for (size_t i = 0; i < decoded.size(); ++i) {
                    data.push_back(decoded[i]);
                }
            } else {
                data = decompress(base64_decode(nodevalue));
            }

            for (unsigned i = 0; i < data.size(); i += 4) {
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
}

//------------------------------------------------------------------------------

void Map::loadObjectGroups(rapidxml::xml_node<> *objectGroup_node)
{
    using namespace rapidxml;

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

                    object.points.push_back(std::pair<int, int>(x, y));
                } 
            }

            objectGroup.objects.push_back(object);
            object_node = object_node->next_sibling("object");
        }

        objectGroups.push_back(objectGroup);
        objectGroup_node = objectGroup_node->next_sibling("objectgroup");
    }
}

//------------------------------------------------------------------------------

std::vector<Property> Map::loadProperties(rapidxml::xml_node<> *node) const
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

//==============================================================================

#include <zlib.h>

static void logZlibError(int error)
{
    switch (error) {
    case Z_MEM_ERROR:
        std::cerr << "Out of memory while (de)compressing data!\n";
        break;
    case Z_VERSION_ERROR:
        std::cerr << "Incompatible zlib version!\n";
        break;
    case Z_NEED_DICT:
    case Z_DATA_ERROR:
        std::cerr << "Incorrect zlib compressed data!\n";
        break;
    default:
        std::cerr << "Unknown error while (de)compressing data!\n";
    }
}

//------------------------------------------------------------------------------

// gzip or zlib
static std::vector<unsigned char> decompress(std::string data, int expectedSize)
{
    std::vector<Bytef> out(expectedSize);
    const Bytef* in = reinterpret_cast<const Bytef*>(data.c_str());

    z_stream strm;

    strm.zalloc    = Z_NULL;
    strm.zfree     = Z_NULL;
    strm.opaque    = Z_NULL;
    strm.next_in   = (Bytef *) in;
    strm.avail_in  = data.length();
    strm.next_out  = (Bytef *) out.data();
    strm.avail_out = out.size();

    int ret = inflateInit2(&strm, 15 + 32);

    if (ret != Z_OK) {
        logZlibError(ret);
        return std::vector<unsigned char>();
    }

    do {
        ret = inflate(&strm, Z_SYNC_FLUSH);

        switch (ret) {
            case Z_NEED_DICT:
            case Z_STREAM_ERROR:
                ret = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                inflateEnd(&strm);
                logZlibError(ret);
                return std::vector<unsigned char>();
        }

        if (ret != Z_STREAM_END) {
            int oldSize = out.size();
            out.resize(out.size() * 2);

            strm.next_out = (Bytef *)(out.data() + oldSize);
            strm.avail_out = oldSize;
        }
    }
    while (ret != Z_STREAM_END);

    if (strm.avail_in != 0) {
        logZlibError(Z_DATA_ERROR);
        return std::vector<unsigned char>();
    }

    const int outLength = out.size() - strm.avail_out;
    inflateEnd(&strm);

    out.resize(outLength);
    return out;
}
