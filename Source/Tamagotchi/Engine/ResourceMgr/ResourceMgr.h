#ifndef __RESOURCEMGR_H__
#define __RESOURCEMGR_H__

class ResourceMgr;

//-----------------------------------------------------------------------------------------------------------
//  class Resource
//-----------------------------------------------------------------------------------------------------------

class Resource
{
public:
	Resource(const std::string &name) : name(name) {};
	const std::string &GetName() const { return this->name; }

private:
	std::string name;
};


//-----------------------------------------------------------------------------------------------------------
//  class IResourceFile
//-----------------------------------------------------------------------------------------------------------

class IResourceFile
{
public:
	virtual bool			Open() = 0;
	virtual int				GetRawResourceSize(const Resource &r) = 0;
	virtual int				GetRawResource(const Resource &r, char *buffer) = 0;
	virtual int				GetNumResources() const = 0;
	virtual std::string		GetResourceName(int num) const = 0;
};


//-----------------------------------------------------------------------------------------------------------
//  class IResourceLoader
//-----------------------------------------------------------------------------------------------------------

class ResourceHandle;

class IResourceLoader
{
public:
	virtual std::string		GetPattern() = 0;
	virtual bool			UseRawFile() = 0;
	virtual bool			DiscardRawBufferAfterLoad() = 0;
	virtual bool			AddNullZero() { return false; }
	virtual unsigned int	GetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) = 0;
	virtual bool			LoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<ResourceHandle> handle) = 0;
};


//-----------------------------------------------------------------------------------------------------------
//  class IResourceExtraData
//-----------------------------------------------------------------------------------------------------------

class IResourceExtraData
{
public:
	virtual std::string ToString() = 0;
};


//-----------------------------------------------------------------------------------------------------------
//  class ResourceHandle
//-----------------------------------------------------------------------------------------------------------

class ResourceHandle
{
	friend class ResourceMgr;

public:
	ResourceHandle(const Resource &resource, char *buffer, unsigned int size, ResourceMgr *resourceMgr);
	~ResourceHandle();

	std::shared_ptr<IResourceExtraData> GetExtra() const { return this->extra; }
	void SetExtra(std::shared_ptr<IResourceExtraData> extra) { this->extra = extra; }

private:
	ResourceMgr		*resourceMgr;
	const Resource	resource;
	char			*buffer;
	unsigned int	size;
	std::shared_ptr<IResourceExtraData> extra;
};


//-----------------------------------------------------------------------------------------------------------
//  class ResourceMgr
//-----------------------------------------------------------------------------------------------------------

typedef std::list< std::shared_ptr<ResourceHandle> >				ResourceHandleList;
typedef std::map< std::string, std::shared_ptr<ResourceHandle> >	ResourceHandleMap;
typedef std::list< std::shared_ptr<IResourceLoader> >				ResourceLoaders;

class ResourceMgr
{
	friend class ResourceHandle;

public:
	ResourceMgr(unsigned int cacheSize, std::shared_ptr<IResourceFile> file);
	virtual ~ResourceMgr();

	bool Init();
	void RegisterLoader(std::shared_ptr<IResourceLoader> loader);
	std::shared_ptr<ResourceHandle> GetHandle(const Resource &r);

private:
	bool MakeRoom(unsigned int size);
	char *Allocate(unsigned int size);
	void Free(std::shared_ptr<ResourceHandle> gonner);

	std::shared_ptr<ResourceHandle> Load(const Resource &r);
	std::shared_ptr<ResourceHandle> Find(const Resource &r);
	void							Update(std::shared_ptr<ResourceHandle> handle);

	void FreeOneResource();
	void MemoryHasBeenFreed(unsigned int size);

private:
	// Resource usage list. The front contains most recently referred resources while the end the least.
	ResourceHandleList	lru;
	ResourceHandleMap	resources;
	ResourceLoaders		resourceLoaders;

	// A compressed resource file holding all the assets.
	std::shared_ptr<IResourceFile> file;

	// Total memory size.
	unsigned int	cacheSize;
	// Total memory allocated.
	unsigned int	allocatedSize;
};

#endif // __RESOURCEMGR_H__