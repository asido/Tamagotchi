#ifndef __Renderer_H__
#define __Renderer_H__

#include "common.h"

//-----------------------------------------------------------------------------------------------------------
//  class IRenderer
//-----------------------------------------------------------------------------------------------------------

class IRenderer
{
public:
	virtual void PreRender() = 0;
	virtual void PostRender() = 0;
};


//-----------------------------------------------------------------------------------------------------------
//  class GLESRenderer
//-----------------------------------------------------------------------------------------------------------

class GLESRenderer : public IRenderer
{
public:
	virtual void PreRender() override;
	virtual void PostRender() override;
};

#endif // __Renderer_H__