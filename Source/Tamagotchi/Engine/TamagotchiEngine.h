#ifndef __TAMAGOTCHIENGINE_H__
#define __TAMAGOTCHIENGINE_H__

#include <memory>
#include "defines.h"
#include "Rendering/GLES.h"
#include "GameLogic.h"
#include "UI/GameView.h"

class ResourceManager;
class IRenderer;
class ShaderManager;

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
    std::shared_ptr<ShaderManager>      GetShadermanager() const { return this->shaderMgr; }
    std::shared_ptr<GameLogic>          GetGameLogic() const { return this->gameLogic; }

protected:
    // Game specific initialization which must be defined by subclass.
    virtual std::shared_ptr<GameLogic>  CreateGameLogic() = 0;
    virtual std::shared_ptr<GameView>   CreateFirstView() = 0;

private:
    GLuint LoadShader(GLenum type, const char *shaderSrc);

    GLint width;
    GLint height;

    GLint shaderProgram;

    std::shared_ptr<ResourceManager>    resourceMgr;
    std::shared_ptr<IRenderer>          renderer;
    std::shared_ptr<ShaderManager>      shaderMgr;
    std::shared_ptr<GameLogic>          gameLogic;
};

extern TamagotchiEngine *g_engine;

#endif // __TAMAGOTCHIENGINE_H__