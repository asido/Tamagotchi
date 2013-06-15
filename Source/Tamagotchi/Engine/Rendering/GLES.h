#ifndef __GLES_H__
#define __GLES_H__

#ifdef WIN32
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif // WIN32

#if defined DEBUG || defined _DEBUG
#    define GL_CHECK_ERROR()                        \
    do {                                            \
            GLenum error = glGetError();            \
            if (error != GL_NO_ERROR)               \
            {                                       \
                LogError("GL Error: 0x%X", error);  \
            }                                       \
        } while (0)
#else
#    define GL_CHECK_ERROR()
#endif // DEBUG

#endif // __GLES_H__