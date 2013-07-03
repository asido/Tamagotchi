#include "Font.h"
#include "Logger.h"

//-----------------------------------------------------------------------------------------------------------
//  class FontSymbol
//-----------------------------------------------------------------------------------------------------------

FontSymbol::FontSymbol(FT_Glyph glyph)
    : ftGlyph(glyph)
{

}


//-----------------------------------------------------------------------------------------------------------
//  class Font
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

Font::Font(FT_Face fontFace)
    : face(fontFace)
{
    // All metric dimensions are multiplied by 64, so we have to divide.
    this->lineHeight = face->size->metrics.height / 64;
}

bool Font::AddSymbol(unsigned char symbol, std::shared_ptr<FontSymbol> fontChar)
{
    FontSymbols::iterator it = this->symbols.find(symbol);
    if (it != this->symbols.end())
    {
        LogError("Duplicate symbol '%c'.", symbol);
        return false;
    }

    std::pair<FontSymbols::iterator, bool> status = this->symbols.insert(std::make_pair(symbol, fontChar));
    return status.second;
}

//-----------------------------------------------
// Private
//-----------------------------------------------

bool Font::CreateAtlas()
{
    LogError("Implement me!");
    return false;
}