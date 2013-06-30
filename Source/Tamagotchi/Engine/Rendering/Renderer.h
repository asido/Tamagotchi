#ifndef __Renderer_H__
#define __Renderer_H__

typedef struct {
    unsigned int width;
    unsigned int height;
} Resolution;

//-----------------------------------------------------------------------------------------------------------
//  class IRenderer
//-----------------------------------------------------------------------------------------------------------

class IRenderer
{
public:
    IRenderer(unsigned int surfaceW, unsigned int surfaceH);

    virtual void PreRender() = 0;
    virtual void PostRender() = 0;

    Resolution  GetResolution() const { return this->resolution; }
    float       GetAspectRatio() const { return this->aspectRatio; }

private:
    Resolution  resolution;
    float       aspectRatio;
};


//-----------------------------------------------------------------------------------------------------------
//  class GLESRenderer
//-----------------------------------------------------------------------------------------------------------

class GLESRenderer : public IRenderer
{
public:
    GLESRenderer(unsigned int surfaceW, unsigned int surfaceH);

    virtual void PreRender() override;
    virtual void PostRender() override;
};

#endif // __Renderer_H__