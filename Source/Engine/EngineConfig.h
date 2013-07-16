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

    const std::string&  GetAssetFile() const { return this->assetFile; }
    const std::string&  GetAssetPathActors() const { return this->assetPathActors; }
    const std::string&  GetAssetPathLevels() const { return this->assetPathLevels; }
    const std::string&  GetAssetPathShaders() const { return this->assetPathShaders; }
    const std::string&  GetAssetPathTextures() const { return this->assetPathTextures; }
    const std::string&  GetAssetPathFonts() const { return this->assetPathFonts; }

    unsigned int        GetFontDPI() const { return this->fontDpi; }
    const std::string&  GetFontSymbols() const { return this->fontSymbols; }

    float               GetResourceCacheSize() const { return this->resourceCacheSize; }

    const std::string&  GetFirstScene() const { return this->firstScene; }

private:
    bool                ReadAllConfigs(tinyxml2::XMLElement *data);
    const char*         ReadConfig(tinyxml2::XMLElement *data, const std::string &elementName) const;

private:
    std::string     assetFile;
    std::string     assetPathActors;
    std::string     assetPathLevels;
    std::string     assetPathShaders;
    std::string     assetPathTextures;
    std::string     assetPathFonts;

    unsigned int    fontDpi;
    std::string     fontSymbols;

    float           resourceCacheSize;

    std::string     firstScene;
};

#endif // __ENGINECONFIG_H__