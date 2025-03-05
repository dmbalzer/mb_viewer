#define UTIL_IMPLEMENTATION
#include "util.h"
#define SDL_IMPLEMENTATION
#include "sdl.h"

int main(void)
{
	sdl_init();
	while ( !sdl_quit() ) {
		sdl_process_events();
		sdl_begin_draw();
		sdl_end_draw();
	}
	sdl_unload();
	return 0;
}
