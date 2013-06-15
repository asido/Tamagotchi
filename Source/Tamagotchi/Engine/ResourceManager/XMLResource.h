#ifndef __XMLRESOURCE_H__
#define __XMLRESOURCE_H__

#include <string>
#include <memory>

#include <tinyxml2.h>

#include "ResourceManager.h"

//-----------------------------------------------------------------------------------------------------------
//  class XMLResourceExtraData
//-----------------------------------------------------------------------------------------------------------

class XMLResourceLoader;

class XMLResourceExtraData : public IResourceExtraData
{
    friend class XMLResourceLoader;

public:
    tinyxml2::XMLElement*   GetRoot();

private:
    bool                    ParseXML(char *rawBuffer);

private:
    tinyxml2::XMLDocument   xmlDocument;
};


//-----------------------------------------------------------------------------------------------------------
//  class XMLResourceLoader
//-----------------------------------------------------------------------------------------------------------

class XMLResourceLoader : public IResourceLoader
{
public:
    virtual bool            UseRawFile() override { return false; }
    virtual bool            DiscardRawBufferAfterLoad() override { return true; }
    virtual bool            AddNullZero() override { return true; }
    virtual unsigned int    GetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) override;
    virtual bool            LoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<ResourceHandle> handle) override;
    virtual std::string     GetPattern() override { return "*.xml"; }
};

#endif // __XMLRESOURCE_H__