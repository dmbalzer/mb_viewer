#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

bool quit = false;

int main(void)
{
	InitWindow(960, 640, "Modbus Viewer");
	SetTargetFPS(30);
	
	while ( !quit ) {
		if ( WindowShouldClose() ) quit = true;
		BeginDrawing();
		ClearBackground(WHITE);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
