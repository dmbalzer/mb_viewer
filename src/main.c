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
} ImageType;

typedef struct {
	int x;
	int y;
	ImageType img_type;
	void* img;
} Entity;

static Gif** gifs = NULL;
static SDL_Texture** textures = NULL;
static Entity** ents = NULL;

static void process_drop_file(const char* fn)
{
	if ( util_is_png(fn) ) {
		SDL_Texture* t = sdl_load_png(fn);
		arrput(textures, t);
		SDL_Log("[PNG][%s] Loaded.", fn);
		Entity* e = SDL_calloc(1, sizeof(Entity));
		e->img_type = PNG;
		e->img = (void*)t;	
		arrput(ents, e);
	} else if ( util_is_gif(fn) ) {
		Gif* g = gif_load(fn);
		arrput(gifs, g);
		SDL_Log("[GIF][%s] Loaded.", fn);
		Entity* e = SDL_calloc(1, sizeof(Entity));
		e->img_type = GIF;
		e->img = (void*)g;	
		arrput(ents, e);
	}
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
			switch ( ents[i]->img_type ) {
				case PNG: sdl_blit((SDL_Texture*)ents[i]->img, ents[i]->x, ents[i]->y); break;
				case GIF: gif_blit((Gif*)ents[i]->img, ents[i]->x, ents[i]->y); break;
			}
		}
		
		sdl_end_draw();
	}
	
	for ( int i = 0; i < arrlen(gifs); i++ ) {
		gif_destroy(gifs[i]);
	}
	arrfree(gifs);
	for ( int i = 0; i < arrlen(textures); i++ ) {
		SDL_DestroyTexture(textures[i]);
	}
	arrfree(textures);
	for ( int i = 0; i < arrlen(ents); i++ ) {
		SDL_free(ents[i]);
	}
	arrfree(ents);
	
	sdl_unload();
	return 0;
}
