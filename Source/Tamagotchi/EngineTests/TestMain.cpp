#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "Logger.h"

static EGLNativeWindowType hwnd;

const std::string InitGlStub()
{
#if defined(WIN32)
    EGLint numConfigs;
    EGLint majorVersion, minorVersion;
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLConfig   config;
    EGLSurface surface;
    EGLContext context;

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

    if (display == EGL_NO_DISPLAY)
    {
        return "display == EGL_NO_DISPLAY";
    }
    if (!eglInitialize(display, &majorVersion, &minorVersion))
    {
        return "eglInitialize() has failed.";
    }
    if (!eglGetConfigs(display, NULL, 0, &numConfigs))
    {
        return "eglGetConfigs() has failed.";
    }
    if (!eglChooseConfig(display, eglAttributes, &config, 1, &numConfigs))
    {
        return "eglChooseConfig() has failed.";
    }
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT)
    {
        return "eglCreateContext() has failed.";
    }

    hwnd = CreateWindowA("STATIC","dummy",WS_VISIBLE,0,0,100,100,NULL,NULL,NULL,NULL);
    surface = eglCreateWindowSurface(display, config, hwnd, NULL);
    if (surface == EGL_NO_SURFACE)
    {
        EGLint f=  eglGetError();
        return "eglCreateWindowSurface() has failed.";
    }

    if (!eglMakeCurrent(display, surface, surface, context))
    {
        return "eglMakeCurrent() has failed.";
    }

    return "";
#else
#   error Implement me
#endif
}

void DestroyGlStub()
{
#if defined(WIN32)
    DestroyWindow(hwnd);
#else
#   error Implement me
#endif
}

const std::string TestMain()
{
    // Init GL stub.
    const std::string status = InitGlStub();
    if (status.size() > 0)
    {
        LogError("GL stub creation has failed: %s", status.c_str());
        return status;
    }

    // Create the event manager and test controller.
    CPPUNIT_NS::TestResult controller;

    // Add a listener that collects test result.
    CPPUNIT_NS::TestResultCollector result;
    controller.addListener(&result);

    // Add a listener that print dots as test run.
    CPPUNIT_NS::BriefTestProgressListener progress;
    controller.addListener(&progress);

    // Add the top suite to the test runner.
    CPPUNIT_NS::TestRunner runner;
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(controller);

    // Destroy GL stub.
    DestroyGlStub();

    // Grab the output.
    std::stringbuf buffer;
    std::ostream os(&buffer);
    CPPUNIT_NS::CompilerOutputter outputter(&result, os);
    outputter.write();

    return buffer.str();
}