#include "FontResource.h"
#include "Logger.h"

//-----------------------------------------------------------------------------------------------------------
//  class FontResourceExtraData
//-----------------------------------------------------------------------------------------------------------

FontResourceExtraData::~FontResourceExtraData()
{
    if (this->face)
    {
        FT_Done_Face(this->face);
    }
}

//-----------------------------------------------------------------------------------------------------------
//  class FontResourceLoader
//-----------------------------------------------------------------------------------------------------------

FontResourceLoader::FontResourceLoader()
{
    if (FT_Init_FreeType(&this->library))
    {
        LogError("FT_Init_FreeType() has failed.");
    }
}

FontResourceLoader::~FontResourceLoader()
{
    FT_Done_FreeType(this->library);
}

const std::string& FontResourceLoader::GetPattern() const
{
    static const std::string pattern("*.ttf");
    return pattern;
}

unsigned int FontResourceLoader::GetLoadedResourceSize(char *rawBuffer, unsigned int rawSize)
{
    // Keep the resource cache from allocating memory for the .ttf file since we are directly
    // using the rawBuffer as a buffer.
    return 0;
}

bool FontResourceLoader::LoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<ResourceHandle> handle)
{
    // We are actually using raw buffer, but we override UseRawFile returning false in order to be able to create
    // a new face in this function.
    handle->buffer = rawBuffer;

    std::shared_ptr<FontResourceExtraData> extra = std::shared_ptr<FontResourceExtraData>(TG_NEW FontResourceExtraData);

    if (FT_New_Memory_Face(this->library, reinterpret_cast<FT_Byte*>(rawBuffer), rawSize, 0, &extra->face))
    {
        LogError("FT_New_Memory_Face() has failed.");
        return false;
    }

    handle->SetExtra(extra);

    return true;
}