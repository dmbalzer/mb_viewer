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
	int frame;
	float delay;
	float timer;
	Img* next;
};

void unload_imgs(void);
void update_imgs(void);
/* New imgs are created with the new object window */
void process_dropped_imgs(FilePathList fpl);
int draw_img_list_win(void);

#endif // IMGS_INCLUDED

