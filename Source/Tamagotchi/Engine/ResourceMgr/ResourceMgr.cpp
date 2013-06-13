#include "ResourceMgr.h"
#include "ZipFile.h"
#include "XMLResource.h"
#include "../StringUtilities.h"

//-----------------------------------------------------------------------------------------------------------
//  class ResourceHandle
//-----------------------------------------------------------------------------------------------------------

ResourceHandle::ResourceHandle(const Resource &resource, char *buffer, unsigned int size, ResourceMgr *resourceMgr)
	: resource(resource), buffer(buffer), size(size), resourceMgr(resourceMgr)
{

}

ResourceHandle::~ResourceHandle()
{
	// Report resourceMgr that buffer is freed
}

//-----------------------------------------------------------------------------------------------------------
//  class ResourceMgr
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

ResourceMgr::ResourceMgr(unsigned int cacheSize, std::shared_ptr<IResourceFile> file)
	: allocatedSize(0), cacheSize(cacheSize), file(file)
{

}

ResourceMgr::~ResourceMgr()
{
	while (!this->lru.empty())
	{
		FreeOneResource();
	}
}

bool ResourceMgr::Init()
{
	if (!this->file)
	{
		return false;
	}

	if (!this->file->Open())
	{
		return false;
	}

	// Register all resource loaders.
	RegisterLoader(std::shared_ptr<IResourceLoader>(TG_NEW XMLResourceLoader()));

	return true;
}

void ResourceMgr::RegisterLoader(std::shared_ptr<IResourceLoader> loader)
{
	this->resourceLoaders.push_front(loader);
}

std::shared_ptr<ResourceHandle> ResourceMgr::GetHandle(const Resource &r)
{
	std::shared_ptr<ResourceHandle> handle(Find(r));

	if (!handle)
	{
		handle = Load(r);
		TG_ASSERT(handle);
	}
	else
	{
		Update(handle);
	}

	return handle;
}

//-----------------------------------------------
// Private
//-----------------------------------------------

bool ResourceMgr::MakeRoom(unsigned int size)
{
	if (size > this->cacheSize)
	{
		return false;
	}

	while (size > (this->cacheSize - this->allocatedSize))
	{
		if (this->lru.empty())
		{
			// The cache is empty, and there's still not enough room.
			return false;
		}

		FreeOneResource();
	}

	return true;
}

char *ResourceMgr::Allocate(unsigned int size)
{
	if (!MakeRoom(size))
	{
		return NULL;
	}

	char *mem = TG_NEW char[size];
	if (mem)
	{
		this->allocatedSize += size;
	}
	return mem;
}

void ResourceMgr::Free(std::shared_ptr<ResourceHandle> gonner)
{
	this->lru.remove(gonner);
	this->resources.erase(gonner->resource.GetName());
}

std::shared_ptr<ResourceHandle> ResourceMgr::Load(const Resource &r)
{
	std::shared_ptr<IResourceLoader>	loader = NULL;
	std::shared_ptr<ResourceHandle>		handle = NULL;

	for (ResourceLoaders::iterator it = this->resourceLoaders.begin(); it != this->resourceLoaders.end(); ++it)
	{
		std::shared_ptr<IResourceLoader> l = *it;

		if (StringUtilities::WildcardMatch(l->GetPattern(), r.GetName()))
		{
			loader = l;
			break;
		}
	}

	if (!loader)
	{
		TG_ERROR("loader for file '" + r.GetName() + "' not found");
		return handle;
	}

	int rawSize = this->file->GetRawResourceSize(r);
	if (rawSize < 0)
	{
		TG_ERROR("Resource not found: " + r.GetName());
		return handle;
	}

	int		allocSize = rawSize + (loader->AddNullZero() ? 1 : 0);
	char	*rawBuffer = (loader->UseRawFile() ? Allocate(allocSize) : TG_NEW char[allocSize]);
	memset(rawBuffer, 0, allocSize);

	if (!rawBuffer || !this->file->GetRawResource(r, rawBuffer))
	{
		return handle;
	}

	char			*buffer = NULL;
	unsigned int	size = 0;
	
	if (loader->UseRawFile())
	{
		buffer = rawBuffer;
		handle = std::shared_ptr<ResourceHandle>(TG_NEW ResourceHandle(r, buffer, rawSize, this));
	}
	else
	{
		size = loader->GetLoadedResourceSize(rawBuffer, rawSize);
		buffer = Allocate(size);

		if (!rawBuffer || !buffer)
		{
			// TODO: Resource cache is out of memory. Print at least a log message.
			return handle;
		}

		handle = std::shared_ptr<ResourceHandle>(TG_NEW ResourceHandle(r, buffer, size, this));
		bool success = loader->LoadResource(rawBuffer, rawSize, handle);

		if (loader->DiscardRawBufferAfterLoad())
		{
			SAFE_DELETE_ARRAY(rawBuffer);
		}

		if (!success)
		{
			return NULL;
		}
	}

	if (handle)
	{
		this->lru.push_front(handle);
		this->resources[r.GetName()] = handle;
	}

	return handle;
}

std::shared_ptr<ResourceHandle> ResourceMgr::Find(const Resource &r)
{
	ResourceHandleMap::iterator it = this->resources.find(r.GetName());
	if (it == this->resources.end())
	{
		return NULL;
	}
	return it->second;
}

void ResourceMgr::Update(std::shared_ptr<ResourceHandle> handle)
{
	this->lru.remove(handle);
	this->lru.push_front(handle);
}

void ResourceMgr::FreeOneResource()
{
	ResourceHandleList::iterator gonner = this->lru.end();
	gonner--;

	std::shared_ptr<ResourceHandle> handle = *gonner;

	this->lru.pop_back();
	this->resources.erase(handle->resource.GetName());

	// NOTE: You can't change the resource cache size yet. The resource bits could still actually
	// be used by some subsystem holding onto the ResourceHandle. Only when it goes out of scope
	// can the memory be actually free again.
}

void ResourceMgr::MemoryHasBeenFreed(unsigned int size)
{
	this->allocatedSize -= size;
}