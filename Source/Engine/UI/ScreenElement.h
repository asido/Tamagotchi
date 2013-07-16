#ifndef __SCREENELEMENT_H__
#define __SCREENELEMENT_H__

#include <memory>
#include <string>

#include <tinyxml2.h>
#include <Eigen/Dense>
using namespace Eigen;

#include "StringUtilities.h"
#include "Rendering/Shader.h"
#include "Geometry.h"

typedef unsigned int ScreenElementId;
const ScreenElementId INVALID_SCREEN_ELEMENT_ID = 0;

class Resource;
class TransformComponent;

//-----------------------------------------------------------------------------------------------------------
//  class ScreenEvent
//-----------------------------------------------------------------------------------------------------------

class ScreenEvent
{
public:
    typedef enum {
        // Valid for both mouse and finger.
        EVENT_TYPE_TOUCH,
        EVENT_TYPE_RELEASE
    } EventType;

    ScreenEvent(EventType type, std::shared_ptr<const Point> p) : type(type), point(p) { }

    EventType       GetEventType() const { return this->type; }
    const Point&    GetPoint() const { return *this->point.get(); }

private:
    EventType                       type;
    std::shared_ptr<const Point>    point;
};


//-----------------------------------------------------------------------------------------------------------
//  class ScreenElement
//-----------------------------------------------------------------------------------------------------------

class ScreenElement
{
public:
#if !defined(DEBUG)
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
#endif

    static ScreenElementId GetIdFromName(const std::string &name) { return StringUtilities::Hash(name); }

    ScreenElement();

    virtual bool    Init(tinyxml2::XMLElement *data);

    virtual void    OnUpdate(float delta) = 0;
    virtual void    OnRender();

    virtual bool    HandleEvent(const ScreenEvent &event) = 0;

    bool            IsVisible() const { return this->visible; }

protected:
    Matrix4f        CalculateMVP();
    bool            IsPointInside(const Point &point) const;

protected:
    static const DefaultVertexData  ElementVerticies[];
    static const unsigned int       VertexCount;

    std::shared_ptr<SpriteShader>   shader;
    std::shared_ptr<Resource>       textureResource;

    Matrix4f                        projectionMatrix;
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

    virtual bool    HandleEvent(const ScreenEvent &event) override;

private:
    static const std::string    name;

    std::string text;
};

#endif // __SCREENELEMENT_H__