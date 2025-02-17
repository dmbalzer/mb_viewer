#include "objs.h"
#undef RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <stdlib.h>

static int id = 0;
static Obj head = { 0 };
static Obj* tail = &head;

static void* _new_static_obj(void)
{
	return NULL;
}

static void* _new_bool_obj(void)
{
	return NULL;
}

void new_obj(ObjType type)
{
	Obj* obj = MemAlloc(sizeof(Obj));
	if ( obj == NULL ) {
		TraceLog(LOG_ERROR, "Error allocating memory for object.");
		return;
	}
	obj->id = id++;
	switch ( type ) {
		case STATIC_OBJ: obj->typed_obj = _new_static_obj();
		case BOOL_OBJ:   obj->typed_obj = _new_bool_obj();
	}
	tail->next = obj;
	tail = obj;
}

int draw_new_obj(void)
{
	int result = 0;
	int w = 200;
	int h = 200;

	/* Window Box */	
	Rectangle bounds = (Rectangle){ GetScreenWidth()/2 - w/2, GetScreenHeight()/2 - h/2, w, h };
	if ( GuiWindowBox(bounds, "New Object") ) result = 1;

	/* Type list */
	Rectangle list_bounds = (Rectangle){ bounds.x + 8, bounds.y + 32, bounds.width - 16, bounds.height - 16 - 56 };
	static int scroll_idx = 0;
	static int active = 0;
	const char* items = "Static Image;Bool Image";
	GuiListView(list_bounds, items, &scroll_idx, &active);	

	/* Submit Button */
	Rectangle btn_bounds = (Rectangle){ bounds.x + bounds.width/2 - 40, bounds.y + bounds.height - 24 - 8, 80, 24 };
	if ( GuiButton(btn_bounds, "Submit") ) {
		result = 1;
		new_obj(active);
	}

	return result;
}

void unload_objs(void)
{
	for ( Obj* obj = head.next; obj != 0; obj = obj->next ) {
		MemFree(obj->typed_obj);
		int _id = obj->id;
		MemFree(obj);
		TraceLog(LOG_INFO, TextFormat("obj [ID %d] unloaded.", _id));
	}
}

void draw_objs(void)
{
}
