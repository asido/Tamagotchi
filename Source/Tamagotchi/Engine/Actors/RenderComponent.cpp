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

bool RenderComponent::Init(tinyxml2::XMLElement *data)
{
    if (!data)
    {
        LogError("XML data == NULL");
        return false;
    }

    // Init shader.
    tinyxml2::XMLElement *shaderElement = data->FirstChildElement("Shader");
    if (shaderElement)
    {
        this->shaderName = shaderElement->Attribute("name");

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

    return true;
}

void RenderComponent::PostInit()
{
    std::shared_ptr<SceneNode> sceneNode(CreateSceneNode());
    std::shared_ptr<Event_NewRenderComponent> event = std::shared_ptr<Event_NewRenderComponent>(TG_NEW Event_NewRenderComponent(sceneNode));
    EventManager::Get().TriggerEvent(event);
}


//-----------------------------------------------------------------------------------------------------------
//  class SpriteRenderComponent
//-----------------------------------------------------------------------------------------------------------

const std::string SpriteRenderComponent::name("SpriteRenderComponent");

ComponentId SpriteRenderComponent::GetIdStatic()
{
    static ComponentId id = INVALID_COMPONENT_ID;

    if (id == INVALID_COMPONENT_ID)
    {
        id = StringUtilities::Hash(SpriteRenderComponent::name);
    }

    return id;
}

SpriteRenderComponent::SpriteRenderComponent()
{
    LogSpam("SpriteRenderComponent created: %s", this->textureFilename.c_str());
}

SpriteRenderComponent::~SpriteRenderComponent()
{
    LogSpam("SpriteRenderComponent destroyed: %s", this->textureFilename.c_str());
}

bool SpriteRenderComponent::Init(tinyxml2::XMLElement *data)
{
    if (!data)
    {
        LogError("XML data == NULL");
        return false;
    }

    if (!RenderComponent::Init(data))
    {
        LogError("RenderComponent::Init has failed.");
        return true;
    }

    // Init texture.
    tinyxml2::XMLElement *textureElement = data->FirstChildElement("Texture");
    if (textureElement)
    {
        this->textureFilename = textureElement->Attribute("resource");

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

    return true;
}

std::shared_ptr<SceneNode> SpriteRenderComponent::CreateSceneNode()
{
    std::shared_ptr<ActorComponent> smartThis = ActorComponent::shared_from_this();
    std::shared_ptr<SceneNode> sceneNode = std::shared_ptr<SpriteSceneNode>(TG_NEW SpriteSceneNode(GetOwner()->GetId(), std::static_pointer_cast<RenderComponent>(smartThis)));
    
    if (!sceneNode->Init())
    {
        LogError("SceneNode creation has failed.");
        return std::shared_ptr<SceneNode>();
    }

    return sceneNode;
}