#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

//-----------------------------------------------------------------------------------------------------------
//  class Allocator
//      
//-----------------------------------------------------------------------------------------------------------

template <class T>
class Allocator
{
public:
    Allocator() : cache(NULL), size(0), currUsed(0) { }
    ~Allocator() { Release(); }
    
    void Release()
    {
        if (this->cache)
        {
            free(this->cache);
            this->size = 0;
            this->cache = NULL;
            this->currUsed = 0;
        }
    }

    void Deallocate() { this->currUsed = 0; }

    void Init(int size)
    {
        if (size > this->size)
        {
            T* newCache = static_cast<T*>(calloc(sizeof(T), size));

            if (newCache)
            {
                memcpy(newCache, this->cache, this->currUsed * sizeof(T));
            }
            else
            {
                LogError("Not enough memory.");
            }

            Release();
            this->cache = newCache;
            this->size = size;
        }
    }

    T* New()
    {
        LogAssert(this->currUsed < this->size && "Surprise allocation.");
        return &this->cache[this->currUsed++];
    }

    T*      GetCache() const { return this->cache; }
    T&      Get(int index) const { return this->cache[index]; }
    T*      GetPtr(int index) const { return &this->cache[index]; }
    void    Remove() { this->currUsed--; }

private:
    T* cache;
    int size;
    int currUsed;
};

#endif // __ALLOCATOR_H__