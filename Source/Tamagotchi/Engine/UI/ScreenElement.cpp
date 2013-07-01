#include "ScreenElement.h"
#include "Logger.h"
#include "TamagotchiEngine.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderManager.h"
#include "ResourceManager/ResourceManager.h"

//-----------------------------------------------------------------------------------------------------------
//  class ScreenElement
//-----------------------------------------------------------------------------------------------------------

ScreenElement::ScreenElement()
    : x(0), y(0), width(0), height(0), visible(true)
{

}

bool ScreenElement::Init(tinyxml2::XMLElement *data)
{
    if (!data)
    {
        LogError("XML data == NULL");
        return false;
    }

    // Init frame.
    tinyxml2::XMLElement *frameElement = data->FirstChildElement("Frame");
    if (frameElement)
    {
        this->x = static_cast<float>(atof(frameElement->Attribute("x")));
        this->y = static_cast<float>(atof(frameElement->Attribute("y")));
        this->width = static_cast<float>(atof(frameElement->Attribute("width")));
        this->height = static_cast<float>(atof(frameElement->Attribute("height")));
    }
    else
    {
        LogWarning("Screen element data has no frame element.");
    }

    return true;
}

void ScreenElement::OnUpdate(float delta)
{
    LogError("Implement me!");
}

void ScreenElement::OnRender()
{
    LogError("Implement me!");
}


//-----------------------------------------------------------------------------------------------------------
//  class ButtonElement
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Static
//-----------------------------------------------

const std::string ButtonElement::name("ButtonElement");

ScreenElementId ButtonElement::GetIdStatic()
{
    static ScreenElementId id = INVALID_SCREEN_ELEMENT_ID;

    if (id == INVALID_SCREEN_ELEMENT_ID)
    {
        id = StringUtilities::Hash(ButtonElement::name);
    }

    return id;
}

//-----------------------------------------------
// Public
//-----------------------------------------------

bool ButtonElement::Init(tinyxml2::XMLElement *data)
{
    if (!data)
    {
        LogError("XML data == NULL");
        return false;
    }

    if (!ScreenElement::Init(data))
    {
        LogError("Button initialization has failed.");
        return false;
    }

    // Init shader.
    tinyxml2::XMLElement *shaderElement = data->FirstChildElement("Shader");
    if (shaderElement)
    {
        const std::string shaderName(shaderElement->Attribute("name"));
        this->shader = g_engine->GetShaderManager()->GetShader(shaderName);

        if (!this->shader)
        {
            LogError("Failed to load shader: %s", shaderName.c_str());
            return false;
        }
    }
    else
    {
        LogWarning("Scree element data has no shader element.");
    }

    // Init texture.
    tinyxml2::XMLElement *textureElement = data->FirstChildElement("Texture");
    if (textureElement)
    {
        const std::string resourceAttr(textureElement->Attribute("resource"));
        this->textureResource = std::shared_ptr<Resource>(TG_NEW Resource(resourceAttr));
    }
    else
    {
        LogWarning("Screen element data has no texture element");
    }

    // Init text.
    tinyxml2::XMLElement *textElement = data->FirstChildElement("Text");
    if (textElement)
    {
        this->text = std::string(textElement->GetText());
    }
    else
    {
        LogWarning("Button element data has no text element.");
    }

    return true;
}

void ButtonElement::OnUpdate(float delta)
{
    LogError("Implement me!");
}

void ButtonElement::OnRender()
{
    LogError("Implement me!");
}