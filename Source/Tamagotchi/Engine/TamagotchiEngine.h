#ifndef __TAMAGOTCHIENGINE_H__
#define __TAMAGOTCHIENGINE_H__

#include <memory>
#include "defines.h"
#include "Rendering/GLES.h"

class ResourceManager;
class IRenderer;

class TamagotchiEngine
{
public:
    TamagotchiEngine();
    virtual ~TamagotchiEngine();

    bool Init(GLint width, GLint height);

    void FrameUpdate(float delta);
    void FrameRender();

    std::shared_ptr<ResourceManager>    GetResourceManager() const { return this->resourceMgr; }
    std::shared_ptr<IRenderer>          GetRenderer() const { return this->renderer; }

private:
    GLuint LoadShader(GLenum type, const char *shaderSrc);

    GLint width;
    GLint height;

    GLint shaderProgram;

    std::shared_ptr<ResourceManager>    resourceMgr;
    std::shared_ptr<IRenderer>          renderer;
};

extern TamagotchiEngine *g_engine;

#endif // __TAMAGOTCHIENGINE_H__