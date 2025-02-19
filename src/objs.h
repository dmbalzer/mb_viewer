#ifndef OBJS_INCLUDED
#define OBJS_INCLUDED

#include <raylib.h>

typedef enum {
	STATIC_IMG,
	STATIC_TXT,
	BIT_IMG,
	BIT_TXT,
	REG_IMG,
	REG_TXT,
	MB_CTX,
} ObjType;

typedef struct _object Object;
struct _object {
	Vector2 position;
	ObjType type;
	void* obj;
	Object* next;
};

#endif // OBJS_INCLUDED
