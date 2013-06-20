#include "XMLResource.h"
#include "defines.h"
#include "Logger.h"

//-----------------------------------------------------------------------------------------------------------
//  class XMLResourceExtraData
//-----------------------------------------------------------------------------------------------------------

bool XMLResourceExtraData::ParseXML(char *rawBuffer)
{
    int result = this->xmlDocument.Parse(rawBuffer);

    if (result != tinyxml2::XML_NO_ERROR)
    {
        LogError("Failed parsing XML file: %d", result);
        return false;
    }
    return true;
}

tinyxml2::XMLElement *XMLResourceExtraData::GetRoot()
{
    return this->xmlDocument.RootElement();
}


//-----------------------------------------------------------------------------------------------------------
//  class XMLResourceLoader
//-----------------------------------------------------------------------------------------------------------

unsigned int XMLResourceLoader::GetLoadedResourceSize(char *rawBuffer, unsigned int rawSize)
{
    // Keep the resource cache from allocating memory for the XML file so that tinyxml2 can manage it on it's own.
    return 0;
}

bool XMLResourceLoader::LoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<ResourceHandle> handle)
{
    if (rawSize <= 0)
    {
        return false;
    }

    std::shared_ptr<XMLResourceExtraData> extraData = std::shared_ptr<XMLResourceExtraData>(TG_NEW XMLResourceExtraData());
    if (!extraData->ParseXML(rawBuffer))
    {
        LogError("Failed loadng XML resource: %s", handle->GetResource().GetName().c_str());
        return false;
    }
    handle->SetExtra(std::shared_ptr<XMLResourceExtraData>(extraData));

    return true;
}