#ifndef IMGS_INCLUDED
#define IMGS_INCLUDED

typedef struct _img_node ImgNode;
struct _img_node {
	char name[256];
	Image img;
	Texture tex;
	ImgNode* next;
};

void load_img(const char* fn);
void unload_imgs(void);
void draw_img_list(int x, int y);

#endif // IMGS_INCLUDED

