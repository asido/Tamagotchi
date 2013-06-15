#include <windows.h>
#include "gles_init_win32.h"
#include "../Engine/TamagotchiEngine.h"

static TamagotchiEngine *engine;

void ESCALLBACK OnRender(ESContext *esContext)
{
    engine->FrameRender();

    eglSwapBuffers(esContext->GetDisplay(), esContext->GetSurface());
}

void ESCALLBACK OnUpdate(ESContext *esContext, float deltaTime)
{
    engine->FrameUpdate(deltaTime);
}

void ESCALLBACK OnKey(ESContext *esContext, unsigned char wParam, int cursorX, int cursorY)
{
    // TODO: need to translate win32 input messages to input controller independent events
    // which game engine can understand.
}

INT WINAPI wWinMain(_In_        HINSTANCE   hInstance,
                    _In_opt_    HINSTANCE   hPrevInstance,
                    _In_        LPWSTR      lpCmdLine,
                    _In_        int         nCmdShow)
{
    // Setup checks for memory leaks.
    int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    
    // Perform memory checks for each alloc/dealloc. This is VERY VERY SLOW and so use only when necessary.
    //tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;

    // Check for leaks just before program exit.
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;

    _CrtSetDbgFlag(tmpDbgFlag);

    ESContext es(1136, 640, L"Tamagotchi");
    es.Init();

    engine = TG_NEW TamagotchiEngine();

    if (engine->Init(es.GetWidth(), es.GetHeight()))
    {
        es.renderFunc   = OnRender;
        es.updateFunc   = OnUpdate;
        es.keyFunc      = OnKey;

        es.MainLoop();
    }
    
    delete engine;

    return 0;
}