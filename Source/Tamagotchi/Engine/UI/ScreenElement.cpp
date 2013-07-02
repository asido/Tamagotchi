#include "ScreenElement.h"
#include "Logger.h"
#include "TamagotchiEngine.h"
#include "Geometry.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderManager.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/TextureResource.h"
#include "Actors/TransformComponent.h"

//-----------------------------------------------------------------------------------------------------------
//  class ScreenElement
//-----------------------------------------------------------------------------------------------------------

const DefaultVertexData ScreenElement::ElementVerticies[] = {
    { Vector3f(0.0f, 0.0f, 0.0f), Vector2f(0.0f, 0.0f) },
    { Vector3f(1.0f, 0.0f, 0.0f), Vector2f(1.0f, 0.0f) },
    { Vector3f(0.0f, 1.0f, 0.0f), Vector2f(0.0f, 1.0f) },
    { Vector3f(1.0f, 1.0f, 0.0f), Vector2f(1.0f, 1.0f) }
};
const unsigned int ScreenElement::VertexCount(ARRAY_SIZE(ScreenElement::ElementVerticies));

ScreenElement::ScreenElement()
    : transform(TG_NEW TransformComponent), visible(true)
{
    // For UI rendering we use projection matrix with same bounds as viewport in order
    // to be able to specify frame values in pixels.
    Resolution resolution = g_engine->GetRenderer()->GetResolution();
    this->projectionMatrix = Math3D::MakeOrthoMatrix(0.0f, static_cast<float>(resolution.width),
                                                     0.0f, static_cast<float>(resolution.height),
                                                     1.0f, 100.0f);
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
        float x = static_cast<float>(atof(frameElement->Attribute("x")));
        float y = static_cast<float>(atof(frameElement->Attribute("y")));
        float width = static_cast<float>(atof(frameElement->Attribute("width")));
        float height = static_cast<float>(atof(frameElement->Attribute("height")));

        // XXX: It's not a good idea to hardcode Z index, but as long as we don't have overlapping windows
        // it will do the job.
        Vector3f pos(x, y, -1.0f);
        this->transform->SetPosition(pos);
        Vector3f scale(width, height, 1.0f);
        this->transform->SetScale(scale);
    }
    else
    {
        LogWarning("Screen element data has no frame element.");
    }

    // Init shader.
    tinyxml2::XMLElement *shaderElement = data->FirstChildElement("Shader");
    if (shaderElement)
    {
        const std::string shaderName(shaderElement->Attribute("name"));
        this->shader = std::static_pointer_cast<SpriteShader>(g_engine->GetShaderManager()->GetShader(shaderName));

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

    // Init VBO
    glGenVertexArraysOES(1, &this->glVertexArray);
    glBindVertexArrayOES(this->glVertexArray);

    glGenBuffers(1, &this->glBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->glBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ButtonElement::ElementVerticies), ButtonElement::ElementVerticies, GL_STATIC_DRAW);

    glVertexAttribPointer(DEFAULT_VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(DefaultVertexData), static_cast<GLvoid*>(NULL + offsetof(DefaultVertexData, positionCoords)));
    glEnableVertexAttribArray(DEFAULT_VERTEX_ATTRIB_POSITION);

    glVertexAttribPointer(DEFAULT_VERTEX_ATTRIB_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(DefaultVertexData), reinterpret_cast<GLvoid*>(NULL + offsetof(DefaultVertexData, textureCoords)));
    glEnableVertexAttribArray(DEFAULT_VERTEX_ATTRIB_TEXTURE);

    GL_CHECK_ERROR();

    return true;
}

void ScreenElement::OnRender()
{
    glBindVertexArrayOES(this->glVertexArray);

    std::shared_ptr<ResourceHandle> handle = g_engine->GetResourceManager()->GetHandle(*this->textureResource.get());
    std::shared_ptr<GLESTextureResourceExtraData> textureExtra = std::static_pointer_cast<GLESTextureResourceExtraData>(handle->GetExtra());

    this->shader->SetTexture(textureExtra->GetTexture());
    this->shader->SetMvpMatrix(this->CalculateMVP());

    if (!this->shader->PrepareToRender())
    {
        LogError("Shader prepare to render has failed.");
        return;
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, ScreenElement::VertexCount);

    GL_CHECK_ERROR();
}

Matrix4f ScreenElement::CalculateMVP()
{
    return this->projectionMatrix * this->transform->GetTransform();
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

    LogInfo("New ButtonElement initialized.");

    return true;
}

void ButtonElement::OnUpdate(float delta)
{

}

void ButtonElement::OnRender()
{
    ScreenElement::OnRender();
}