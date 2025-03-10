#include <stdlib.h>
#include <SDL3/SDL.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool quit = false;
Uint64 frametime = 0;

void process_drop(const char* filename)
{
}

void draw_textures(void)
{
}

void init(void)
{
	if ( !SDL_Init(SDL_INIT_VIDEO) ) exit(-1);
	if ( !SDL_CreateWindowAndRenderer("Modbus Viewer", 960, 640, SDL_WINDOW_RESIZABLE, &window, &renderer) ) exit(-1);
	SDL_SetRenderVSync(renderer, 1);
}

void unload(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void process_events(void)
{
	SDL_Event ev;
	while ( SDL_PollEvent(&ev) ) {
		switch ( ev.type ) {
			case SDL_EVENT_QUIT: quit = true; break;
			case SDL_EVENT_KEY_DOWN: if ( ev.key.key == SDLK_ESCAPE ) quit = true; break;
			case SDL_EVENT_DROP_FILE: process_drop(ev.drop.data); break;
		}
	}
}

void begin_draw(void)
{
	SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0xFF);
	SDL_RenderClear(renderer);
}

void end_draw(void)
{
	SDL_RenderPresent(renderer);
	static Uint64 prev = 0;
	const Uint64 now = SDL_GetTicks();
	frametime = now - prev;
	prev = now;
}

int main(void)
{
	init();
	while ( !quit ) {
		process_events();
		begin_draw();
		end_draw();
	}
	unload();
	return 0;
}
