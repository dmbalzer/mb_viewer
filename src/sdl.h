#ifndef SDL_INCLUDED
#define SDL_INCLUDED

#include <SDL3/SDL.h>

typedef struct {
	SDL_Texture* texture;
	int w;
	int h;
	int qty;
	int frame;
	Uint64 delay;
	Uint64 timer;
} Gif;

Gif* sdl_load_gif(const char* fn);
void sdl_destroy_gif(Gif* gif);
void sdl_anim_gif(Gif* gif);
void sdl_blit_gif(Gif* gif, int x, int y);

void sdl_init(void);
void sdl_process_events(void);
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

#undef UTIL_IMPLEMENTATION
#include "util.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static bool quit = false;
static Uint64 frametime = 0;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

static SDL_Texture** textures = NULL;
static Gif** gifs = NULL;

static void sdl__process_drop_file(SDL_DropEvent drop)
{
	if ( util_is_png(drop.data) ) {
		arrput(textures, sdl_load_png(drop.data));
		SDL_Log("[PNG][%s] Loaded.", drop.data);
	} else if ( util_is_gif(drop.data) ) {
		arrput(gifs, sdl_load_gif(drop.data));
		SDL_Log("[GIF][%s] Loaded.", drop.data);
	}
}

Gif* sdl_load_gif(const char* fn)
{
	if ( fn == NULL ) return NULL;
	size_t len;
	unsigned char* buff = SDL_LoadFile(fn, &len);
	if ( buff == NULL ) return NULL;
	int w,h,q,n;
	int* d = NULL;
	stbi_uc* data = stbi_load_gif_from_memory(buff, len, &d, &w, &h, &q, &n, 4);
	if ( data == NULL ) { SDL_free(buff); SDL_free(d); return NULL; }
	if ( d[0] == 0 ) d[0] = 100;
	Gif* gif = SDL_calloc(1, sizeof(Gif));
	if ( gif == NULL ) { SDL_free(buff); SDL_free(d); stbi_image_free(data); return NULL; }
	gif->w = w;
	gif->h = h;
	gif->qty = q;
	SDL_Log("%d", q);
	gif->delay = d[0];
	gif->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, w*q, h);
	for ( int i = 0; i < q; i++ ) {
		SDL_Rect src = (SDL_Rect){w*i, 0, w, h};
		SDL_UpdateTexture(gif->texture, &src, (void*)(data + w*h*4*i), w*4);
	}
	SDL_free(buff);
	SDL_free(d);
	stbi_image_free(data);
	return gif;
}

void sdl_destroy_gif(Gif* gif)
{
	if ( gif == NULL ) return;
	SDL_DestroyTexture(gif->texture);
	SDL_free(gif);
}

void sdl_anim_gif(Gif* gif)
{
	gif->timer += frametime;
	if ( gif->timer >= gif->delay ) {
		gif->timer = 0;
		gif->frame = ( gif->frame + 1 ) % gif->qty;
	}
}

void sdl_blit_gif(Gif* gif, int x, int y)
{
	if ( gif == NULL ) return;
	if ( gif->texture == NULL ) return;
	SDL_FRect src = (SDL_FRect){ gif->w*gif->frame, 0, gif->w, gif->h };
	SDL_FRect dst = (SDL_FRect){ x, y, gif->w, gif->h };
	SDL_RenderTexture(renderer, gif->texture, &src, &dst);
}

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
			case SDL_EVENT_DROP_FILE: sdl__process_drop_file(ev.drop); break;
		}
	}
}

void sdl_begin_draw(void)
{
	for ( int i = 0; i < arrlen(gifs); i++ ) {
		sdl_anim_gif(gifs[i]);
	}
	SDL_SetRenderDrawColor(renderer, 0x50,0x50,0x50,0xFF);
	SDL_RenderClear(renderer);
	for ( int i = 0; i < arrlen(textures); i++ ) {
		sdl_blit(textures[i], 0, 0);
	}
	for ( int i = 0; i < arrlen(gifs); i++ ) {
		sdl_blit_gif(gifs[i], 0, 0);
	}
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
	for ( int i = 0; i < arrlen(gifs); i++ ) {
		sdl_destroy_gif(gifs[i]);
	}
	arrfree(gifs);
	for ( int i = 0; i < arrlen(textures); i++ ) {
		SDL_DestroyTexture(textures[i]);
	}
	arrfree(textures);
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
