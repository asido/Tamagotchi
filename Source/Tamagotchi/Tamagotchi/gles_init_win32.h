#ifndef __GLES_INIT_H__
#define __GLES_INIT_H__

#include <GLES2/gl2.h>
#include <egl/egl.h>
#include <string>

#define ESCALLBACK	__cdecl

class ESContext
{
public:
	ESContext(GLint width, GLint height, std::wstring title);
	virtual ~ESContext();
	bool Init();

	void MainLoop();
	void OnClose();

	// Window handle
	EGLNativeWindowType	hWnd;

	EGLDisplay		eglDisplay;
	EGLContext		eglContext;
	EGLSurface		eglSurface;

	// Window width
	GLint	width;
	// Window height
	GLint	height;

	// Callbacks
	void (ESCALLBACK *renderFunc)(ESContext *esContext);
	void (ESCALLBACK *keyFunc)(ESContext *esContext, unsigned char wParam, int cursorX, int cursorY);
	void (ESCALLBACK *updateFunc)(ESContext *esContext, float deltaTime);

private:
	bool InitWindow();
	bool CreateEGLContext();

	void Print(std::string msg) const;

	std::wstring	title;

	bool	quit;
};

#endif // __GLES_INIT_H__