#include "ResourceMgr.h"
#include <iostream>

ResourceMgr::ResourceMgr()
{
  std::cout << "ResourceMgr created\n";
}

ResourceMgr::~ResourceMgr()
{
  release();
  std::cout << "ResourceMgr destroyed\n";
}

void ResourceMgr::setDataFolder(const std::string& folder)
{
  dataFolder = folder;
}

void ResourceMgr::release()
{
  releaseTextures();
}

bool ResourceMgr::addTexture(const std::string& filename)
{
  Texture *tex = new Texture;

  if(!tex->loadFromFile(dataFolder + filename))
    return false;

  m_textures.push_back(std::make_pair(filename, tex));
  return true;
}

void ResourceMgr::releaseTextures()
{
  // Delete textures
  for (auto item : m_textures) {
    item.second->release();
    delete item.second;
  }
  m_textures.clear();
}

const Texture* ResourceMgr::getTexture(const std::string& filename)
{
  for (auto item : m_textures) {
    if (item.first == filename)
      return item.second;
  }

  return 0;
}