#include "imgs.h"
#undef RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <stdlib.h>

static ImgNode head = { 0 };
static ImgNode* tail = &head;

void load_img(const char* fn)
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

void unload_imgs(void)
{
	for ( ImgNode* in = head.next; in != 0; in = in->next ) {
		UnloadTexture(in->tex);
		UnloadImage(in->img);
		MemFree(in);
	}
}

int draw_img_list(int x, int y)
{
	int result = 0;
	char buff[1024] = { 0 };
	int cursor = 0;
	for ( ImgNode* in = head.next; in != 0; in = in->next ) {
		TextAppend(buff, in->name, &cursor);
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

