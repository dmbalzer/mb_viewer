#ifndef SDL_INCLUDED
#define SDL_INCLUDED

#include <SDL3/SDL.h>

void sdl_init(void);
void sdl_process_events(void);
void sdl_begin_draw(void);
void sdl_end_draw(void);
void sdl_unload(void);
bool sdl_quit(void);
SDL_Renderer* sdl_renderer(void);
Uint64 sdl_frametime(void);

#endif // SDL_INCLUDED

#if defined(SDL_IMPLEMENTATION)

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static bool quit = false;
static Uint64 frametime = 0;

void sdl_init(void)
{
	if ( !SDL_Init(SDL_INIT_VIDEO) ) { SDL_Log(SDL_GetError()); exit(1); }
	if ( !SDL_CreateWindowAndRenderer("Modbus Viewer", 960, 640, SDL_WINDOW_RESIZABLE, &window, &renderer) ) { SDL_Log(SDL_GetError()); exit(1); }
	SDL_SetRenderVSync(renderer, 1);
}

void sdl_process_events(void)
{
	SDL_Event ev;
	while ( SDL_PollEvent(&ev) ) {
		switch ( ev.type ) {
			case SDL_EVENT_QUIT: quit = true; break;
			case SDL_EVENT_KEY_DOWN: if ( ev.key.key == SDLK_ESCAPE ) quit = true; break;
		}
	}
}

void sdl_begin_draw(void)
{
	SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0xFF);
	SDL_RenderClear(renderer);
}

void sdl_end_draw(void)
{
	SDL_RenderPresent(renderer);
	static Uint64 prev = 0;
	const Uint64 now = SDL_GetTicks();
	frametime = now - prev;
	prev = now;
}

void sdl_unload(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool sdl_quit(void) { return quit; }

SDL_Renderer* sdl_renderer(void) { return renderer; }

Uint64 sdl_frametime(void) { return frametime; }

#endif // SDL_IMPLEMENTATION
