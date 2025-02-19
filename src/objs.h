#ifndef OBJS_INCLUDED
#define OBJS_INCLUDED

#include <raylib.h>

/********************************************************
 * Objects are anything that can be drawn in the window
 * The private object list is ordered by draw order for
 * display purposes and selection when interacting with
 * the program.  Each type has its own object file for
 * its creation, updating, drawing, and unloading.
 ********************************************************/
typedef enum {
	STATIC_IMG,
	STATIC_TXT,
	BIT_IMG,
	BIT_TXT,
	REG_IMG,
	REG_TXT,
} ObjType;

typedef struct _object Object;
struct _object {
	int id;
	Rectangle rect;
	ObjType type;
	/**************************************************
	 * pointer to the typed object the type determines
	 * which update/draw/unload function gets called
	 **************************************************/
	bool selected;
	void* obj; 
	Object* next;
};

void load_objects(void);
void unload_objects(void);
void update_objects(void);
void draw_objects(void);
/* New objects are created with the new object window */
int draw_new_object_win(void);
int draw_edit_object_win(void);

#endif // OBJS_INCLUDED
