#ifndef __GLES_INIT_H__
#define __GLES_INIT_H__

#include <GLES2/gl2.h>
#include <egl/egl.h>
#include <string>

#define ESCALLBACK    __cdecl

class ESContext
{
public:
    ESContext(GLint width, GLint height, std::wstring title);
    virtual ~ESContext();
    bool Init();

    void MainLoop();
    void OnClose();

    // Callbacks
    void (ESCALLBACK *renderFunc)(ESContext *esContext);
    void (ESCALLBACK *keyFunc)(ESContext *esContext, unsigned int msg, unsigned char wParam, int cursorX, int cursorY);
    void (ESCALLBACK *updateFunc)(ESContext *esContext, float deltaTime);

    EGLNativeWindowType GetHwnd() const { return this->hWnd; }
    GLint               GetWidth() const { return this->width; }
    GLint               GetHeight() const { return this->height; }
    EGLDisplay          GetDisplay() const { return this->eglDisplay; }
    EGLSurface          GetSurface() const { return this->eglSurface; }

private:
    bool InitWindow();
    bool CreateEGLContext();

    void Print(std::string msg) const;

    // Window handle
    EGLNativeWindowType hWnd;

    EGLContext          eglContext;
    EGLDisplay          eglDisplay;
    EGLSurface          eglSurface;

    // Window width
    GLint               width;
    // Window height
    GLint               height;

    std::wstring        title;

    bool                quit;
};

#endif // __GLES_INIT_H__