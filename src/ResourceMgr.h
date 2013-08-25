/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef RESOURCE_MGR_H
#define RESOURCE_MGR_H

#include "Singleton.h"
#include <map>
#include <utility>
#include <iostream>
#include "Texture.h"

class ResourceMgr : public Singleton<ResourceMgr>
{
 public:
    ResourceMgr()
    {
        std::clog << "ResourceMgr created\n";
    }

    ~ResourceMgr()
    {
        release();
        std::clog << "ResourceMgr destroyed\n";
    }

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
