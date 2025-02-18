#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "imgs.h"
#include "objs.h"

bool quit = false;
bool show_new_obj = false;
bool show_edit_obj = false;
bool show_img_list = false;

void draw_main_menu(void);

int main(void)
{
	InitWindow(960, 640, "Modbus Viewer");
	SetTargetFPS(30);
	while ( !quit ) {
		if ( WindowShouldClose() ) quit = true;
		
		if ( IsFileDropped() ) {
			FilePathList fpl = LoadDroppedFiles();
			for ( int i = 0; i < fpl.count; i++ ) load_img(fpl.paths[i]);
			UnloadDroppedFiles(fpl);
		}
		
		BeginDrawing();
		ClearBackground(WHITE);
		draw_objs();
		if ( show_new_obj ) show_new_obj = !draw_new_obj();
		if ( show_img_list ) show_img_list = !draw_img_list();
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
	if ( GuiButton(btn_bounds, "New Object") ) show_new_obj = true;
	btn_bounds.x += btn_bounds.width + 4;
	if ( GuiButton(btn_bounds, "Edit Object") ) show_edit_obj = true;
	btn_bounds.x += btn_bounds.width + 4;
	if ( GuiButton(btn_bounds, "Image List") ) show_img_list = true;
}
