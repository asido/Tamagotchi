#ifndef __TAMAGOTCHIENGINE_H__
#define __TAMAGOTCHIENGINE_H__

#include <memory>
#include "defines.h"
#include "GLES.h"

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