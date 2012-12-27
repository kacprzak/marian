#include "ResourceMgr.h"
#include <iostream>

ResourceMgr::ResourceMgr()
{
  std::cout << "ResourceMgr created\n";
}

ResourceMgr::~ResourceMgr()
{
  // Delete textures
  for (auto item : m_textures) {
    item.second->release();
    delete item.second;
  }

  std::cout << "ResourceMgr destroyed\n";
}

void ResourceMgr::setDataFolder(const std::string& folder)
{
  dataFolder = folder;
}


bool ResourceMgr::addTexture(const std::string& filename)
{
  Texture *tex = new Texture;

  if(!tex->loadFromFile(dataFolder + filename))
    return false;

  m_textures.push_back(std::make_pair(filename, tex));
  return true;
}

const Texture* ResourceMgr::getTexture(const std::string& filename)
{
  for (auto item : m_textures) {
    if (item.first == filename)
      return item.second;
  }

  return 0;
}
