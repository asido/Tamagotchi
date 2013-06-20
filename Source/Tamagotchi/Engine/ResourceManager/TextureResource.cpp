#include <streambuf>
#include <istream>
#include "TextureResource.h"
#include "defines.h"
#include "Logger.h"

//-----------------------------------------------------------------------------------------------------------
//  class TextureResourceLoader
//-----------------------------------------------------------------------------------------------------------

GLESTextureResourceExtraData::GLESTextureResourceExtraData()
    : texture(0)
{

}

GLESTextureResourceExtraData::~GLESTextureResourceExtraData()
{
    if (this->texture)
    {
        glDeleteTextures(1, &this->texture);
        this->texture = 0;
    }
}


//-----------------------------------------------------------------------------------------------------------
//  class TextureResourceLoader
//-----------------------------------------------------------------------------------------------------------

struct StreamBuf : std::streambuf
{
    StreamBuf(char *begin, char *end)
    {
        this->setg(begin, begin, end);
    }
};

unsigned int TextureResourceLoader::GetLoadedResourceSize(char *rawBuffer, unsigned int rawSize)
{
    // Keep the resource cache from allocating memory for the texture, so OpenGL can manage it on it's own.
    // Once the data is loaded and pushed to OpenGL, ResourceHandle doesn't need to keep the data.
    return 0;
}

bool TextureResourceLoader::LoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<ResourceHandle> handle)
{
    enum {
        PNG_SIGNATURE_LENGTH = 8
    };

    png_structp     pngPtr;
    png_infop       infoPtr;
    unsigned int    pngWidth, pngHeight;
    unsigned int    bitDepth, channels;
    unsigned int    colorType;
    unsigned char   signature[PNG_SIGNATURE_LENGTH];

    png_bytep       *pngRowPointers = NULL;
    char            *decompressedBuffer = NULL;
    GLint            pngGlFormat;


    // First check the signature.
    memcpy(signature, rawBuffer, PNG_SIGNATURE_LENGTH);
    if (!png_check_sig(signature, PNG_SIGNATURE_LENGTH))
    {
        LogError("signature mismatch.");
        return false;
    }

    pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pngPtr)
    {
        LogError("png_create_read_struct() has failed.");
        return false;
    }

    infoPtr = png_create_info_struct(pngPtr);
    if (!infoPtr)
    {
        LogError("png_create_info_struct() has failed.");
        png_destroy_read_struct(&pngPtr, NULL, NULL);
        return false;
    }

    // Tell libpng that we already have checked the signature and so should skip it.
    png_set_sig_bytes(pngPtr, PNG_SIGNATURE_LENGTH);
    // We also do the same with data pointer before giving it to istream.
    rawBuffer += 8;

    StreamBuf sBuf(rawBuffer, rawBuffer + rawSize);
    std::istream dataStream(&sBuf);
    png_set_read_fn(pngPtr, (png_voidp)&dataStream, TextureResourceLoader::ReadPngData);

    if (setjmp(png_jmpbuf(pngPtr)))
    {
        LogError("Error reading png data.");
        png_destroy_read_struct(&pngPtr, &infoPtr, 0);
        SAFE_DELETE_ARRAY(decompressedBuffer);
        SAFE_DELETE_ARRAY(pngRowPointers);
        return false;
    }

    png_read_info(pngPtr, infoPtr);
    
    pngWidth    = png_get_image_width(pngPtr, infoPtr);
    pngHeight   = png_get_image_height(pngPtr, infoPtr);
    bitDepth    = png_get_bit_depth(pngPtr, infoPtr);
    channels    = png_get_channels(pngPtr, infoPtr);
    colorType   = png_get_color_type(pngPtr, infoPtr);

    switch (colorType) {
    case PNG_COLOR_TYPE_RGB:
        pngGlFormat = GL_RGB;
        break;

    case PNG_COLOR_TYPE_RGBA:
        pngGlFormat = GL_RGBA;
        break;

    case PNG_COLOR_TYPE_PALETTE:
        LogError("PNG_COLOR_TYPE_PALETTE is not supported.");
        return false;
        break;

    case PNG_COLOR_TYPE_GRAY:
        LogError("PNG_COLOR_TYPE_GRAY is not supported.");
        break;
    }

    // Prepare decompressed PNG buffer.
    pngRowPointers = TG_NEW png_bytep[pngHeight];
    decompressedBuffer = TG_NEW char[pngWidth * pngHeight * bitDepth * channels / 8];
    const unsigned int stride = pngWidth * bitDepth * channels / 8;
    for (unsigned int i = 0; i < pngHeight; ++i)
    {
        unsigned int q = (pngHeight - i - 1) * stride;
        pngRowPointers[i] = reinterpret_cast<png_bytep>(decompressedBuffer + q);
    }

    // Fill PNG buffer with with decompressed bytes.
    png_read_image(pngPtr, pngRowPointers);
    png_destroy_read_struct(&pngPtr, &infoPtr, NULL);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, pngGlFormat, pngWidth, pngHeight, 0, pngGlFormat, GL_UNSIGNED_BYTE, decompressedBuffer);

    GL_CHECK_ERROR();
    
    std::shared_ptr<GLESTextureResourceExtraData> extra = std::shared_ptr<GLESTextureResourceExtraData>(TG_NEW GLESTextureResourceExtraData());
    extra->texture = texture;
    handle->SetExtra(extra);

    delete[] decompressedBuffer;
    delete[] pngRowPointers;

    return true;
}


//-----------------------------------------------
// Private
//-----------------------------------------------

void TextureResourceLoader::ReadPngData(png_structp pngPtr, png_bytep data, png_size_t length)
{
    std::istream *dataStream = static_cast<std::istream*>(png_get_io_ptr(pngPtr));
    dataStream->read(reinterpret_cast<char*>(data), length);
}