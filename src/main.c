#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "imgs.h"
#include "objs.h"

bool quit = false;
bool show_img_list = false;
bool show_new_obj = true;

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
		if ( show_new_obj ) show_new_obj = !draw_new_obj();
		if ( show_img_list ) show_img_list = !draw_img_list( 24, 24);
		EndDrawing();
	}
	
	unload_objs();	
	unload_imgs();	
	CloseWindow();
	return 0;
}
