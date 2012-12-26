#ifndef RESOURCE_MGR_H
#define RESOURCE_MGR_H

#include <vector>
#include <utility>
#include "Texture.h"

class ResourceMgr
{
 public:
  static ResourceMgr& instance()
    {
      static ResourceMgr s_instance;
      return s_instance;
    }

  ~ResourceMgr();

  // Textures management
  bool addTexture(const std::string& filename);
  Texture *getTexture(const std::string& filename);

 private:
  ResourceMgr();

  std::vector<std::pair<std::string, Texture*> > m_textures;
};

#endif
