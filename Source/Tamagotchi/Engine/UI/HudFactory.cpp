#include "HudFactory.h"
#include "ScreenElement.h"
#include "Logger.h"

//-----------------------------------------------------------------------------------------------------------
//  class HudFactory
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

HudFactory::HudFactory()
{
    this->elementFactory.Register<ButtonElement>(ButtonElement::GetIdStatic());
}

std::shared_ptr<ScreenElement> HudFactory::CreateScreenElement(tinyxml2::XMLElement *data)
{
    if (!data)
    {
        LogError("XML data == NULL.");
        return std::shared_ptr<ScreenElement>();
    }

    const std::string name(data->Value());
    std::shared_ptr<ScreenElement> element(this->elementFactory.Create(ScreenElement::GetIdFromName(name)));

    if (element)
    {
        if (!element->Init(data))
        {
            LogError("Failed to initialize screen element named '%s'.", name.c_str());
            return std::shared_ptr<ScreenElement>();
        }
    }
    else
    {
        LogError("Failed to create screen element named '%s'.", name.c_str());
        return std::shared_ptr<ScreenElement>();
    }

    return element;
}

//-----------------------------------------------
// Private
//-----------------------------------------------

ScreenElementId HudFactory::GetNextScreenElementId() const
{
    static ScreenElementId id = 0;
    id++;
    return id;
}