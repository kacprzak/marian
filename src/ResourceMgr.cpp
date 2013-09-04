/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "ResourceMgr.h"

#include "Logger.h"

ResourceMgr::ResourceMgr()
{
    LOG << "ResourceMgr created\n";
}

//------------------------------------------------------------------------------

ResourceMgr::~ResourceMgr()
{
    release();
    LOG << "ResourceMgr destroyed\n";
}

//------------------------------------------------------------------------------

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
    if (m_textures.find(filename) != m_textures.end()) {
        LOG_WARNING << "Warning: Trying to double load " << filename << " texture!" << std::endl;
        return false;
    }

    Texture *tex = new Texture;
    std::string fullpath = dataFolder + filename;

    if(!tex->loadFromFile(fullpath)) {
        LOG_ERROR << "Error: Unable to load " << filename << " texture!" << std::endl;
        delete tex;
        return false;
    }

    m_textures.insert(std::make_pair(filename, tex));
    LOG << "Loaded texture from: " << fullpath << std::endl;

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

    std::cerr << "Error: Texture " << filename << " was not loaded!" << std::endl;

    return 0;
}
