#include "StringUtilities.h"

// The following function was found on http://xoomer.virgilio.it/acantato/dev/wildcard/wildmatch.html,
// where it was attributed to the C/C++ Users Journal, written by Mike Cornelison. It is a little ugly,
// but it is FAST. Use this as an excercise in not reinventing the wheel, even if you see gotos. 
bool StringUtilities::WildcardMatch(const std::string &pattern, const std::string &string)
{
	int		i;
	bool	star;

	const char *pat = pattern.c_str();
	const char *str = string.c_str();

new_segment:

	star = false;
	if (*pat == '*')
	{
		star = true;
		
		do {
			pat++;
		} while (*pat == '*');
	}

test_match:

	for (i = 0; pat[i] && (pat[i] != '*'); ++i)
	{
		if (str[i] != pat[i])
		{
			if (!str[i])							return 0;
			if ((pat[i] == '?') && (str[i] != '.'))	continue;
			if (!star)								return 0;
			str++;
			goto test_match;
		}
	}

	if (pat[i] == '*')
	{
		str += i;
		pat += i;
		goto new_segment;
	}

	if (!str[i])				return true;
	if (i && pat[i-1] == '*')	return true;
	if (!star)					return false;
	str++;
	goto test_match;

	return true;
}

char* StringUtilities::ToLower(char *c)
{
    if (c == NULL)
    {
        return NULL;
    }
    
    char *str = c;
    
    while (*str)
    {
        *str = std::tolower(*str);
        ++str;
    }
    
    return c;
}