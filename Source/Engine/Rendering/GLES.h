#ifndef __GLES_H__
#define __GLES_H__

#ifdef WIN32
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif // WIN32

#include "Logger.h"

/*
static const char *GlEnumStrings[] = {
    [GL_NO_ERROR] = "GL_NO_ERROR",
    [GL_INVALID_ENUM] = "GL_INVALID_ENUM"
};
*/

static inline const std::string GlErrorToString(GLenum error)
{
    switch (error) {
    case GL_NO_ERROR:           return "GL_NO_ERROR";
    case GL_INVALID_ENUM:       return "GL_INVALID_ENUM";
    case GL_INVALID_OPERATION:  return "GL_INVALID_OPERATION";
    case GL_INVALID_VALUE:      return "GL_INVALID_VALUE";
    case GL_STACK_OVERFLOW:     return "GL_STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW:    return "GL_STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY:      return "GL_OUT_OF_MEMORY";
    default:                    return "unknown error";
    }
}

#if defined(DEBUG)
#    define GL_CHECK_ERROR()                                                                        \
    do {                                                                                            \
        /* Check for up to 10 pending errors. */                                                    \
        for (int i = 0; i < 10; ++i)                                                                \
        {                                                                                           \
            GLenum error = glGetError();                                                            \
            if (error == GL_NO_ERROR)                                                               \
            {                                                                                       \
                break;                                                                              \
            }                                                                                       \
            else                                                                                    \
            {                                                                                       \
                LogWarning("GL Error(%d): 0x%X (%s)", i, error, GlErrorToString(error).c_str());    \
            }                                                                                       \
        }                                                                                           \
    } while (0)
#else
#    define GL_CHECK_ERROR()
#endif // DEBUG

#endif // __GLES_H__