#include <iostream>
#include <fstream>
#include "EngineConfig.h"
#include "Logger.h"
#include "ResourceManager/Filesystem.h"

//-----------------------------------------------------------------------------------------------------------
//  class EngineConfig
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

bool EngineConfig::Init(const std::string &configFile)
{
    if (configFile.size() == 0)
    {
        LogError("Config file is not provided.");
        return false;
    }

    const std::string absoluteConfigFile(Filesystem::GetRootPath() + configFile);

    std::ifstream stream(absoluteConfigFile, std::ios::in);
    if (!stream)
    {
        LogError("Couldn't open config file: %s", absoluteConfigFile.c_str());
        return false;
    }

    stream.seekg(0, std::ios::end);
    std::string rawXml;
    rawXml.resize(static_cast<unsigned int>(stream.tellg()));
    stream.seekg(0, std::ios::beg);
    stream.read(&rawXml[0], rawXml.size());
    stream.close();

    tinyxml2::XMLDocument configXmlDoc;
    tinyxml2::XMLError xmlError = configXmlDoc.Parse(rawXml.c_str());
    if (xmlError != tinyxml2::XML_NO_ERROR)
    {
        LogError("Error parsing engine config: %d", xmlError);
        return false;
    }

    if (!ReadAllConfigs(configXmlDoc.RootElement()))
    {
        LogError("Reading engine config has failed.");
        return false;
    }

    return true;
}

//-----------------------------------------------
// Private
//-----------------------------------------------

bool EngineConfig::ReadAllConfigs(tinyxml2::XMLElement *data)
{
    bool result = true;

    if (data == NULL)
    {
        LogError("data == NULL.");
        return false;
    }

    this->assetFile         = std::string(ReadConfig(data, "AssetFile"));
    this->assetPathActors   = std::string(ReadConfig(data, "AssetPathActors"));
    this->assetPathLevels   = std::string(ReadConfig(data, "AssetPathLevels"));
    this->assetPathShaders  = std::string(ReadConfig(data, "AssetPathShaders"));
    this->assetPathTextures = std::string(ReadConfig(data, "AssetPathTextures"));
    this->assetPathFonts    = std::string(ReadConfig(data, "AssetPathFonts"));

    this->fontDpi           = atoi(ReadConfig(data, "FontDPI"));
    this->fontSymbols       = std::string(ReadConfig(data, "FontSymbols"));

    this->resourceCacheSize = static_cast<float>(atof(ReadConfig(data, "ResourceCacheSize")));

    this->firstScene        = std::string(ReadConfig(data, "FirstScene"));

    return result;
}

const char* EngineConfig::ReadConfig(tinyxml2::XMLElement *data, const std::string &elementName) const
{
    tinyxml2::XMLElement *element = data->FirstChildElement(elementName.c_str());
    if (element)
    {
        const char *value = element->FirstChild()->Value();
        LogInfo("Config loaded: %s=%s", elementName.c_str(), value);
        return value;
    }
    else
    {
        LogError("Engine config is missing tag: %s", elementName.c_str());
        return NULL;
    }
}