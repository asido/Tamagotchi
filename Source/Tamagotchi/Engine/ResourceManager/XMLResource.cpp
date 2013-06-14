#include "XMLResource.h"
#include "defines.h"

//-----------------------------------------------------------------------------------------------------------
//  class XMLResourceExtraData
//-----------------------------------------------------------------------------------------------------------

void XMLResourceExtraData::ParseXML(char *rawBuffer)
{
	this->xmlDocument.Parse(rawBuffer);
}

tinyxml2::XMLElement *XMLResourceExtraData::GetRoot()
{
	return this->xmlDocument.RootElement();
}


//-----------------------------------------------------------------------------------------------------------
//  class XMLResourceLoader
//-----------------------------------------------------------------------------------------------------------

bool XMLResourceLoader::LoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<ResourceHandle> handle)
{
	if (rawSize <= 0)
	{
		return false;
	}

	std::shared_ptr<XMLResourceExtraData> extraData = std::shared_ptr<XMLResourceExtraData>(TG_NEW XMLResourceExtraData());
	extraData->ParseXML(rawBuffer);
	handle->SetExtra(std::shared_ptr<XMLResourceExtraData>(extraData));

	return true;
}