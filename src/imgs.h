#ifndef IMGS_INCLUDED
#define IMGS_INCLUDED

typedef struct _img_node ImgNode;
struct _img_node {
	char name[256];
	Image img;
	Texture tex;
	ImgNode* next;
};

static void load_img(const char* fn);
static void unload_imgs(void);
static void draw_img_list(int x, int y);

#endif // IMGS_INCLUDED

#if defined(IMGS_IMPLEMENTATION)
#undef RAYGUI_IMPLEMENTATION
#include <raygui.h>

static ImgNode head = { 0 };
static ImgNode* tail = &head;

static void load_img(const char* fn)
{
	if ( IsFileExtension(fn, ".png") ) {
		ImgNode* in = MemAlloc(sizeof(ImgNode));
		if ( in == 0 ) {
			TraceLog(LOG_ERROR, "Error allocating memory for img.");
			return;
		}
		TextCopy(in->name, GetFileName(fn));
		in->img = LoadImage(fn);
		in->tex = LoadTextureFromImage(in->img);
		tail->next = in;
		tail = in;
	}
}

static void unload_imgs(void)
{
	for ( ImgNode* in = head.next; in != 0; in = in->next ) {
		UnloadTexture(in->tex);
		UnloadImage(in->img);
		MemFree(in);
	}
}

static void draw_img_list(int x, int y)
{
	int count = 0;
	for ( ImgNode* in = head.next; in != 0; in = in->next ) { count++; }
	int w = 120;
	int h = 24 * (count + 1);
	Rectangle bounds = (Rectangle){ x, y, w, h };
	GuiPanel(bounds, "Images");
	bounds.x += 8;
	bounds.width -= 16;
	bounds.y += 24;
	bounds.height = 24;
	for ( ImgNode* in = head.next; in != 0; in = in->next ) {
		GuiLabel(bounds, in->name);
		bounds.y += 24;
	}
}

#endif // IMGS_IMPLEMENTATION
