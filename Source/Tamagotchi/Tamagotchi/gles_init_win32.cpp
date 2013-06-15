#include "gles_init_win32.h"

//-----------------------------------------------------------------------------------------------------------
//  Win32 window message processing
//-----------------------------------------------------------------------------------------------------------
LRESULT WINAPI ESWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT ret = 1;

    switch (msg)
    {
    case WM_CREATE:
        break;

    case WM_PAINT: {
        ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

        if (esContext)
        {
            if (esContext->renderFunc)
            {
                esContext->renderFunc(esContext);
            }

            ValidateRect(esContext->GetHwnd(), NULL);
        }
        
        break;
    }

    case WM_CLOSE: {
        ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

        if (esContext)
        {
            esContext->OnClose();
        }
        break;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_CHAR:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP: {
        POINT point;
        ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

        if (esContext && esContext->keyFunc)
        {
            GetCursorPos(&point);
            esContext->keyFunc(esContext, (unsigned char) wParam, (int) point.x, (int)point.y);
        }
        break;
    }

    default:
        ret = DefWindowProc(hWnd, msg, wParam, lParam);
        break;
    }

    return ret;
}
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
// Public
//-----------------------------------------------------------------------------------------------------------

ESContext::ESContext(GLint width, GLint height, std::wstring title)
{
    this->width = width;
    this->height = height;
    this->title = title;
    this->quit = false;
}

ESContext::~ESContext()
{

}

bool ESContext::Init()
{
    if (!this->InitWindow())
    {
        this->Print("Window initialization has failed.");
        return false;
    }

    if (!this->CreateEGLContext())
    {
        this->Print("EGL context initialization has failed");
        return false;
    }

    return true;
}

void ESContext::MainLoop()
{
    MSG            msg = { 0 };
    ULONGLONG    lastTime = GetTickCount64();

    while (!this->quit)
    {
        int         gotMsg      = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE));
        ULONGLONG   curTime     = GetTickCount64();
        float       deltaTime   = (float)(curTime - lastTime) / 1000.0f;

        lastTime = curTime;

        if (gotMsg)
        {
            if (msg.message == WM_QUIT)
            {
                this->quit = true;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            SendMessage(this->hWnd, WM_PAINT, 0, 0);
        }

        if (this->updateFunc != NULL)
        {
            this->updateFunc(this, deltaTime);
        }
    }
}

void ESContext::OnClose()
{
    this->quit = true;
}

//-----------------------------------------------------------------------------------------------------------
// Private
//-----------------------------------------------------------------------------------------------------------

bool ESContext::InitWindow()
{
    WNDCLASS    wndclass = { 0 };
    DWORD       style = 0;
    RECT        windowRect;
    HINSTANCE   hInstance = GetModuleHandle(NULL);

    wndclass.style          = CS_OWNDC;
    wndclass.lpfnWndProc    = (WNDPROC)ESWindowProc;
    wndclass.hInstance      = hInstance;
    wndclass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszClassName  = this->title.c_str();

    if (!RegisterClass(&wndclass))
    {
        this->Print("RegisterClass() has failed.");
        return false;
    }

    style = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

    windowRect.left = 0;
    windowRect.top = 0;
    windowRect.right = this->width;
    windowRect.bottom = this->height;
    AdjustWindowRect(&windowRect, style, false);

    this->hWnd = CreateWindow(this->title.c_str(),
                              this->title.c_str(),
                              style,
                              0,
                              0,
                              windowRect.right - windowRect.left,
                              windowRect.bottom - windowRect.top,
                              NULL,
                              NULL,
                              hInstance,
                              NULL);

    if (this->hWnd == NULL)
    {
        this->Print("CreateWindow() has failed.");
        return false;
    }

    // Set the ESContext* to the GWL_USERDATA so that it is available to the ESWindowProc.
    SetWindowLongPtr(this->hWnd, GWL_USERDATA, (LONG)(LONG_PTR)this);

    ShowWindow(this->hWnd, true);

    return true;
}

bool ESContext::CreateEGLContext()
{
    EGLint      numConfigs;
    EGLint      majorVersion;
    EGLint      minorVersion;
    EGLDisplay  display;
    EGLContext  context;
    EGLSurface  surface;
    EGLConfig   config;

    EGLint eglAttributes[] = {
        EGL_RED_SIZE,       8,
        EGL_GREEN_SIZE,     8,
        EGL_BLUE_SIZE,      8,
        EGL_ALPHA_SIZE,     8,
        EGL_DEPTH_SIZE,     8,
        EGL_STENCIL_SIZE,   8,
        EGL_SAMPLE_BUFFERS, 1,
        EGL_NONE
    };
    EGLint  contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2,
                                 EGL_NONE, EGL_NONE };

    display = eglGetDisplay(GetDC(this->hWnd));
    if (display == EGL_NO_DISPLAY)
    {
        this->Print("eglGetDisplay() has failed.");
        return false;
    }

    if (!eglInitialize(display, &majorVersion, &minorVersion))
    {
        this->Print("eglInitialize() has failed.");
        return false;
    }

    if (!eglGetConfigs(display, NULL, 0, &numConfigs))
    {
        this->Print("eglGetConfigs() has failed.");
        return false;
    }

    if (!eglChooseConfig(display, eglAttributes, &config, 1, &numConfigs))
    {
        this->Print("eglChooseConfig() has failed.");
        return false;
    }

    surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)this->hWnd, NULL);
    if (surface == EGL_NO_SURFACE)
    {
        this->Print("eglCreateWindowSurface() has failed");
        return false;
    }

    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT)
    {
        this->Print("eglCreateContext() has failed.");
        return false;
    }

    if (!eglMakeCurrent(display, surface, surface, context))
    {
        this->Print("eglMakeCurrent() has failed.");
        return false;
    }

    this->eglDisplay = display;
    this->eglSurface = surface;
    this->eglContext = context;

    return true;
}

void ESContext::Print(std::string msg) const
{
    fprintf(stderr, "[ESConext]: %s", msg.c_str());
}