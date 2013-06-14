#include "TamagotchiEngine.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/ZipFile.h"
#include "Rendering/Renderer.h"
#include "Logger.h"

//-----------------------------------------------------------------------------------------------------------
// Public
//-----------------------------------------------------------------------------------------------------------

TamagotchiEngine::TamagotchiEngine()
{
	
}

TamagotchiEngine::~TamagotchiEngine()
{
	LogMgr::Destroy();
}

bool TamagotchiEngine::Init(GLint width, GLint height)
{
	LogMgr::Init();

	// TODO: we should create TamagotchiOptions class which will parse the .xml config file
	// and get all necessary information, like asset zip file name, ResourceManager cache size and such.
	std::string assetFile = "Assets.zip";

	std::shared_ptr<IResourceFile> resourceFile = std::shared_ptr<IResourceFile>(TG_NEW ResourceZipFile(assetFile));
	this->resourceMgr = std::shared_ptr<ResourceManager>(TG_NEW ResourceManager(static_cast<unsigned int>(MB_TO_B(50.0f)), resourceFile));
	if (!this->resourceMgr->Init())
	{
		LogError("ResourceMgr::Init() has failed.");
		return false;
	}

	this->width = width;
	this->height = height;
	LogInfo("Using resolution: %dx%d", this->width, this->height);

	this->renderer = std::shared_ptr<IRenderer>(TG_NEW GLESRenderer());


	/*
	 * All the following crap is just to render a test triangle.
	 * It will go away once we have renderer ready to draw our objects.
	 */

	const char vShaderStr[] =  
		"attribute vec4 vPosition;    \n"
		"void main()                  \n"
		"{                            \n"
		"   gl_Position = vPosition;  \n"
		"}                            \n";

	const char fShaderStr[] =  
		"precision mediump float;\n"\
		"void main()                                  \n"
		"{                                            \n"
		"  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
		"}                                            \n";

	GLuint	vertexShader;
	GLuint	fragmentShader;
	GLuint	programObject;
	GLint	linked;

	vertexShader	= LoadShader(GL_VERTEX_SHADER, vShaderStr);
	fragmentShader	= LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

	programObject = glCreateProgram();
	if (!programObject)
	{
		return 0;
	}

	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	glBindAttribLocation(programObject, 0, "vPosition");
	
	glLinkProgram(programObject);

	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		//OutputDebugString(L"Shader linking has failed.");
		glDeleteProgram(programObject);
		return false;
	}

	this->shaderProgram = programObject;
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	return true;
}

void TamagotchiEngine::FrameUpdate(float delta)
{

}

void TamagotchiEngine::FrameRender()
{
	GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f, 
							-0.5f, -0.5f, 0.0f,
							 0.5f, -0.5f, 0.0f };
	glViewport(0, 0, this->width, this->height);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(this->shaderProgram);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

//-----------------------------------------------------------------------------------------------------------
// Private
//-----------------------------------------------------------------------------------------------------------

GLuint TamagotchiEngine::LoadShader(GLenum type, const char *shaderSrc)
{
	GLuint	shader;
	GLint	compiled;

	shader = glCreateShader(type);

	if (!shader)
	{
		return 0;
	}

	glShaderSource(shader, 1, &shaderSrc, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char *infoLog = (char*)malloc(sizeof(char) * infoLen);
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			//OutputDebugString(L"Failed to compile shader");
			free(infoLog);
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}