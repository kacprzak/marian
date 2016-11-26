#ifndef RESOURCE_MGR_H
#define RESOURCE_MGR_H

#include "Singleton.h"
#include "graphics/Texture.h"

#include <unordered_map>
#include <utility>

class ResourceMgr final : public Singleton<ResourceMgr>
{
  public:
    ResourceMgr();

    ~ResourceMgr() override;

    // Set data folder ex: "media/"
    void setDataFolder(const std::string& folder);
    std::string dataFolder() const { return m_dataFolder; }

    // Releases all resources.
    void release();

    // Textures management
    void addTexture(const std::string& filename);
    const gfx::Texture* getTexture(const std::string& filename);
    void releaseTextures();

  private:
    std::string m_dataFolder;
    std::unordered_map<std::string, gfx::Texture*> m_textures;
};

#endif
