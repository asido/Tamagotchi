#ifndef __STRINGUTILITIES_H__
#define __STRINGUTILITIES_H__

#include "common.h"

class StringUtilities
{
public:
	static bool WildcardMatch(const std::string &pattern, const std::string &string);
};

#endif // __STRINGUTILITIES_H__