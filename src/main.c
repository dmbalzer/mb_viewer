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
	bool selected;
} Entity;

static SDL_Texture** texs = NULL;
static Gif** gifs = NULL;
static Entity** ents = NULL;

static void get_comp_width(Component* c, int* w, int* h)
{
	switch ( c->type ) {
		case PNG:
			SDL_Texture* t = (SDL_Texture*)c->data;
			*w = t->w;
			*h = t->h;
		break;
		case GIF:
			Gif* g = (Gif*)c->data;
			*w = g->w;
			*h = g->h;
		break;
	}
}

static void process_drop_file(const char* fn)
{
	Component* c = NULL;
	/* PNG drop */
	if ( util_is_png(fn) ) {
		SDL_Texture* t = sdl_load_png(fn);
		if ( t == NULL ) return;
		SDL_Log("[PNG][%s] Loaded.", fn);
		arrput(texs, t);
		c = SDL_calloc(1, sizeof(Component));
		if ( c == NULL ) return;
		c->data = (void*)t;
		c->type = PNG;
	/* GIF drop */
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

static Entity* get_top_entity_in_point(SDL_Point pos)
{
	Entity* e = NULL;
	for ( int i = 0; i < arrlen(ents); i++ ) {
		int w = 0;
		int h = 0;
		for ( int j = 0; j < arrlen(ents[i]->comps); j++ ) {
			get_comp_width(ents[i]->comps[j], &w, &h);
			if ( w != 0 && h != 0 ) {
				SDL_Rect rec = (SDL_Rect){ ents[i]->x, ents[i]->y, w, h };
				if ( SDL_PointInRect(&pos, &rec) ) {
					e = ents[i];
				}
				break;
			}
		}
	}
	return e;
}

static void check_entity_select(SDL_Point pos)
{
	Entity* e = get_top_entity_in_point(pos);
	if ( e != NULL ) e->selected = true;
}

static void mouse_press(Uint8 button, SDL_Point pos)
{
	if ( SDL_BUTTON_MASK(button) & SDL_BUTTON_LMASK ) check_entity_select(pos);
	if ( SDL_BUTTON_MASK(button) & SDL_BUTTON_RMASK ) for ( int i = 0; i < arrlen(ents); i++ ) ents[i]->selected = false;
}

static void mouse_motion(float dx, float dy)
{
	SDL_FPoint pos = { 0 };
	if ( !(SDL_BUTTON_LMASK & SDL_GetMouseState(&pos.x, &pos.y)) ) return;
	if ( get_top_entity_in_point((SDL_Point){ pos.x, pos.y }) == NULL ) return;
	for ( int i = 0; i < arrlen(ents); i++ ) {
		if ( ents[i]->selected ) {
			ents[i]->x += (int)dx;
			ents[i]->y += (int)dy;
		}
	}
}

int main(void)
{
	sdl_init();
	sdl_set_drop_callback(process_drop_file);
	sdl_set_mouse_press_callback(mouse_press);
	sdl_set_mouse_motion_callback(mouse_motion);

	while ( !sdl_quit() ) {
		sdl_process_events();
		
		/* animate gifs */
		for ( int i = 0; i < arrlen(gifs); i++ ) {
			gif_anim(gifs[i]);
		}
		
		sdl_begin_draw();
		
		/* draw entities */
		for ( int i = 0; i < arrlen(ents); i++ ) {
			for ( int j = 0; j < arrlen(ents[i]->comps); j++ ) {
				Component* c = ents[i]->comps[j];
				switch ( c->type ) {
					case PNG: sdl_blit((SDL_Texture*)c->data, ents[i]->x, ents[i]->y); break;
					case GIF: gif_blit((Gif*)c->data, ents[i]->x, ents[i]->y); break;
				}
			}
			if ( ents[i]->selected ) {
				int w = 0;
				int h = 0;
				for ( int j = 0; j < arrlen(ents[i]->comps); j++ ) {
					get_comp_width(ents[i]->comps[j], &w, &h);
					if ( w != 0 && h != 0 ) break;
				}
				if ( w != 0 && h != 0 ) {
					SDL_SetRenderDrawColor(sdl_renderer(), 0xFF,0xFF,0xFF,0xFF);
					SDL_FRect rec = (SDL_FRect){ ents[i]->x, ents[i]->y, w, h };
					SDL_RenderRect(sdl_renderer(), &rec);
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
