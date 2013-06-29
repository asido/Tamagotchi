#include "Renderer.h"
#include "GLES.h"

void GLESRenderer::PreRender()
{
    // TODO: use dynamic values based on screen resolution.
    glViewport(0, 0, 1136, 640);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GL_CHECK_ERROR();
}

void GLESRenderer::PostRender()
{

}