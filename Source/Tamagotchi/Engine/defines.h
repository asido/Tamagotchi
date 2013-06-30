#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef WIN32
#    if defined(_DEBUG)
#        define TG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#    else
#        define TG_NEW new
#    endif
#else
#    define TG_NEW new
#endif // WIN32


#if !defined(SAFE_DELETE)
#    define SAFE_DELETE(x)          if (x) delete x; x = NULL;
#endif

#if !defined(SAFE_DELETE_ARRAY)
#    define SAFE_DELETE_ARRAY(x)    if (x) delete[] x; x = NULL;
#endif

#ifndef PATH_MAX
#    define PATH_MAX 1024
#endif

#define B_TO_KB(a)      ((a) / 1024.0f)
#define B_TO_MB(a)      (B_TO_KB(a) / 1024.0f)
#define B_TO_GB(a)      (B_TO_MB(a) / 1024.0f)
#define B_TO_TB(a)      (B_TO_GB(a) / 1024.0f)
#define KB_TO_B(a)      ((a) * 1024.0f)
#define MB_TO_B(a)      (KB_TO_B(a) * 1024.0f)
#define GB_TO_B(a)      (MB_TO_B(a) * 1024.0f)
#define TB_TO_B(a)      (GB_TO_B(a) * 1024.0f)

// Determines the size of statically allocated array.
// the last bit of division is a compile time check in order to make sure the array size is divisible by the size of a single element size.
// If not, the macro will evaluate to division by zero and compilation will fail.
#define ARRAY_SIZE(a)   (sizeof(a) / sizeof(*(a)) / static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

#endif // __COMMON_H__