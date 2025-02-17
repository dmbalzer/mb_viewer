#ifndef OBJS_INCLUDED
#define OBJS_INCLUDED

#include <raylib.h>

typedef enum {
	STATIC_OBJ=0,
	BOOL_OBJ,
} ObjType;

typedef struct _static_obj StaticObj;
struct _static_obj {
	int img_id;
	Texture* texture;
};

typedef struct _bool_obj BoolObj;
struct _bool_obj {
	bool value;
	int true_img_id;
	int false_img_id;
	Texture* true_texture;
	Texture* false_texture;
};

typedef struct _obj Obj;
struct _obj {
	Vector2 pos;
	ObjType type;
	void* typed_obj;
	Obj* next;
};

void* new_static_obj(int img_id);
void* new_bool_obj(int true_img_id, int false_img_id);
void new_obj(ObjType type, void* typed_obj);
void unload_objs(void);
void draw_objs(void);

#endif // OBJS_INCLUDED
