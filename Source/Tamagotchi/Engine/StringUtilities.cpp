#include <cctype>
#include <cstdarg>
#include <algorithm>
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

std::string &StringUtilities::ToLower(std::string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), std::tolower);
	return str;
}

std::string StringUtilities::Format(const std::string fmt, ...)
{
	int			size = 100;
	std::string str;

	va_list ap;
	while (true)
	{
		str.resize(size);

		va_start(ap, fmt);
		int n = vsnprintf(const_cast<char*>(str.c_str()), size, fmt.c_str(), ap);
		va_end(ap);

		if (n > -1 && n < size)
		{
			str.resize(n);
			return str;
		}

		if (n > -1)
		{
			size = n + 1;
		}
		else
		{
			size *= 2;
		}
	}

	return str;
}

unsigned int StringUtilities::Hash(const std::string &str)
{
	// Relatively simple hash of arbitrary text string into a
	// 32-bit identifier Output value is
	// input-valid-deterministic, but no guarantees are made
	// about the uniqueness of the output per-input
	//
	// Input value is treated as lower-case to cut down on false
	// separations cause by human mistypes. Sure, it could be
	// construed as a programming error to mix up your cases, and
	// it cuts down on permutations, but in Real World Usage
	// making this text case-sensitive will likely just lead to
	// Pain and Suffering.
	//
	// This code lossely based upon the adler32 checksum by Mark
	// Adler and published as part of the zlib compression
	// library sources.

	// largest prime smaller than 65536
	unsigned long BASE = 65521L;

	// NMAX is the largest n such that 255n(n+1)/2 +
	// (n+1)(BASE-1) <= 2^32-1
	unsigned long NMAX = 5552;

	unsigned long s1 = 0;
	unsigned long s2 = 0;

	unsigned int index = 0;

#define DO1(buf, i)	{ s1 += std::tolower(buf[index+i]); s2 += s1; }
#define DO2(buf, i) DO1(buf, i); DO1(buf, i+1);
#define DO4(buf, i) DO2(buf, i); DO2(buf, i+2);
#define DO8(buf, i)	DO4(buf, i); DO4(buf, i+4);
#define DO16(buf)	DO8(buf, 0); DO8(buf, 8);

	for (size_t len = str.length(); len > 0; )
	{
		unsigned long k = (len < NMAX ? len : NMAX);
		len -= k;

		while (k >= 16)
		{
			DO16(str);
			index += 16;
			k -= 16;
		}

		if (k != 0)
		{
			do {
				s1 += std::tolower(str[index++]);
				s2 += s1;
			} while (--k);
		}

		s1 %= BASE;
		s2 %= BASE;
	}

	return (s2 << 16 | s1);

#undef DO1
#undef DO2
#undef DO4
#undef DO8
#undef DO16
}