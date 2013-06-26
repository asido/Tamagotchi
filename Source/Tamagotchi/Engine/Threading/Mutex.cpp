#include "Mutex.h"

//-----------------------------------------------------------------------------------------------------------
//  class Mutex
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------
// Public
//-----------------------------------------------

Mutex::Mutex()
{
    Init();
}

Mutex::Mutex(const Mutex &m)
{
    Init();

    if (m.locked && !this->locked)
    {
        Lock();
    }
    else if (!m.locked && locked)
    {
        Unlock();
    }
}

Mutex::~Mutex()
{
#if defined(WIN32)
    DeleteCriticalSection(&mutex);
#elif defined(__APPLE__)
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);
#else
#   error implement me
#endif
}

Mutex& Mutex::operator=(const Mutex &m)
{
    if (m.locked && !locked)
    {
        Lock();
    }
    else if (!m.locked && locked)
    {
        Unlock();
    }

    return *this;
}

bool Mutex::Lock()
{
    locked = true;

#if defined(WIN32)
    EnterCriticalSection(&mutex);
    return true;
#elif defined(__APPLE__)
    return pthread_mutex_lock(&mutex) == 0;
#else
#   error implement me
#endif
}

bool Mutex::TryLock()
{
    locked = true;

#if defined(WIN32)
    return TryEnterCriticalSection(&mutex) == TRUE;
#elif defined(__APPLE__)
    return pthread_mutex_trylock(&mutex) == 0;
#else
#   error implement me
#endif
}

bool Mutex::Unlock()
{
    locked = false;

#if defined(WIN32)
    LeaveCriticalSection(&mutex);
    return true;
#elif defined(__APPLE__)
    return pthread_mutex_unlock(&mutex) == 0;
#else
#   error implement me
#endif
}

bool Mutex::IsLocked() const
{
    return locked;
}

//-----------------------------------------------
// Private
//-----------------------------------------------

void Mutex::Init()
{
#if defined(WIN32)
    InitializeCriticalSection(&mutex);
#elif defined(__APPLE__)
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
    pthread_mutexattr_destroy(&attr);
#else
#   error implement me
#endif
}


//-----------------------------------------------------------------------------------------------------------
//  class ScopedMutex
//-----------------------------------------------------------------------------------------------------------

ScopedMutex::ScopedMutex(Mutex &mutex)
    : mutex(mutex)
{
    this->mutex.Lock();
}

ScopedMutex::~ScopedMutex()
{
    this->mutex.Unlock();
}