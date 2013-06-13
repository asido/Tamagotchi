#ifndef __TAMAGOTCHIENGINE_H__
#define __TAMAGOTCHIENGINE_H__

#ifdef _WIN32
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif // __WIN32

#include "common.h"

class IRenderer;

class TamagotchiEngine
{
public:
	TamagotchiEngine();
	virtual ~TamagotchiEngine();

	bool Init(GLint width, GLint height);

	void FrameUpdate(float delta);
	void FrameRender();

	std::shared_ptr<IRenderer> GetRenderer() { return this->renderer; }

private:
	GLuint LoadShader(GLenum type, const char *shaderSrc);

	GLint width;
	GLint height;

	GLint shaderProgram;

	std::shared_ptr<IRenderer> renderer;
};

#endif // __TAMAGOTCHIENGINE_H__