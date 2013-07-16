#ifndef __CLOCK_H__
#define __CLOCK_H__

#if defined(WIN32)
#   include <windows.h>
#endif

//-----------------------------------------------------------------------------------------------------------
//  class Clock
//-----------------------------------------------------------------------------------------------------------

class Clock
{
public:
    static bool Init();

    // Don't try to convert it to calendar time as Windows and POSIX return milliseconds
    // based on different reference point. This function is suitable to calculate the
    // elapsed milliseconds.
    static long long GetTimeMilli();

private:
    static Clock& Get();

private:
#if defined(WIN32)
    LARGE_INTEGER llFrequency;
#endif
};

#endif // __CLOCK_H__