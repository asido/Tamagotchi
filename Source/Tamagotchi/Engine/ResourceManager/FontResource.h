#ifndef __FONTRESOURCE_H__
#define __FONTRESOURCE_H__

#include <memory>
#include <string>
#include <ft2build.h>
#include <freetype/freetype.h>
#include "ResourceManager.h"

//-----------------------------------------------------------------------------------------------------------
//  class FontResourceExtraData
//-----------------------------------------------------------------------------------------------------------

class FontResourceExtraData : public IResourceExtraData
{
    friend class FontResourceLoader;

public:
    ~FontResourceExtraData();

    FT_Face GetFtFace() const { return this->face; }

private:
    FT_Face face;
};


//-----------------------------------------------------------------------------------------------------------
//  class FontResourceLoader
//-----------------------------------------------------------------------------------------------------------

class FontResourceLoader : public IResourceLoader
{
public:
    FontResourceLoader();
    ~FontResourceLoader();

    virtual const std::string&  GetPattern() const override;
    virtual bool                UseRawFile() const override { return false; }
    virtual bool                DiscardRawBufferAfterLoad() const override { return false; }
    virtual unsigned int        GetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) override;
    virtual bool                LoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<ResourceHandle> handle) override;

private:
    FT_Library library;
};

#endif // __FONTRESOURCE_H__