#include <windows.h>
#include "gles_init_win32.h"

void ESCALLBACK Render(ESContext *esContext)
{
	// TODO: Call game instance renderer.

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

INT WINAPI wWinMain(HINSTANCE	hInstance,
					HINSTANCE	hPrevInstance,
					LPWSTR		lpCmdLine,
					int			nCmdShow)
{
	ESContext es(1136, 640, L"Tamagotchi");
	es.Init();
	es.renderFunc = Render;

	return 0;
}