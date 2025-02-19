#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "defs.h"
#include "objs.h"
#include "imgs.h"

#define STATE_QTY 2
typedef enum {
	RUN,
	NEW_OBJ,
	EDIT_OBJ,
	IMG_LIST,
	QUIT,
} State;

void process_file_drop(void);
void draw_main_menu(void);

static State state = RUN;

int main(void)
{
	/* Initialization */
	InitWindow(960, 640, "Modbus Viewer");
	SetTargetFPS(30);
	SetExitKey(KEY_NULL);
	
	/* Main loop */
	while ( state != QUIT ) {
		/* Process events */
		if ( WindowShouldClose() ) state = QUIT;
		if ( IsFileDropped() ) process_file_drop();
		
		/* Update app */
		update_imgs();
		update_objects();
		
		/* Draw app */
		BeginDrawing();
		ClearBackground(WHITE);
		draw_objects();
		switch ( state ) {
			case NEW_OBJ:  { if ( draw_new_object_win() )  state = RUN; break; }
			case EDIT_OBJ: { if ( draw_edit_object_win() ) state = RUN; break; }
			case IMG_LIST: { if ( draw_img_list_win() )    state = RUN; break; }
			default: break;
		}
		draw_main_menu();
		EndDrawing();
	}
	
	/* Cleanup */
	unload_objects();
	unload_imgs();
	CloseWindow();
	return 0;
}

void process_file_drop(void)
{
	FilePathList fpl = LoadDroppedFiles();
	for ( int i = 0; i < fpl.count; i++ ) {
		process_dropped_imgs(fpl.paths[i]);
	}
	UnloadDroppedFiles(fpl);
}

void draw_main_menu(void)
{
	/* Panel */
	Rectangle bounds = (Rectangle){ 0, 0, GetScreenWidth(), STD_H+PAD*2 };
	GuiPanel(bounds, NULL);
	
	/* Left Side Buttons */
	/***************************************/
	Rectangle bounds_btn = { 0 };
	
	/* New Object Button */
	bounds_btn = (Rectangle){ PAD, PAD, STD_W, STD_H };
	if ( GuiButton(bounds_btn, "New Object") ) state = NEW_OBJ;
	
	/* Edit Object Button */
	bounds_btn.x += bounds_btn.width + PAD;
	if ( GuiButton(bounds_btn, "Edit Object") ) state = EDIT_OBJ;
	
	/* Image List Button */
	bounds_btn.x += bounds_btn.width + PAD;
	if ( GuiButton(bounds_btn, "Image List") ) state = IMG_LIST;
	
	/* Right Side Buttons */
	/***************************************/
	/* Quit Button */
	bounds_btn.x = GetScreenWidth() - bounds_btn.width - PAD;
	if ( GuiButton(bounds_btn, "Quit") ) state = QUIT;
}
