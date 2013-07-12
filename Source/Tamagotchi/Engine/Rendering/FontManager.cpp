#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include "FontManager.h"
#include "Font.h"
#include "TamagotchiEngine.h"
#include "Logger.h"
#include "ResourceManager/FontResource.h"

//-----------------------------------------------------------------------------------------------------------
//  class FontManager
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

bool FontManager::Init()
{
    return true;
}

std::shared_ptr<Font> FontManager::FindFont(const std::string &fontName, unsigned int fontSize)
{
    const std::string fontIdentifier = FontNameSizeToIdentifier(fontName, fontSize);
    std::shared_ptr<Font> font;

    FontMap::iterator it = this->fonts.find(fontIdentifier);
    if (it == this->fonts.end())
    {
        font = LoadFont(fontName, fontSize, fontIdentifier);

        if (!font)
        {
            LogError("Font with identifier '%s' not found.", fontIdentifier.c_str());
            return std::shared_ptr<Font>();
        }
    }
    else
    {
        font = it->second;
    }

    return font;
}

//-----------------------------------------------
// Private
//-----------------------------------------------

const std::string FontManager::FontNameSizeToIdentifier(const std::string &fontName, unsigned int fontSize) const
{
    return fontName + ":" + std::to_string(fontSize);
}

std::shared_ptr<Font> FontManager::LoadFont(const std::string &fontName, unsigned int fontSize, const std::string &fontIdentifier)
{
    LogInfo("Loading font: %s-%d", fontName.c_str(), fontSize);

    const std::string fontsPath = g_engine->GetEngineConfig()->GetAssetPathFonts();
    const Resource fontResource(fontsPath + fontName + ".ttf");
    std::shared_ptr<ResourceHandle> fontHandle = g_engine->GetResourceManager()->GetHandle(fontResource);

    if (!fontHandle)
    {
        LogWarning("Couldn't load font resource handle: %s", fontResource.GetName().c_str());
        return std::shared_ptr<Font>();
    }

    std::shared_ptr<FontResourceExtraData> fontExtra = std::static_pointer_cast<FontResourceExtraData>(fontHandle->GetExtra());
    FT_Face fontFace = fontExtra->GetFtFace();

    // FreeType measures font size in terms of 1/64th of pixel.
    unsigned int size = fontSize * 64;
    unsigned int dpi = g_engine->GetEngineConfig()->GetFontDPI();
    if (FT_Set_Char_Size(fontFace, size, size, dpi, dpi))
    {
        LogError("Error trying to setup font size.");
        return std::shared_ptr<Font>();
    }
    
    std::shared_ptr<Font> font = std::shared_ptr<Font>(TG_NEW Font(fontFace));
    
    const std::string &fontSymbols = g_engine->GetEngineConfig()->GetFontSymbols();
    size_t symbolCount = fontSymbols.size();
    for (size_t i = 0; i < symbolCount; ++i)
    {
        unsigned char c = fontSymbols.at(i);
        unsigned int glyphIndex = FT_Get_Char_Index(fontFace, c);
        if (glyphIndex == 0)
        {
            LogError("Font '%s' does not have symbol '%c'.", fontName.c_str(), c);
            return std::shared_ptr<Font>();
        }

        if (FT_Load_Glyph(fontFace, glyphIndex, FT_LOAD_DEFAULT))
        {
            LogError("Failed to load glyph '%c' at index %d from font '%s'", c, glyphIndex, fontName.c_str());
            return std::shared_ptr<Font>();
        }

        FT_Glyph glyph;
        if (FT_Get_Glyph(fontFace->glyph, &glyph))
        {
            LogError("Failed to extract glyph image from slot for font '%s', symbol '%c', glyph index '%d'", fontName.c_str(), c, glyphIndex);
            return std::shared_ptr<Font>();
        }

        std::shared_ptr<FontSymbol> fontChar = std::shared_ptr<FontSymbol>(TG_NEW FontSymbol(glyph));

        unsigned int xOffset = fontFace->glyph->metrics.horiBearingX / 64;
        unsigned int yOffset = font->GetLineHeight() - (fontFace->glyph->metrics.horiBearingY / 64);
        fontChar->SetOffset(xOffset, yOffset);

        unsigned int width = fontFace->glyph->metrics.width / 64;
        unsigned int height = fontFace->glyph->metrics.height / 64;
        fontChar->SetSize(width, height);

        unsigned int xAdvance = fontFace->glyph->metrics.horiAdvance / 64;
        fontChar->SetXAdvance(xAdvance);

        if (!font->AddSymbol(c, fontChar))
        {
            LogWarning("Failed to add symbol '%c' to font '%s'.", c, fontName.c_str());
        }
    }
    
    if (!font->CreateAtlas())
    {
        LogError("Failed to create atlas for font: %s", fontName.c_str());
        return std::shared_ptr<Font>();
    }
    
    if (!RegisterFont(font, fontIdentifier))
    {
        LogError("Failed to register font.");
        return std::shared_ptr<Font>();
    }

    return font;
}

bool FontManager::RegisterFont(std::shared_ptr<Font> font, const std::string &fontIdentifier)
{
    std::pair<FontMap::iterator, bool> status = this->fonts.insert(std::make_pair(fontIdentifier, font));
    return status.second;
}