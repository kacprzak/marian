/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "ResourceMgr.h"

#include "Logger.h"
#include "Util.h"

#include <memory>
#include <stdexcept>

ResourceMgr::ResourceMgr()
{
    LOG << "created ResourceMgr\n";
}

//------------------------------------------------------------------------------

ResourceMgr::~ResourceMgr()
{
    release();
    LOG << "destroyed ResourceMgr\n";
}

//------------------------------------------------------------------------------

void ResourceMgr::setDataFolder(const std::string& folder)
{
    m_dataFolder = appendDirSeparator(folder);
}

//------------------------------------------------------------------------------

void ResourceMgr::release()
{
    releaseTextures();
}

//------------------------------------------------------------------------------

void ResourceMgr::addTexture(const std::string& filename)
{
    if (m_textures.find(filename) != m_textures.end()) {
        LOG_WARNING << "Trying to double load " << filename << " texture!\n";
        return;
    }

    std::unique_ptr<gfx::Texture> tex(new gfx::Texture);
    std::string fullpath = m_dataFolder + filename;

    tex->loadFromFile(fullpath);

    m_textures.insert(std::make_pair(filename, tex.release()));

    LOG << "Loaded texture from: " << fullpath << std::endl;
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

const gfx::Texture* ResourceMgr::getTexture(const std::string& filename)
{
    auto it = m_textures.find(filename);
    
    if (it != std::end(m_textures)) {
        return it->second;
    } else {
        throw std::logic_error("Texture " + filename + " was not loaded!");
    }
}
