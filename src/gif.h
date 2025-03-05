#ifndef GIF_INCLUDED
#define GIF_INCLUDED

typedef struct {
	SDL_Texture* texture;
	int w;
	int h;
	int qty;
	int frame;
	Uint64 delay;
	Uint64 timer;
} Gif;

Gif* gif_load(const char* fn);
void gif_destroy(Gif* gif);
void gif_anim(Gif* gif);
void gif_blit(Gif* gif, int x, int y);

#endif // GIF_INCLUDED

#if defined(GIF_IMPLEMENTATION)

#include <SDL3/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

extern SDL_Renderer* renderer;
extern Uint64 frametime;

Gif* gif_load(const char* fn)

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

void gif_destroy(Gif* gif)
{
	if ( gif == NULL ) return;
	SDL_DestroyTexture(gif->texture);
	SDL_free(gif);
}

void gif_anim(Gif* gif)
{
	gif->timer += frametime;
	if ( gif->timer >= gif->delay ) {
		gif->timer = 0;
		gif->frame = ( gif->frame + 1 ) % gif->qty;
	}
}

void gif_blit(Gif* gif, int x, int y)
{
	if ( gif == NULL ) return;
	if ( gif->texture == NULL ) return;
	SDL_FRect src = (SDL_FRect){ gif->w*gif->frame, 0, gif->w, gif->h };
	SDL_FRect dst = (SDL_FRect){ x, y, gif->w, gif->h };
	SDL_RenderTexture(renderer, gif->texture, &src, &dst);
}

#endif // GIF_IMPLEMENTATION
