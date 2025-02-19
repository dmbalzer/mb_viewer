#ifndef IMGS_INCLUDED
#define IMGS_INCLUDED

#include <raylib.h>

/**********************************************************
 * Imgs are wrapper structs for raylib Images and Textures.
 * They are created by dropping a '.png' '.jpg' or '.gif'
 * file into the application window.  They are tracked in
 * their own linked list.
 **********************************************************/

 /* TODO: implement rres resource creation for saving the
          dropped images for later use. */

typedef struct _img Img;
struct _img {
	int id;
	char name[256];
	Image image;
	Texture texture;
	int count;		/* Used only for gifs in update */
	int frame;		/* Used only for gifs in update */
	float delay;	/* Used only for gifs in update */
	float timer;	/* Used only for gifs in update */
	Img* next;
};

void load_imgs(void);
void unload_imgs(void);
void update_imgs(void); /* animating gif imgs */
void process_dropped_imgs(const char* path);
int draw_img_list_win(void);

#endif // IMGS_INCLUDED

