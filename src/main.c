#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <string.h>
#include "imgs.h"
#include "objs.h"

#define STATE_QTY 4
typedef enum {
	QUIT,
	NEW_OBJ,
	EDIT_OBJ,
	IMG_LIST,
} States;

bool state[4] = { 0 };

void draw_main_menu(void);

int main(void)
{
	InitWindow(960, 640, "Modbus Viewer");
	SetTargetFPS(30);
	SetExitKey(KEY_NULL);

	while ( !state[QUIT] ) {
		if ( WindowShouldClose() ) state[QUIT]= true;
		
		if ( IsFileDropped() ) {
			FilePathList fpl = LoadDroppedFiles();
			for ( int i = 0; i < fpl.count; i++ ) load_img(fpl.paths[i]);
			UnloadDroppedFiles(fpl);
		}
		
		BeginDrawing();
		ClearBackground(WHITE);
		draw_objs();
		if ( state[NEW_OBJ] ) state[NEW_OBJ]   = !draw_new_obj();
		if ( state[IMG_LIST] ) state[IMG_LIST] = !draw_img_list();
		draw_main_menu();
		EndDrawing();
	}
	
	unload_objs();	
	unload_imgs();	
	CloseWindow();
	return 0;
}

void draw_main_menu(void)
{
	Rectangle bounds = (Rectangle){ 0, 0, GetScreenWidth(), 28 };
	GuiPanel(bounds, NULL);
	Rectangle btn_bounds = (Rectangle){ 4, 4, 80, 20 };
	if ( GuiButton(btn_bounds, "New Object") )  { memset(state, 0, sizeof(bool)*STATE_QTY); state[NEW_OBJ] = true; }
	btn_bounds.x += btn_bounds.width + 4;
	if ( GuiButton(btn_bounds, "Edit Object") ) { memset(state, 0, sizeof(bool)*STATE_QTY); state[EDIT_OBJ] = true; }
	btn_bounds.x += btn_bounds.width + 4;
	if ( GuiButton(btn_bounds, "Image List") )  { memset(state, 0, sizeof(bool)*STATE_QTY); state[IMG_LIST] = true; }
	btn_bounds.x = GetScreenWidth()-btn_bounds.width-4;
	if ( GuiButton(btn_bounds, "Quit") )        { memset(state, 0, sizeof(bool)*STATE_QTY); state[QUIT] = true; }
}
