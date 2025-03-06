#ifndef SDL_INCLUDED
#define SDL_INCLUDED

#include <SDL3/SDL.h>

void sdl_init(void);
void sdl_process_events(void);
void sdl_set_drop_callback(void (*cb)(const char* fn));
void sdl_set_mouse_press_callback(void (*cb)(Uint8 button, SDL_Point pos));
void sdl_set_mouse_motion_callback(void (*cb)(float dx, float dy));
void sdl_begin_draw(void);
void sdl_end_draw(void);
void sdl_unload(void);
bool sdl_quit(void);
SDL_Renderer* sdl_renderer(void);
Uint64 sdl_frametime(void);
void sdl_blit(SDL_Texture* texture, int x, int y);
SDL_Texture* sdl_load_png(const char* fn);

#endif // SDL_INCLUDED

#if defined(SDL_IMPLEMENTATION)

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static bool quit = false;
static Uint64 frametime = 0;
static void (*drop_cb)(const char* fn);
static void (*mpress_cb)(Uint8 button, SDL_Point pos);
static void (*mmotion_cb)(float dx, float dy);

#undef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
			case SDL_EVENT_DROP_FILE: if ( drop_cb != NULL ) drop_cb(ev.drop.data); break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN: if ( mpress_cb != NULL ) mpress_cb(ev.button.button, (SDL_Point){ (int)ev.button.x, (int)ev.button.y }); break;
			case SDL_EVENT_MOUSE_MOTION: if ( mmotion_cb != NULL ) mmotion_cb( ev.motion.xrel, ev.motion.yrel ); break;
		}
	}
}

void sdl_set_drop_callback(void (*cb)(const char* fn)) { drop_cb = cb; }
void sdl_set_mouse_press_callback(void (*cb)(Uint8 button, SDL_Point pos)) { mpress_cb = cb; }
void sdl_set_mouse_motion_callback(void (*cb)(float dx, float dy)) { mmotion_cb = cb; }

void sdl_begin_draw(void)
{
	SDL_SetRenderDrawColor(renderer, 0x50,0x50,0x50,0xFF);
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

void sdl_blit(SDL_Texture* texture, int x, int y)
{
	if ( texture == NULL ) return;
	SDL_FRect dst = (SDL_FRect){ x, y, texture->w, texture->h };
	SDL_RenderTexture(renderer, texture, NULL, &dst);
}

SDL_Texture* sdl_load_png(const char* fn)
{
	int w, h, n;
	stbi_uc* data = stbi_load(fn, &w, &h, &n, 4);
	if ( data == NULL ) return NULL;
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, w, h);
	SDL_UpdateTexture(texture, NULL, (void*)data, w*4);
	stbi_image_free(data);
	return texture;
}

#endif // SDL_IMPLEMENTATION
