#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#include "structs.h"
#define UTIL_IMPLEMENTATION
#include "util.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool quit = false;
Uint64 frametime = 0;

SDL_Texture** textures = NULL;
Gif* gifs = NULL;


void process_drop(const char* filename)
{
	if ( is_png(filename) ) {
		SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
		if ( texture != NULL ) arrput(textures, texture);
	} else if ( is_gif(filename) ) {
		Gif gif = { 0 };
		gif.anim = IMG_LoadAnimation(filename);
		if ( gif.anim != NULL )	{
			gif.texture = SDL_CreateTextureFromSurface(renderer, gif.anim->frames[0]);
			arrput(gifs, gif);
			arrput(textures, gif.texture);
		}
	}
}

void draw_textures(void)
{
	for ( int i = 0; i < arrlen(textures); i++ ) {
		SDL_FRect dst = (SDL_FRect){ 0, 0, textures[i]->w, textures[i]->h };
		SDL_RenderTexture(renderer, textures[i], NULL, &dst);
	}
}

void unload_textures(void)
{
	for ( int i = 0; i < arrlen(textures); i++ ) {
		SDL_DestroyTexture(textures[i]);
	}
	arrfree(textures);
}

void unload_gifs(void)
{
	for ( int i = 0; i < arrlen(gifs); i++ ) {
		IMG_FreeAnimation(gifs[i].anim);
	}
	arrfree(gifs);
}

void anim_gifs(void)
{
	for ( int i = 0; i < arrlen(gifs); i++ ) {
		gifs[i].timer += frametime;
		if ( gifs[i].timer >= gifs[i].anim->delays[gifs[i].frame] ) {
			gifs[i].timer = 0;
			gifs[i].frame = (gifs[i].frame + 1) % gifs[i].anim->count;
			SDL_UpdateTexture(gifs[i].texture, NULL, gifs[i].anim->frames[gifs[i].frame]->pixels, gifs[i].anim->frames[gifs[i].frame]->pitch);
		}
	}
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
		anim_gifs();
		begin_draw();
		draw_textures();
		end_draw();
	}
	unload_gifs();
	unload_textures();
	unload();
	return 0;
}
