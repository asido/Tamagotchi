#include <cassert>
#include "TamagotchiEngine.h"
#include "ResourceManager/ZipFile.h"
#include "Rendering/Renderer.h"
#include "Rendering/ShaderManager.h"
#include "Logger.h"
#include "Clock.h"

TamagotchiEngine *g_engine = NULL;

//-----------------------------------------------------------------------------------------------------------
//  class TamagotchiEngine
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

TamagotchiEngine::TamagotchiEngine()
    : width(0), height(0),
      shaderProgram(0)
{
    g_engine = this;
}

TamagotchiEngine::~TamagotchiEngine()
{
    this->gameLogic->UnloadCurrentScene();
    g_engine = NULL;
}

bool TamagotchiEngine::Init(GLint width, GLint height)
{
    // If logger can't be initialised, just crash here right away.
    assert(LogMgr::Init());

    if (!Clock::Init())
    {
        LogError("Clock::Init() has failed.");
        return false;
    }

    this->engineConfig = std::shared_ptr<EngineConfig>(TG_NEW EngineConfig());
    if (!this->engineConfig->Init("EngineConfig.xml"))
    {
        LogError("Failed to initialize EngineConfig.");
        return false;
    }

    std::shared_ptr<IResourceFile> resourceFile = std::shared_ptr<IResourceFile>(TG_NEW ResourceZipFile(this->engineConfig->GetAssetFile()));
    this->resourceMgr = std::shared_ptr<ResourceManager>(TG_NEW ResourceManager(static_cast<unsigned int>(MB_TO_B(this->engineConfig->GetResourceCacheSize())), resourceFile));
    if (!this->resourceMgr->Init())
    {
        LogError("ResourceMgr::Init() has failed.");
        return false;
    }

    this->renderer = std::shared_ptr<IRenderer>(TG_NEW GLESRenderer(width, height));

    this->shaderMgr = std::shared_ptr<ShaderManager>(TG_NEW ShaderManager);

    this->gameLogic = CreateGameLogic();
    if (!this->gameLogic)
    {
        LogError("Failure creating game logic.");
        return false;
    }
    if (!this->gameLogic->Init())
    {
        LogError("GameLogic::Init() has failed.");
        return false;
    }

    this->gameLogic->AddGameView(this->CreateFirstView());

    this->gameLogic->LoadScene(this->GetEngineConfig()->GetFirstScene());

    return true;
}

void TamagotchiEngine::FrameUpdate(float delta)
{
    this->gameLogic->OnUpdate(delta);
}

void TamagotchiEngine::FrameRender()
{
    for (GameViewList::iterator it = this->gameLogic->gameViews.begin(); it != this->gameLogic->gameViews.end(); ++it)
    {
        std::shared_ptr<GameView> gameView = *it;
        gameView->OnRender();
    }
}

void TamagotchiEngine::HandleScreenEvent(const ScreenEvent &event)
{
    for (GameViewList::iterator it = this->gameLogic->gameViews.begin(), end = this->gameLogic->gameViews.end(); it != end; ++it)
    {
        std::shared_ptr<GameView> gameView = *it;
        gameView->HandleScreenEvent(event);
    }
}