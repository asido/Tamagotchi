#include "ResourceManager.h"
#include "ZipFile.h"
#include "XMLResource.h"
#include "TextureResource.h"
#include "StringUtilities.h"
#include "defines.h"
#include "Logger.h"

//-----------------------------------------------------------------------------------------------------------
//  class ResourceHandle
//-----------------------------------------------------------------------------------------------------------

ResourceHandle::ResourceHandle(const Resource &resource, char *buffer, unsigned int size, ResourceManager *resourceMgr)
    : resource(resource), buffer(buffer), size(size), resourceMgr(resourceMgr)
{

}

ResourceHandle::~ResourceHandle()
{
    SAFE_DELETE_ARRAY(this->buffer);
    this->resourceMgr->MemoryHasBeenFreed(this->size);
}

//-----------------------------------------------------------------------------------------------------------
//  class ResourceManager
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

ResourceManager::ResourceManager(unsigned int cacheSize, std::shared_ptr<IResourceFile> file)
    : allocatedSize(0), cacheSize(cacheSize), file(file)
{

}

ResourceManager::~ResourceManager()
{
    while (!this->lru.empty())
    {
        FreeOneResource();
    }
}

bool ResourceManager::Init()
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
    RegisterLoader(std::shared_ptr<IResourceLoader>(TG_NEW TextureResourceLoader()));

    return true;
}

void ResourceManager::RegisterLoader(std::shared_ptr<IResourceLoader> loader)
{
    this->resourceLoaders.push_front(loader);
}

std::shared_ptr<ResourceHandle> ResourceManager::GetHandle(const Resource &r)
{
    std::shared_ptr<ResourceHandle> handle(Find(r));

    if (!handle)
    {
        handle = Load(r);
        if (!handle)
        {
            LogWarning("ResourceHandle for file %s couldn't be loaded.", r.GetName());
        }
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

bool ResourceManager::MakeRoom(unsigned int size)
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

char *ResourceManager::Allocate(unsigned int size)
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

void ResourceManager::Free(std::shared_ptr<ResourceHandle> gonner)
{
    this->lru.remove(gonner);
    this->resources.erase(gonner->resource.GetName());
}

std::shared_ptr<ResourceHandle> ResourceManager::Load(const Resource &r)
{
    std::shared_ptr<IResourceLoader>    loader = NULL;
    std::shared_ptr<ResourceHandle>     handle = NULL;

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
        LogWarning("loader for file '" + r.GetName() + "' not found");
        return handle;
    }

    int rawSize = this->file->GetRawResourceSize(r);
    if (rawSize < 0)
    {
        LogWarning("Resource not found: " + r.GetName());
        return handle;
    }

    int allocSize = rawSize + (loader->AddNullZero() ? 1 : 0);
    char *rawBuffer = (loader->UseRawFile() ? Allocate(allocSize) : TG_NEW char[allocSize]);
    memset(rawBuffer, 0, allocSize);

    if (!rawBuffer || !this->file->GetRawResource(r, rawBuffer))
    {
        return handle;
    }

    char *buffer = NULL;
    unsigned int size = 0;
    
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
            LogError("Resource manager cache is out of memory.");
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

std::shared_ptr<ResourceHandle> ResourceManager::Find(const Resource &r)
{
    ResourceHandleMap::iterator it = this->resources.find(r.GetName());
    if (it == this->resources.end())
    {
        return NULL;
    }
    return it->second;
}

void ResourceManager::Update(std::shared_ptr<ResourceHandle> handle)
{
    this->lru.remove(handle);
    this->lru.push_front(handle);
}

void ResourceManager::FreeOneResource()
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

void ResourceManager::MemoryHasBeenFreed(unsigned int size)
{
    this->allocatedSize -= size;
}