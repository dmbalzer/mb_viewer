#define UTIL_IMPLEMENTATION
#include "util.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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
} Obj;

static Gif** gifs = NULL;
static SDL_Texture** textures = NULL;
static Obj** objs = NULL;

static void process_drop_file(const char* fn)
{
	if ( util_is_png(fn) ) {
		SDL_Texture* t = sdl_load_png(fn);
		arrput(textures, t);
		SDL_Log("[PNG][%s] Loaded.", fn);
		Obj* o = SDL_calloc(1, sizeof(Obj));
		o->img_type = PNG;
		o->img = (void*)t;	
		arrput(objs, o);
	} else if ( util_is_gif(fn) ) {
		Gif* g = gif_load(fn);
		arrput(gifs, g);
		SDL_Log("[GIF][%s] Loaded.", fn);
		Obj* o = SDL_calloc(1, sizeof(Obj));
		o->img_type = GIF;
		o->img = (void*)g;	
		arrput(objs, o);
	}
}

int main(void)
{
	sdl_init();
	sdl_set_drop_callback(process_drop_file);
	while ( !sdl_quit() ) {
		sdl_process_events();
		for ( int i = 0; i < arrlen(gifs); i++ ) {
			gif_anim(gifs[i]);
		}
		sdl_begin_draw();
		
		for ( int i = 0; i < arrlen(objs); i++ ) {
			switch ( objs[i]->img_type ) {
				case PNG: sdl_blit((SDL_Texture*)objs[i]->img, objs[i]->x, objs[i]->y); break;
				case GIF: gif_blit((Gif*)objs[i]->img, objs[i]->x, objs[i]->y); break;
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
	for ( int i = 0; i < arrlen(objs); i++ ) {
		SDL_free(objs[i]);
	}
	arrfree(objs);
	
	sdl_unload();
	return 0;
}
