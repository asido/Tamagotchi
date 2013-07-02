#include <windows.h>
#include <stdio.h>
#include "gles_init_win32.h"
#include "TamagotchiGame.h"

static const unsigned int surfaceWidth  = 1136;
static const unsigned int surfaceHeight = 640;
static Tamagotchi *game;

void ESCALLBACK OnRender(ESContext *esContext)
{
    game->FrameRender();

    eglSwapBuffers(esContext->GetDisplay(), esContext->GetSurface());
}

void ESCALLBACK OnUpdate(ESContext *esContext, float deltaTime)
{
    game->FrameUpdate(deltaTime);
}

void ESCALLBACK OnKey(ESContext *esContext, unsigned int msg, unsigned char wParam, int cursorX, int cursorY)
{
    // Engine treats bottom-left corner as (0,0) coordinates, while in windows GUI top-left is (0,0).
    cursorY = surfaceHeight - cursorY;

    switch (msg) {
    case WM_LBUTTONDOWN: {
        std::shared_ptr<const Point> point = std::shared_ptr<const Point>(TG_NEW Point(static_cast<float>(cursorX), static_cast<float>(cursorY)));
        ScreenEvent event(ScreenEvent::EVENT_TYPE_TOUCH, point);
        game->HandleScreenEvent(event);
        break;
    }

    case WM_LBUTTONUP: {
        std::shared_ptr<const Point> point(TG_NEW Point(static_cast<float>(cursorX), static_cast<float>(cursorY)));
        ScreenEvent event(ScreenEvent::EVENT_TYPE_RELEASE, point);
        game->HandleScreenEvent(event);
        break;
    }
    }
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

    ESContext es(surfaceWidth, surfaceHeight, L"Tamagotchi");
    es.Init();

    game = TG_NEW Tamagotchi;

    if (game->Init(es.GetWidth(), es.GetHeight()))
    {
        es.renderFunc   = OnRender;
        es.updateFunc   = OnUpdate;
        es.keyFunc      = OnKey;

        es.MainLoop();
    }

    // We have to delete the game object in order to trigger all game object deletion.
    // Otherwise default destruction will happen in wrong order and internal asserts will fail.
    delete game;

    return 0;
}