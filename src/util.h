#ifndef UTIL_INCLUDED
#define UTIL_INCLUDED

#include <stdbool.h>

bool is_png(const char* filename);
bool is_gif(const char* filename);

#endif // UTIL_INCLUDED

#if defined(UTIL_IMPLEMENTATION)

bool is_png(const char* filename)
{
	if ( filename == NULL ) return false;
	size_t len = strlen(filename);
	return   filename[len-4] == '.' &&
			(filename[len-3] == 'p' || filename[len-3] == 'P') &&
			(filename[len-2] == 'n' || filename[len-2] == 'N') &&
			(filename[len-1] == 'g' || filename[len-1] == 'G');
}

bool is_gif(const char* filename)
{
	if ( filename == NULL ) return false;
	size_t len = strlen(filename);
	return   filename[len-4] == '.' &&
			(filename[len-3] == 'g' || filename[len-3] == 'G') &&
			(filename[len-2] == 'i' || filename[len-2] == 'I') &&
			(filename[len-1] == 'f' || filename[len-1] == 'F');
}

#endif // UTIL_IMPLEMENTATION
