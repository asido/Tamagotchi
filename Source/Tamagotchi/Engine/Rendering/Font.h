#ifndef __FONT_H__
#define __FONT_H__

#include <memory>
#include <map>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

//-----------------------------------------------------------------------------------------------------------
//  class FontSymbol
//      Represents a single font glyph.
//-----------------------------------------------------------------------------------------------------------

class FontSymbol
{
public:
    FontSymbol(FT_Glyph glyph);

    void SetSize(unsigned int width, unsigned int height) { this->width = width; this->height = height; }
    void SetOffset(unsigned int xOffset, unsigned int yOffset) { this->xOffset = xOffset; this->yOffset = yOffset; }
    void setXAdvance(unsigned int xAdvance) { this->xAdvance = xAdvance; }

private:
    FT_Glyph ftGlyph;

    unsigned int width, height;
    unsigned int xOffset, yOffset;
    unsigned int xAdvance;
};


//-----------------------------------------------------------------------------------------------------------
//  class Font
//      Represents a collection of glyphs forming a font.
//-----------------------------------------------------------------------------------------------------------

typedef std::map< unsigned char, std::shared_ptr<FontSymbol> >    FontSymbols;

class Font
{
    friend class FontManager;

public:
    Font(FT_Face fontFace);

    bool            AddSymbol(unsigned char symbol, std::shared_ptr<FontSymbol> fontChar);

    unsigned int    GetLineHeight() const { return this->lineHeight; }

private:
    bool            CreateAtlas();

private:
    FT_Face         face;
    unsigned int    lineHeight;

    FontSymbols     symbols;
};

#endif // __FONT_H__