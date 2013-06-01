/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "ResourceMgr.h"

void ResourceMgr::setDataFolder(const std::string& folder)
{
    dataFolder = folder;
}

//------------------------------------------------------------------------------

void ResourceMgr::release()
{
    releaseTextures();
}

//------------------------------------------------------------------------------

bool ResourceMgr::addTexture(const std::string& filename)
{
    Texture *tex = new Texture;
    std::string fullpath = dataFolder + filename;

    if(!tex->loadFromFile(fullpath))
        return false;

    m_textures.push_back(std::make_pair(filename, tex));
    std::cout << "Loaded texture from: " << fullpath << std::endl;

    return true;
}

//------------------------------------------------------------------------------

void ResourceMgr::releaseTextures()
{
    // Delete textures
    for (auto item : m_textures) {
        delete item.second;
    }
    m_textures.clear();
}

//------------------------------------------------------------------------------

const Texture* ResourceMgr::getTexture(const std::string& filename)
{
    for (auto item : m_textures) {
        if (item.first == filename)
            return item.second;
    }
    return 0;
}
