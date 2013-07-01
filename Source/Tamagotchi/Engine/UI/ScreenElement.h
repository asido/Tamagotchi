#ifndef __SCREENELEMENT_H__
#define __SCREENELEMENT_H__

#include <memory>
#include <string>
#include <tinyxml2.h>
#include "StringUtilities.h"

typedef unsigned int ScreenElementId;
const ScreenElementId INVALID_SCREEN_ELEMENT_ID = 0;

//-----------------------------------------------------------------------------------------------------------
//  class ScreenElement
//-----------------------------------------------------------------------------------------------------------

class ScreenElement
{
public:
    static ScreenElementId GetIdFromName(const std::string &name) { return StringUtilities::Hash(name); }

    ScreenElement();

    virtual bool    Init(tinyxml2::XMLElement *data);

    virtual void    OnUpdate(float delta) = 0;
    virtual void    OnRender() = 0;

    bool            IsVisible() const { return this->visible; }

protected:
    float   x, y;
    float   width, height;

    bool    visible;
};


//-----------------------------------------------------------------------------------------------------------
//  class ButtonElement
//-----------------------------------------------------------------------------------------------------------

class Shader;
class Resource;

class ButtonElement : public ScreenElement
{
public:
    static ScreenElementId GetIdStatic();

    virtual bool    Init(tinyxml2::XMLElement *data) override;

    virtual void    OnUpdate(float delta) override;
    virtual void    OnRender() override;

private:
    static const std::string    name;

    std::shared_ptr<Shader>     shader;
    std::shared_ptr<Resource>   textureResource;
    std::string                 text;
};

#endif // __SCREENELEMENT_H__