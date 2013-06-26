#ifndef __MUTEX_H__
#define __MUTEX_H__

#if defined(WIN32)
#   include <windows.h>
#elif defined(__APPLE__)
#   include <pthread.h>
#else
#error implement me
#endif

//-----------------------------------------------------------------------------------------------------------
//  class Mutex
//-----------------------------------------------------------------------------------------------------------

class Mutex
{
public:
    Mutex();
    Mutex(const Mutex &m);
    ~Mutex();

    Mutex& operator=(const Mutex &m);

    bool Lock();
    bool TryLock();
    bool Unlock();
    bool IsLocked() const;

private:
    void Init();

private:
#if defined(WIN32)
    CRITICAL_SECTION mutex;
#elif defined(__APPLE__)
    pthread_mutex_t mutex;
#else
#   error implement me
#endif

    bool locked;
};


//-----------------------------------------------------------------------------------------------------------
//  class ScopedMutex
//-----------------------------------------------------------------------------------------------------------

class ScopedMutex
{
public:
    ScopedMutex(Mutex &mutex);
    ~ScopedMutex();

private:
    Mutex &mutex;
};

#endif // __MUTEX_H__