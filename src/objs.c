#include "objs.h"
#include <stdlib.h>

static int id = 0;
static Obj head = { 0 };
static Obj* tail = &head;

void* new_static_obj(int img_id)
{
	return NULL;
}

void* new_bool_obj(int true_img_id, int false_img_id)
{
	return NULL;
}

void new_obj(ObjType type, void* typed_obj)
{
}

void unload_objs(void)
{
	for ( Obj* obj = head.next; obj != 0; obj = obj->next ) {
		MemFree(obj->typed_obj);
		MemFree(obj);
	}
}

void draw_objs(void)
{
}
