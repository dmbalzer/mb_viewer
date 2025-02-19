#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "objs.h"
#include "imgs.h"

#define STATE_QTY 2
typedef enum {
	RUN,
	QUIT,
} State;

void draw_main_menu(void);

static State state = RUN;

int main(void)
{
	InitWindow(960, 640, "Modbus Viewer");
	SetTargetFPS(30);
	SetExitKey(KEY_NULL);

	while ( state != QUIT ) {
		if ( WindowShouldClose() ) state = QUIT;
		
		if ( IsFileDropped() ) {
			FilePathList fpl = LoadDroppedFiles();
			for ( int i = 0; i < fpl.count; i++ ) {  }
			UnloadDroppedFiles(fpl);
		}
		
		BeginDrawing();
		ClearBackground(WHITE);

		draw_main_menu();
		EndDrawing();
	}
		
	CloseWindow();
	return 0;
}

void draw_main_menu(void)
{
	
}
