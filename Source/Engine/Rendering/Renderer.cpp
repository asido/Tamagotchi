#include "Renderer.h"
#include "GLES.h"

//-----------------------------------------------------------------------------------------------------------
//  class IRenderer
//-----------------------------------------------------------------------------------------------------------

IRenderer::IRenderer(unsigned int surfaceW, unsigned int surfaceH)
{
    this->resolution.width = surfaceW;
    this->resolution.height = surfaceH;
    this->aspectRatio = static_cast<float>(this->resolution.width) / static_cast<float>(this->resolution.height);

    LogInfo("Using resolution: %dx%d", this->resolution.width, this->resolution.height);
    LogInfo("Using aspect ratio: %0.2f", this->aspectRatio);
}


//-----------------------------------------------------------------------------------------------------------
//  class GLESRenderer
//-----------------------------------------------------------------------------------------------------------

GLESRenderer::GLESRenderer(unsigned int surfaceW, unsigned int surfaceH)
    : IRenderer(surfaceW, surfaceH)
{

}

void GLESRenderer::PreRender()
{
    // TODO: use dynamic values based on screen resolution.
    glViewport(0, 0, this->GetResolution().width, this->GetResolution().height);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GL_CHECK_ERROR();
}

void GLESRenderer::PostRender()
{

}