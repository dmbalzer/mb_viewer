#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

static bool quit = false;

static void draw_main_menu(void)
{
	static bool file_menu_active = false;
	
	/* Main Menu */
	Rectangle main_menu_bounds = (Rectangle){0, 0, GetScreenWidth(), 24};
	GuiPanel(main_menu_bounds, NULL);
	Rectangle file_menu_bounds = (Rectangle){0, 0, GetTextWidth("FILE") + TEXT_PADDING*2, 24};
	GuiToggle(file_menu_bounds, "File", &file_menu_active);
}

int main(void)
{
	InitWindow(800, 600, "Modbus Viewer");
	SetWindowMinSize(800, 600);
	SetTargetFPS(60);
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetExitKey(KEY_NULL);
	
	while ( !quit )
	{
		if ( WindowShouldClose() ) quit = true;
		BeginDrawing();
		ClearBackground(DARKBLUE);
		
		draw_main_menu();
		
		EndDrawing();
	}
	
	CloseWindow();
	return 0;
}
