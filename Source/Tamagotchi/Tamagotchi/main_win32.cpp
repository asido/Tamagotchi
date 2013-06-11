#include <windows.h>
#include "gles_init_win32.h"
#include "..\Engine\EngineMain.h"

TamagotchiEngine *g_engine;

void ESCALLBACK OnRender(ESContext *esContext)
{
	g_engine->OnRender();

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void ESCALLBACK OnUpdate(ESContext *esContext, float deltaTime)
{
	g_engine->OnUpdate(deltaTime);
}

void ESCALLBACK OnKey(ESContext *esContext, unsigned char wParam, int cursorX, int cursorY)
{
	OutputDebugString(L"OnKey\n");
}

INT WINAPI wWinMain(HINSTANCE	hInstance,
					HINSTANCE	hPrevInstance,
					LPWSTR		lpCmdLine,
					int			nCmdShow)
{
	ESContext es(1136, 640, L"Tamagotchi");
	es.Init();

	g_engine = new TamagotchiEngine();
	g_engine->Init(es.width, es.height);

	es.renderFunc	= OnRender;
	es.updateFunc	= OnUpdate;
	es.keyFunc		= OnKey;

	es.MainLoop();

	delete g_engine;

	return 0;
}