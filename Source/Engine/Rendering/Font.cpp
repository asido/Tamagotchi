#include <algorithm>
#include <vector>
#include "Font.h"
#include "Logger.h"
#include "Allocator.h"

//-----------------------------------------------------------------------------------------------------------
//  class GlyphTreeNode
//      For details: http://www.blackpawn.com/texts/lightmaps/default.html
//-----------------------------------------------------------------------------------------------------------

class GlyphTreeNode
{
public:
    static Allocator<GlyphTreeNode>& GetAllocator();

    GlyphTreeNode();
    GlyphTreeNode(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

    void Set(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    bool Add(const std::shared_ptr<FontSymbol> &symbol);
    bool IsEmpty() const;

private:
    bool Fits(const std::shared_ptr<FontSymbol> &symbol) const;
    void CreateBranches(const std::shared_ptr<FontSymbol> &symbol);

private:
    unsigned int x, y;
    unsigned int width, height;

    // We use allocator to allocate the leafs, thus raw pointers.
    GlyphTreeNode *leafLeft;
    GlyphTreeNode *leafRight;
};

//-----------------------------------------------
// Static
//-----------------------------------------------

Allocator<GlyphTreeNode>& GlyphTreeNode::GetAllocator()
{
    static Allocator<GlyphTreeNode> allocator;
    return allocator;
}

//-----------------------------------------------
// Public
//-----------------------------------------------

GlyphTreeNode::GlyphTreeNode()
    : x(0), y(0), width(0), height(0), leafLeft(NULL), leafRight(NULL)
{

}

GlyphTreeNode::GlyphTreeNode(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    : x(x), y(y), width(width), height(height), leafLeft(NULL), leafRight(NULL)
{

}

void GlyphTreeNode::Set(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    this->x         = x;
    this->y         = y;
    this->width     = width;
    this->height    = height;
}

bool GlyphTreeNode::Add(const std::shared_ptr<FontSymbol> &symbol)
{
    if (symbol->IsEmpty())
    {
        return true;
    }

    bool result = false;

    if (IsEmpty())
    {
        if (Fits(symbol))
        {
            CreateBranches(symbol);
            symbol->SetXY(this->x, this->y);
            result = true;
        }
    }
    else
    {
        if (this->leafLeft->Add(symbol))
        {
            result = true;
        }
        else if (this->leafRight->Add(symbol))
        {
            result = true;
        }
    }

    return result;
}

bool GlyphTreeNode::IsEmpty() const
{
    return this->leafLeft == NULL && this->leafRight == NULL;
}

//-----------------------------------------------
// Private
//-----------------------------------------------

bool GlyphTreeNode::Fits(const std::shared_ptr<FontSymbol> &symbol) const
{
    return symbol->GetWidth() <= this->width && symbol->GetHeight() <= this->height;
}

void GlyphTreeNode::CreateBranches(const std::shared_ptr<FontSymbol> &symbol)
{
    unsigned int dw = this->width - symbol->GetWidth();
    unsigned int dh = this->height - symbol->GetHeight();

    if (dw < dh)
    {
        this->leafLeft = GetAllocator().New();
        this->leafLeft->Set(this->x + symbol->GetWidth(), this->y,
                            this->width - symbol->GetWidth(), this->height);

        this->leafRight = GetAllocator().New();
        this->leafRight->Set(this->x, this->y + symbol->GetHeight(),
                             this->width, this->height - symbol->GetHeight());
    }
    else
    {
        this->leafLeft = GetAllocator().New();
        this->leafLeft->Set(this->x, this->y + symbol->GetHeight(),
                            this->width, this->height - symbol->GetHeight());

        this->leafRight = GetAllocator().New();
        this->leafRight->Set(this->x + symbol->GetWidth(), this->y,
                             this->width - symbol->GetWidth(), this->height);
    }
}


//-----------------------------------------------------------------------------------------------------------
//  class FontSymbol
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

FontSymbol::FontSymbol()
{
    Init(NULL);
}

FontSymbol::FontSymbol(FT_Glyph glyph)
{
    Init(glyph);
}

void FontSymbol::RenderToBuffer(unsigned char *buffer, unsigned int textureWidth, unsigned int textureHeight)
{
    if (IsEmpty())
    {
        return;
    }

    InitTextureCoords(textureWidth, textureHeight);

    FT_Glyph_To_Bitmap(&this->ftGlyph, FT_RENDER_MODE_NORMAL, NULL, true);
    FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(this->ftGlyph);

    LogAssert(bitmapGlyph->bitmap.width == this->width && "Dimensions don't match.");
    LogAssert(bitmapGlyph->bitmap.rows == this->height && "Dimensions don't match.");

    for (int y = 0; y < bitmapGlyph->bitmap.rows; ++y)
    {
        for (int x = 0; x < bitmapGlyph->bitmap.width; ++x)
        {
            int index = ((this->y + y) * textureWidth) + (this->x + x);
            LogAssert(index >= 0 && "Invalid index.");
            buffer[index] = bitmapGlyph->bitmap.buffer[y * bitmapGlyph->bitmap.width + x];
        }
    }
}

void FontSymbol::GlyphLoaded()
{
    if (this->ftGlyph)
    {
        FT_Done_Glyph(this->ftGlyph);
    }
}

//-----------------------------------------------
// Private
//-----------------------------------------------

void FontSymbol::Init(FT_Glyph glyph)
{
    this->ftGlyph   = glyph;
    this->x         = 0;
    this->y         = 0;
    this->width     = 0;
    this->height    = 0;
    this->xOffset   = 0;
    this->yOffset   = 0;
    this->xAdvance  = 0;
}

void FontSymbol::InitTextureCoords(unsigned int textureWidth, unsigned int textureHeight)
{
    float x1 = static_cast<float>(this->x) / static_cast<float>(textureWidth);
    float y1 = static_cast<float>(this->y) / static_cast<float>(textureHeight);

    float x2 = static_cast<float>(this->x + this->width) / static_cast<float>(textureWidth);
    float y2 = static_cast<float>(this->y + this->height) / static_cast<float>(textureHeight);

    this->textureCoords[0](0) = x1;
    this->textureCoords[0](1) = y1;

    this->textureCoords[1](0) = x2;
    this->textureCoords[1](1) = y1;

    this->textureCoords[2](0) = x1;
    this->textureCoords[2](1) = y2;

    this->textureCoords[3](0) = x2;
    this->textureCoords[3](1) = y2;
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

bool Font::RenderText(const std::string &text, unsigned int x, unsigned int y, unsigned int maxWidth)
{
    LogError("Implement me!");
    return false;
}


//-----------------------------------------------
// Private
//-----------------------------------------------

bool Font::AddSymbol(unsigned char symbol, std::shared_ptr<FontSymbol> fontChar)
{
    FontSymbols::iterator it = this->symbols.find(symbol);
    if (it != this->symbols.end())
    {
        LogWarning("Duplicate symbol '%c'.", symbol);
        return false;
    }

    std::pair<FontSymbols::iterator, bool> status = this->symbols.insert(std::make_pair(symbol, fontChar));
    return status.second;
}

// Used for stl::sort
bool CompareForLarger(std::shared_ptr<FontSymbol> font1, std::shared_ptr<FontSymbol> font2)
{
    return font1->GetPixelCount() < font2->GetPixelCount();
}

bool Font::CreateAtlas()
{
    unsigned int totalPixels = 0;
    for (FontSymbols::const_iterator it = this->symbols.cbegin(), end = this->symbols.cend(); it != end; ++it)
    {
        std::shared_ptr<FontSymbol> glyph = it->second;
        totalPixels += glyph->GetPixelCount();
    }

    unsigned int textureWidth   = 1;
    unsigned int textureHeight  = 1;
    while (textureWidth * textureHeight < totalPixels)
    {
        if (textureWidth < textureHeight)
        {
            textureWidth *= 2;
        }
        else
        {
            textureHeight *= 2;
        }

        if (textureWidth > MaxTextureDimension || textureHeight > MaxTextureDimension)
        {
            LogError("Font atlas became too large.");
            return false;
        }
    }

    std::vector<std::shared_ptr<FontSymbol>> sortedSymbols;
    for (FontSymbols::const_iterator it = this->symbols.cbegin(), end = this->symbols.cend(); it != end; ++it)
    {
        sortedSymbols.push_back(it->second);
    }
    std::sort(sortedSymbols.begin(), sortedSymbols.end(),
        [](std::shared_ptr<FontSymbol> &glyph1, std::shared_ptr<FontSymbol> &glyph2) -> bool {
            return glyph1->GetPixelCount() > glyph2->GetPixelCount();
        }
    );
    
    GlyphTreeNode::GetAllocator().Init((this->symbols.size() + 1) * 2);
    if (!BinPack(textureWidth, textureHeight))
    {
        // TODO: We shouldn't fail like this because binary packing the glyphs might fail if for example
        // texture is not big enough to fit all glyphs because of too many unused gaps. In such case we should
        // continue looping and increase textureWidth & textureHeight until BinPack succeeds.
        GlyphTreeNode::GetAllocator().Release();
        LogError("Font::BinPack has failed.");
        return false;
    }
    GlyphTreeNode::GetAllocator().Release();

    unsigned char *textureBuffer = new unsigned char[textureWidth * textureHeight];

    for (FontSymbols::const_iterator it = this->symbols.cbegin(), end = this->symbols.cend(); it != end; ++it)
    {
        std::shared_ptr<FontSymbol> symbol = it->second;
        symbol->RenderToBuffer(textureBuffer, textureWidth, textureHeight);
        symbol->GlyphLoaded();
    }
    
    glGenTextures(1, &this->glTexture);
    glBindTexture(GL_TEXTURE_2D, this->glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, textureWidth, textureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, textureBuffer);

    GL_CHECK_ERROR();

    delete[] textureBuffer;

    this->atlas = std::shared_ptr<FontSymbol>(TG_NEW FontSymbol);
    this->atlas->SetXY(0, 0);
    this->atlas->SetSize(textureWidth, textureHeight);
    this->atlas->InitTextureCoords(textureWidth, textureHeight);

    return true;
}

bool Font::BinPack(unsigned int width, unsigned int height)
{
    GlyphTreeNode *rootNode = GlyphTreeNode::GetAllocator().New();
    rootNode->Set(0, 0, width, height);

    for (FontSymbols::iterator it = this->symbols.begin(), end = this->symbols.end(); it != end; ++it)
    {
        std::shared_ptr<FontSymbol> symbol = it->second;
        if (!rootNode->Add(symbol))
        {
            return false;
        }
    }

    return true;
}