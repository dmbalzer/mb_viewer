#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "objs.h"
#include "imgs.h"

#define STATE_QTY 2
typedef enum {
	RUN,
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
		
		/* Draw app */
		BeginDrawing();
		ClearBackground(WHITE);

		draw_main_menu();
		EndDrawing();
	}
	
	/* Cleanup */
	CloseWindow();
	return 0;
}

void process_file_drop(void)
{
	FilePathList fpl = LoadDroppedFiles();
	for ( int i = 0; i < fpl.count; i++ ) {
		
	}
	UnloadDroppedFiles(fpl);
}

void draw_main_menu(void)
{
	/* Button dimensions */
	int w = 60;
	int h = 20;
	int pad = 4;
	
	/* Panel */
	Rectangle bounds = (Rectangle){ 0, 0, GetScreenWidth(), h+pad*2 };
	GuiPanel(bounds, NULL);
	
	/* Quit Button */
	Rectangle bounds_btn = (Rectangle){ GetScreenWidth() - w - pad, pad, w, h };
	if ( GuiButton(bounds_btn, "Quit") ) state = QUIT;
}
