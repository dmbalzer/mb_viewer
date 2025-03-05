#ifndef UTIL_INCLUDED
#define UTIL_INCLUDED

#include <stdbool.h>

bool util_is_png(const char* fn);
bool util_is_gif(const char* fn);

#endif // UTIL_INCLUDED

#if defined(UTIL_IMPLEMENTATION)

#include <stddef.h>
#include <string.h>

bool util_is_png(const char* fn)
{
	if ( fn == NULL ) return false;
	bool result = false;
	
	int len = strlen(fn);
	if (  fn[len - 4] == '.' &&
		 (fn[len - 3] == 'p' || fn[len - 3] == 'P') &&
		 (fn[len - 2] == 'n' || fn[len - 2] == 'N') &&
		 (fn[len - 1] == 'g' || fn[len - 1] == 'G') )
		result = true;

	return result;	
}

bool util_is_gif(const char* fn)
{
	if ( fn == NULL ) return false;
	bool result = false;
	
	int len = strlen(fn);
	if (  fn[len - 4] == '.' &&
		 (fn[len - 3] == 'g' || fn[len - 3] == 'G') &&
		 (fn[len - 2] == 'i' || fn[len - 2] == 'I') &&
		 (fn[len - 1] == 'f' || fn[len - 1] == 'F') )
		result = true;

	return result;	
}

#endif // UTIL_IMPLEMENTATION
