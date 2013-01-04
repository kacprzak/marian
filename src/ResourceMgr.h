#ifndef RESOURCE_MGR_H
#define RESOURCE_MGR_H

#include <vector>
#include <utility>
#include <boost/utility.hpp>
#include "Texture.h"

class ResourceMgr : boost::noncopyable
{
 public:
  static ResourceMgr& instance()
    {
      static ResourceMgr s_instance;
      return s_instance;
    }

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
  ResourceMgr();
  std::string dataFolder;
  std::vector<std::pair<std::string, Texture*> > m_textures;
};

#endif
