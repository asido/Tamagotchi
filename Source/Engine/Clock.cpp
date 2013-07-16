#include <cassert>

#if defined (__APPLE__)
#include <sys/time.h>
#endif

#include "Clock.h"
#include "Logger.h"

//-----------------------------------------------
// Private
//-----------------------------------------------

Clock& Clock::Get()
{
    static Clock instance;
    return instance;
}

//-----------------------------------------------
// Public
//-----------------------------------------------

bool Clock::Init()
{
#if defined(WIN32)
    return QueryPerformanceFrequency(&Get().llFrequency) != FALSE;
#elif defined(__APPLE__)
    return true;
#else
#   error implement me
#endif
}

long long Clock::GetTimeMilli()
{
#if defined(WIN32)
    LARGE_INTEGER llPerf = { 0 };
    // We assert here because we can't do logging in this function since logger uses it to get a timestamp.
    // Attempt to call logger will create infinitive recursion and so it's better to just crash when
    // things go wrong.
    assert(QueryPerformanceCounter(&llPerf));
    return llPerf.QuadPart * 1000ll / (Get().llFrequency.QuadPart / 1000ll);
#elif defined(__APPLE__)
    struct timeval timeVal;
    gettimeofday(&timeVal, NULL);
    return (timeVal.tv_sec * 1000) + (timeVal.tv_usec / 1000);
#else
#   error implement me
#endif
}