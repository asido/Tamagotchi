#ifndef __ENGINECONFIG_H__
#define __ENGINECONFIG_H__

#include <string>
#include <tinyxml2.h>

//-----------------------------------------------------------------------------------------------------------
//  class EngineConfig
//-----------------------------------------------------------------------------------------------------------

class EngineConfig
{
public:
    bool Init(const std::string &configFile);

    std::string GetAssetFile() const { return this->assetFile; }
    std::string GetAssetPathActors() const { return this->assetPathActors; }
    std::string GetAssetPathLevels() const { return this->assetPathLevels; }
    std::string GetAssetPathShaders() const { return this->assetPathShaders; }
    std::string GetAssetPathTextures() const { return this->assetPathTextures; }
    float       GetResourceCacheSize() const { return this->resourceCacheSize; }
    std::string GetFirstScene() const { return this->firstScene; }

private:
    bool        ReadAllConfigs(tinyxml2::XMLElement *data);
    const char* ReadConfig(tinyxml2::XMLElement *data, const std::string &elementName) const;

private:
    std::string assetFile;
    std::string assetPathActors;
    std::string assetPathLevels;
    std::string assetPathShaders;
    std::string assetPathTextures;

    float       resourceCacheSize;

    std::string firstScene;
};

#endif // __ENGINECONFIG_H__