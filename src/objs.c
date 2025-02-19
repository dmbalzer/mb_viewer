#include "objs.h"
#include "defs.h"
#undef RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <stdlib.h>

static int id = 0;
static Object head = { 0 };
static Object* tail = &head;

static const char* type_names = "Static Image;Static Text;Bit Image;Bit Text;Register Image;Register Text";
static const Rectangle default_obj_rect = (Rectangle){ 0, 0, 64, 64 };

static void _new_object(ObjType type)
{
	Object* o = MemAlloc(sizeof(Object));
	if ( o == NULL ) { TraceLog(LOG_ERROR, "Error creating object."); return; }
	o->id = id++;
	o->type = type;
	o->rect = default_obj_rect;
	o->rect.x = GetScreenWidth()/2 - o->rect.width/2;
	o->rect.y = GetScreenHeight()/2 - o->rect.height/2;
	switch ( type ) {
		case STATIC_IMG: break;
		case STATIC_TXT: break;
		case BIT_IMG: break;
		case BIT_TXT: break;
		case REG_IMG: break;
		case REG_TXT: break;
	}
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

static void _move_objects(void)
{
	static Object* move_obj = NULL;
	if ( IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ) {
		for ( Object* o = head.next; o != NULL; o = o->next ) {
			if ( CheckCollisionPointRec(GetMousePosition(), o->rect) ) {
				move_obj = o;
			}
		}
	}
	
	if ( IsMouseButtonReleased(MOUSE_BUTTON_LEFT) ) move_obj = NULL;
	
	if ( move_obj != NULL ) {
		move_obj->rect.x += GetMouseDelta().x;
		move_obj->rect.y += GetMouseDelta().y;
	}
}

void update_objects(void)
{
	_move_objects();
}

void draw_objects(void)
{
	for ( Object* o = head.next; o != NULL; o = o->next ) {
		if ( o->obj == NULL ) GuiDummyRec(o->rect, TextFormat("Object %d", o->id));
	}
}

int draw_new_object_win(void)
{
	int result = 0;
	int x = GetScreenWidth()/2 - STD_WIN_W/2;
	int y = GetScreenHeight()/2 - STD_WIN_H/2;
	/* Window Box */
	Rectangle bounds = (Rectangle){ x, y, STD_WIN_W, STD_WIN_H };
	if ( GuiWindowBox(bounds, "New Object") ) result = 1;
	
	/* Selection List */
	static int scroll_idx = 0;
	static int active = 0;
	Rectangle bounds_list = (Rectangle){
		bounds.x + PAD,
		bounds.y + 24 + PAD,
		bounds.width - PAD*2,
		(bounds.height - 24 - PAD*2) - (STD_H * 2) - (PAD * 2)};
	GuiListView(bounds_list, type_names, &scroll_idx, &active);
	
	/* Submit Button */
	Rectangle bounds_btn = (Rectangle){
		bounds.x + bounds.width/2 - STD_W/2,
		bounds.y + bounds.height - STD_H - PAD,
		STD_W,
		STD_H};
	if ( GuiButton(bounds_btn, "Submit") ) {
		result = 1;
		_new_object(active);
	}
	
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