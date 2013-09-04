/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef RESOURCE_MGR_H
#define RESOURCE_MGR_H

#include "Singleton.h"
#include <map>
#include <utility>

#include "graphics/Texture.h"

class ResourceMgr final : public Singleton<ResourceMgr>
{
 public:
    ResourceMgr();

    ~ResourceMgr();

    // Set data folder ex: "media/"
    void setDataFolder(const std::string& folder);

    // Releases all resources.
    void release();

    // Textures management
    bool addTexture(const std::string& filename);
    const Texture* getTexture(const std::string& filename);
    void releaseTextures();

 private:
    std::string dataFolder;
    std::map<std::string, Texture*> m_textures;
};

#endif
