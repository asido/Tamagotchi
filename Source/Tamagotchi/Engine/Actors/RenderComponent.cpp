#include <Eigen/Dense>
using namespace Eigen;

#include "RenderComponent.h"
#include "EventManager/EventManager.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/ShaderResource.h"
#include "Rendering/Shader.h"
#include "Rendering/SceneNode.h"
#include "TamagotchiEngine.h"

//-----------------------------------------------------------------------------------------------------------
//  class RenderComponent
//-----------------------------------------------------------------------------------------------------------

void RenderComponent::PostInit()
{
    std::shared_ptr<SceneNode> sceneNode(CreateSceneNode());
    std::shared_ptr<Event_NewRenderComponent> event = std::shared_ptr<Event_NewRenderComponent>(TG_NEW Event_NewRenderComponent(this->GetOwner()->GetId(), sceneNode));
    EventManager::Get().TriggerEvent(event);
}


//-----------------------------------------------------------------------------------------------------------
//  class SpriteRenderComponent
//-----------------------------------------------------------------------------------------------------------
enum {
    VERTEX_ATTRIB_POSITION=0,
    VERTEX_ATTRIB_TEXTURE
};

typedef struct {
    Vector3f positionCoords;
    Vector2f textureCoords;
} VertexData;

static const VertexData SpriteVerticies[] = {
    { Vector3f(0.0f, 0.0f, 0.0f), Vector2f(0.0f, 0.0f) },
    { Vector3f(1.0f, 0.0f, 0.0f), Vector2f(1.0f, 0.0f) },
    { Vector3f(0.0f, 1.0f, 0.0f), Vector2f(0.0f, 1.0f) },
    { Vector3f(1.0f, 1.0f, 0.0f), Vector2f(1.0f, 1.0f) }
};

SpriteRenderComponent::SpriteRenderComponent()
    : vertexCount(4)
{

}

bool SpriteRenderComponent::Init(tinyxml2::XMLElement *data)
{
    // Init texture.
    tinyxml2::XMLElement *textureElement = data->FirstChildElement("Texture");
    if (textureElement)
    {
        this->textureFilename(textureElement->Attribute("resource"));

        if (this->textureFilename.size() == 0)
        {
            LogError("<Texture> tag has no 'resource' attribute.");
            return false;
        }
    }
    else
    {
        LogError("SpriteRenderComponent without <Texture> element.");
        return false;
    }

    // Init shader.
    tinyxml2::XMLElement *shaderElement = data->FirstChildElement("Shader");
    if (shaderElement)
    {
        this->shaderName(shaderElement->Attribute("name"));

        if (this->shaderName.size() == 0)
        {
            LogError("<Shader> tag has no 'name' attribute.");
            return false;
        }
    }
    else
    {
        LogError("SpriteRenderComponent without <Shader> element.");
        return false;
    }

    // Init VBO
    glGenVertexArraysOES(1, &this->glVertexArray);
    glBindVertexArrayOES(this->glVertexArray);

    glGenBuffers(1, &this->glbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->glbuffer);
    glBufferData(this->glbuffer, sizeof(SpriteVerticies), SpriteVerticies, GL_STATIC_DRAW);

    glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), static_cast<GLvoid*>(NULL + offsetof(VertexData, positionCoords)));
    glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);

    glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<GLvoid*>(NULL + offsetof(VertexData, textureCoords)));
    glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE);

    return true;
}

std::shared_ptr<SceneNode> SpriteRenderComponent::CreateSceneNode()
{
    return std::shared_ptr<SpriteSceneNode>(TG_NEW SpriteSceneNode(GetOwner()->GetId(), std::shared_ptr(this)));
}