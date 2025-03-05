#define UTIL_IMPLEMENTATION
#include "util.h"
#define SDL_IMPLEMENTATION
#include "sdl.h"
#define GIF_IMPLEMENTATION
#include "gif.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef enum {
	PNG=0,
	GIF,
} ComponentType;

typedef struct {
	void* data;
	ComponentType type;
} Component;

typedef struct {
	int x;
	int y;
	Component** comps;
} Entity;

static SDL_Texture** texs = NULL;
static Gif** gifs = NULL;
static Entity** ents = NULL;

static void process_drop_file(const char* fn)
{
	Component* c = NULL;
	if ( util_is_png(fn) ) {
		SDL_Texture* t = sdl_load_png(fn);
		if ( t == NULL ) return;
		SDL_Log("[PNG][%s] Loaded.", fn);
		arrput(texs, t);
		c = SDL_calloc(1, sizeof(Component));
		if ( c == NULL ) return;
		c->data = (void*)t;
		c->type = PNG;
	} else if ( util_is_gif(fn) ) {
		Gif* g = gif_load(fn);
		if ( g == NULL ) return;
		SDL_Log("[GIF][%s] Loaded.", fn);
		arrput(gifs, g);
		c = SDL_calloc(1, sizeof(Component));
		if ( c == NULL ) return;
		c->data = (void*)g;
		c->type = GIF;
	}
	
	if ( c == NULL ) return;
	Entity* e = SDL_calloc(1, sizeof(Entity));
	if ( e == NULL ) { SDL_free(c); return; }

	arrput(e->comps, c);
	arrput(ents, e);
}

int main(void)
{
	sdl_init();
	sdl_set_drop_callback(process_drop_file);
	while ( !sdl_quit() ) {
		sdl_process_events();
		
		/* animate gifs */
		for ( int i = 0; i < arrlen(gifs); i++ ) {
			gif_anim(gifs[i]);
		}
		
		sdl_begin_draw();
		
		/* draw objects */
		for ( int i = 0; i < arrlen(ents); i++ ) {
			for ( int j = 0; j < arrlen(ents[i]->comps); j++ ) {
				Component* c = ents[i]->comps[j];
				switch ( c->type ) {
					case PNG: sdl_blit((SDL_Texture*)c->data, ents[i]->x, ents[i]->y); break;
					case GIF: gif_blit((Gif*)c->data, ents[i]->x, ents[i]->y); break;
				}
			}
		}
		sdl_end_draw();
	}
	
	for ( int i = 0; i < arrlen(gifs); i++ ) {
		gif_destroy(gifs[i]);
	}
	arrfree(gifs);

	for ( int i = 0; i < arrlen(texs); i++ ) {
		SDL_DestroyTexture(texs[i]);
	}
	arrfree(texs);

	for ( int i = 0; i < arrlen(ents); i++ ) {
		for ( int j = 0; j < arrlen(ents[i]->comps); j++ ) {
			SDL_free(ents[i]->comps[j]);
			arrfree(ents[i]->comps);
		}		
		SDL_free(ents[i]);
	}
	arrfree(ents);
	
	sdl_unload();
	return 0;
}
