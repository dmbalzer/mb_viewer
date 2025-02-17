#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "imgs.h"
#include "objs.h"

bool quit = false;
bool show_img_list = true;

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
		if ( show_img_list ) show_img_list = !draw_img_list( 24, 24);
		EndDrawing();
	}
	
	unload_imgs();	
	CloseWindow();
	return 0;
}
