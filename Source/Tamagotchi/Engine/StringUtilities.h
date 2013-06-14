#ifndef __STRINGUTILITIES_H__
#define __STRINGUTILITIES_H__

#include <string>

class StringUtilities
{
public:
	static bool			WildcardMatch(const std::string &pattern, const std::string &string);
	static std::string &ToLower(std::string &str);
	static std::string	Format(const std::string fmt, ...);
};

#endif // __STRINGUTILITIES_H__