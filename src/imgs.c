#include "imgs.h"
#include "defs.h"
#undef RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <stdlib.h>

static int id = 0;
static Img head = { 0 };
static Img* tail = &head;

static void _new_img(void)
{
	Img* o = MemAlloc(sizeof(Img));
	if ( o == NULL ) { TraceLog(LOG_ERROR, "Error creating img."); return; }
	o->id = id++;
	tail->next = o;
	tail = o;
}

void load_imgs(void)
{
	
}

void unload_imgs(void)
{
	Img* o = head.next;
	while ( o != NULL ) {
		int _id = o->id;
		Img* tmp = o;
		o = o->next;
		free(tmp);
		TraceLog(LOG_INFO, TextFormat("IMG [ID %d] unloaded.", _id));
	}
}

void update_imgs(void)
{
	for ( Img* o = head.next; o != NULL; o = o->next ) {
		
	}
}

void process_dropped_imgs(const char* path)
{
	
}

int draw_img_list_win(void)
{
	GuiEnable();
	int result = 0;
	int x = GetScreenWidth()/2 - STD_WIN_W/2;
	int y = GetScreenHeight()/2 - STD_WIN_H/2;
	Rectangle bounds = (Rectangle){ x, y, STD_WIN_W, STD_WIN_H };
	if ( GuiWindowBox(bounds, "Image List") ) result = 1;
	GuiDisable();
	return result;
}
