#ifndef __HUDFACTORY_H__
#define __HUDFACTORY_H__

#include <memory>
#include <tinyxml2.h>
#include "ObjectFactory.h"
#include "ScreenElement.h"

//-----------------------------------------------------------------------------------------------------------
//  class HudFactory
//-----------------------------------------------------------------------------------------------------------

class HudFactory
{
public:
    HudFactory();

    std::shared_ptr<ScreenElement>  CreateScreenElement(tinyxml2::XMLElement *data);

private:
    ScreenElementId                 GetNextScreenElementId() const;

private:
    GenericObjectFactory<ScreenElement, ScreenElementId> elementFactory;
};

#endif // __HUDFACTORY_H__