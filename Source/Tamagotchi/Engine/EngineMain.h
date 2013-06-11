#ifndef __ENGINEMAIN_H__
#define __ENGINEMAIN_H__

#ifdef _WIN32
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#endif // __WIN32

class TamagotchiEngine
{
public:
	TamagotchiEngine() {}
	virtual ~TamagotchiEngine() {}

	bool Init(GLint width, GLint height);

	void OnUpdate(float delta);
	void OnRender();

private:
	GLuint LoadShader(GLenum type, const char *shaderSrc);

	GLint width;
	GLint height;

	GLint shaderProgram;
};

#endif // __ENGINEMAIN_H__