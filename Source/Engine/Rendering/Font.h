#ifndef __FONT_H__
#define __FONT_H__

#include <memory>
#include <map>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#include <Eigen/Dense>
using namespace Eigen;

#include "Rendering/GLES.h"

//-----------------------------------------------------------------------------------------------------------
//  class FontSymbol
//      Represents a single font glyph.
//-----------------------------------------------------------------------------------------------------------

class FontSymbol
{
    friend class FontManager;
    friend class Font;
    friend class GlyphTreeNode;
    
public:
    FontSymbol();
    FontSymbol(FT_Glyph glyph);

    void            RenderToBuffer(unsigned char *buffer, unsigned int textureWidth, unsigned int textureHeight);
    void            GlyphLoaded();

    unsigned int    GetWidth() const { return this->width; }
    unsigned int    GetHeight() const { return this->height; }

    unsigned int    GetPixelCount() const { return width * height; }
    bool            IsEmpty() const { return this->width == 0 || this->height == 0; }

private:
    void            Init(FT_Glyph glyph);

    void            InitTextureCoords(unsigned int textureWidth, unsigned int textureHeight);
    
    void            SetXY(unsigned int x, unsigned int y) { this->x = x; this->y = y; }
    void            SetSize(unsigned int width, unsigned int height) { this->width = width; this->height = height; }
    void            SetOffset(unsigned int xOffset, unsigned int yOffset) { this->xOffset = xOffset; this->yOffset = yOffset; }
    void            SetXAdvance(unsigned int xAdvance) { this->xAdvance = xAdvance; }

private:
    static const unsigned int VerticiesPerQuad  = 4;
    static const unsigned int IndicesPerQuad    = 6;

    Vector2f textureCoords[VerticiesPerQuad];

    FT_Glyph ftGlyph;

    unsigned int x, y;
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

    bool            RenderText(const std::string &text, unsigned int x, unsigned int y, unsigned int maxWidth);

    unsigned int    GetTextWidth(const std::string &text) const;
    unsigned int    GetLineHeight() const { return this->lineHeight; }

private:
    bool            AddSymbol(unsigned char symbol, std::shared_ptr<FontSymbol> fontChar);
    bool            CreateAtlas();

    // For details: http://www.blackpawn.com/texts/lightmaps/default.html
    bool            BinPack(unsigned int width, unsigned int height);

private:
    static const unsigned int MaxTextureDimension = 2048;

    FT_Face         face;
    unsigned int    lineHeight;

    FontSymbols     symbols;
    std::shared_ptr<FontSymbol> atlas;

    GLuint          glTexture;
};

#endif // __FONT_H__