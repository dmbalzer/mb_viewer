#include "objs.h"
#include "defs.h"
#undef RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <stdlib.h>

static int id = 0;
static Object head = { 0 };
static Object* tail = &head;

static void _new_object(void)
{
	Object* o = MemAlloc(sizeof(Object));
	if ( o == NULL ) { TraceLog(LOG_ERROR, "Error creating object."); return; }
	o->id = id++;
	tail->next = o;
	tail = o;
}

void load_objects(void)
{
	
}

void unload_objects(void)
{
	Object* o = head.next;
	while ( o != NULL ) {
		int _id = o->id;
		Object* tmp = o;
		o = o->next;
		free(tmp);
		TraceLog(LOG_INFO, TextFormat("OBJECT [ID %d] unloaded.", _id));
	}
}

void update_objects(void)
{
	for ( Object* o = head.next; o != NULL; o = o->next ) {
		
	}
}

void draw_objects(void)
{
	for ( Object* o = head.next; o != NULL; o = o->next ) {
		
	}
}

int draw_new_object_win(void)
{
	int result = 0;
	int x = GetScreenWidth()/2 - STD_WIN_W/2;
	int y = GetScreenHeight()/2 - STD_WIN_H/2;
	Rectangle bounds = (Rectangle){ x, y, STD_WIN_W, STD_WIN_H };
	if ( GuiWindowBox(bounds, "New Object") ) result = 1;
	return result;
}

int draw_edit_object_win(void)
{
	int result = 0;
	int x = GetScreenWidth()/2 - STD_WIN_W/2;
	int y = GetScreenHeight()/2 - STD_WIN_H/2;
	Rectangle bounds = (Rectangle){ x, y, STD_WIN_W, STD_WIN_H };
	if ( GuiWindowBox(bounds, "Edit Object") ) result = 1;
	return result;
}