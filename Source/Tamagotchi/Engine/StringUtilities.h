#ifndef __STRINGUTILITIES_H__
#define __STRINGUTILITIES_H__

#include <string>

//-----------------------------------------------------------------------------------------------------------
//  class StringUtilities
//-----------------------------------------------------------------------------------------------------------

class StringUtilities
{
public:
    static bool             WildcardMatch(const std::string &pattern, const std::string &string);
    static std::string&     ToLower(std::string &str);
    static std::string      Format(const std::string fmt, ...);
    static unsigned int     Hash(const std::string &str);
};

#endif // __STRINGUTILITIES_H__