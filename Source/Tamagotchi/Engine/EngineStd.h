// EngineStd.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef __ENGINESTD_H__
#define __ENGINESTD_H__

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>

#ifdef _WIN32
#	if defined(_DEBUG)
#		define T_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#	else
#		define T_NEW new
#	endif
#else
#	define T_NEW new
#endif // _WIN32


#if !defined(SAFE_DELETE)
#	define SAFE_DELETE(x)	if (x) delete x; x = NULL;
#endif

#if !defined(SAFE_DELETE_ARRAY)
#	define SAFE_DELETE_ARRAY(x)	if (x) delete[] x; x = NULL;
#endif

#endif // __ENGINESTD_H__