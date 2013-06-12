#include "..\Engine\EngineStd.h"

#include <windows.h>
#include "gles_init_win32.h"
#include "..\Engine\EngineMain.h"

TamagotchiEngine *g_engine;

void ESCALLBACK OnRender(ESContext *esContext)
{
	g_engine->FrameRender();

	eglSwapBuffers(esContext->GetDisplay(), esContext->GetSurface());
}

void ESCALLBACK OnUpdate(ESContext *esContext, float deltaTime)
{
	g_engine->FrameUpdate(deltaTime);
}

void ESCALLBACK OnKey(ESContext *esContext, unsigned char wParam, int cursorX, int cursorY)
{
	OutputDebugString(L"OnKey\n");
}

INT WINAPI wWinMain(_In_		HINSTANCE	hInstance,
					_In_opt_	HINSTANCE	hPrevInstance,
					_In_		LPWSTR		lpCmdLine,
					_In_		int			nCmdShow)
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

	g_engine = new TamagotchiEngine();

	if (g_engine->Init(es.GetWidth(), es.GetHeight()))
	{
		es.renderFunc	= OnRender;
		es.updateFunc	= OnUpdate;
		es.keyFunc		= OnKey;

		es.MainLoop();
	}
	
	delete g_engine;

	return 0;
}