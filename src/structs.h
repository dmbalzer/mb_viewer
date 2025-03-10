#ifndef STRUCTS_INCLUDED
#define STRUCTS_INCLUDED
#include <SDL3_image/SDL_image.h>

typedef struct {
	SDL_Texture* texture;
	IMG_Animation* anim;
	int frame;
	Uint64 timer;
} Gif;

#endif // STRUCTS_INCLUDED
