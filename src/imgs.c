#include "imgs.h"
#undef RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <stdlib.h>

static int id = 0;
static ImgNode head = { 0 };
static ImgNode* tail = &head;

void load_img(const char* fn)
{
	if ( IsFileExtension(fn, ".png") ) {
		ImgNode* img = MemAlloc(sizeof(ImgNode));
		if ( img == 0 ) {
			TraceLog(LOG_ERROR, "Error allocatimgg memory for img.");
			return;
		}
		img->id = id++;
		TextCopy(img->name, GetFileName(fn));
		img->img = LoadImage(fn);
		img->texture = LoadTextureFromImage(img->img);
		tail->next = img;
		tail = img;
	}
}

void unload_imgs(void)
{
	for ( ImgNode* img = head.next; img != 0; img = img->next ) {
		UnloadTexture(img->texture);
		UnloadImage(img->img);
		int _id = img->id;
		MemFree(img);
		TraceLog(LOG_INFO, TextFormat("img [ID %d] unloaded.", _id));
	}
}

int draw_img_list(int x, int y)
{
	int result = 0;
	char buff[1024] = { 0 };
	int cursor = 0;
	for ( ImgNode* img = head.next; img != 0; img = img->next ) {
		TextAppend(buff, img->name, &cursor);
		TextAppend(buff, ";", &cursor);
	}
	buff[cursor - 1] = '\0';
	Rectangle bounds = (Rectangle){ x, y, 120, 200 };
	result = GuiWindowBox(bounds, "Images");
	bounds.y += 24;
	bounds.height -= 24;
	static int scroll_idx = 0;
	static int active = 0;
	GuiListView(bounds, buff, &scroll_idx, &active);
	return result;
}

