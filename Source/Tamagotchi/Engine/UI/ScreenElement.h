#ifndef __SCREENELEMENT_H__
#define __SCREENELEMENT_H__

#include <memory>
#include <string>

#include <tinyxml2.h>
#include <Eigen/Dense>
using namespace Eigen;

#include "StringUtilities.h"
#include "Rendering/Shader.h"

typedef unsigned int ScreenElementId;
const ScreenElementId INVALID_SCREEN_ELEMENT_ID = 0;

class Resource;
class TransformComponent;

//-----------------------------------------------------------------------------------------------------------
//  class ScreenElement
//-----------------------------------------------------------------------------------------------------------

class ScreenElement
{
#if !defined(DEBUG) && !defined(_DEBUG)
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
#endif

public:
    static ScreenElementId GetIdFromName(const std::string &name) { return StringUtilities::Hash(name); }

    ScreenElement();

    virtual bool    Init(tinyxml2::XMLElement *data);

    virtual void    OnUpdate(float delta) = 0;
    virtual void    OnRender();

    bool            IsVisible() const { return this->visible; }

protected:
    Matrix4f        CalculateMVP();

protected:
    static const DefaultVertexData  ElementVerticies[];
    static const unsigned int       VertexCount;

    std::shared_ptr<Shader>     shader;
    std::shared_ptr<Resource>   textureResource;

    Matrix4f                    projectionMatrix;
    std::shared_ptr<TransformComponent> transform;

    bool    visible;

    GLuint  glVertexArray;
    GLuint  glBuffer;
};


//-----------------------------------------------------------------------------------------------------------
//  class ButtonElement
//-----------------------------------------------------------------------------------------------------------

class ButtonElement : public ScreenElement
{
public:
    static ScreenElementId GetIdStatic();

    virtual bool    Init(tinyxml2::XMLElement *data) override;

    virtual void    OnUpdate(float delta) override;
    virtual void    OnRender() override;

private:
    static const std::string    name;

    std::string text;
};

#endif // __SCREENELEMENT_H__