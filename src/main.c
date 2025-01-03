#define RAYGUI_IMPLEMENTATION
#define DEVICE_IMPLEMENTATION
#include <raygui.h>
#include "device.h"

static bool quit = false;
#define MAX_DEVICES 64
static Device devices[MAX_DEVICES] = { 0 };

static void draw_main_menu(void)
{
	static bool file_menu_active = false;
	
	/* Main Menu */
	Rectangle main_menu_bounds = (Rectangle){0, 0, GetScreenWidth(), 24};
	GuiPanel(main_menu_bounds, NULL);
	Rectangle file_menu_bounds = (Rectangle){0, 0, GetTextWidth("File") + TEXT_PADDING*2, 24};
	/* File Menu */
	GuiToggle(file_menu_bounds, "File", &file_menu_active);
	if ( file_menu_active )
	{
		/* Establish back panel bounds */
		Rectangle sub_menu_bounds = { 0, 24, 0, 0 };
		
		sub_menu_bounds.height += 24;
		if ( sub_menu_bounds.width < GetTextWidth("New Device") + TEXT_PADDING*2) sub_menu_bounds.width = GetTextWidth("New Device") + TEXT_PADDING*2;
		
		sub_menu_bounds.height += 24;
		if ( sub_menu_bounds.width < GetTextWidth("Quit") + TEXT_PADDING*2) sub_menu_bounds.width = GetTextWidth("Quit") + TEXT_PADDING*2;
		
		/* Draw Back Panel */
		GuiPanel(sub_menu_bounds, NULL);
		
		int i = 0;
		GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
		/* New Device */
		if ( GuiLabelButton((Rectangle){ sub_menu_bounds.x, sub_menu_bounds.y + 24*i++, sub_menu_bounds.width, 24 }, "New Device") ) { file_menu_active = false; }
		/* Quit */
		if ( GuiLabelButton((Rectangle){ sub_menu_bounds.x, sub_menu_bounds.y + 24*i++, sub_menu_bounds.width, 24 }, "Quit") ) { file_menu_active = false; quit = true; }
		
		GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
		
		/* Close menu if clicked outside */
		if ( IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(GetMousePosition(), file_menu_bounds) && !CheckCollisionPointRec(GetMousePosition(), sub_menu_bounds) ) file_menu_active = false;
	}
}

int main(void)
{
	InitWindow(800, 600, "Modbus Viewer");
	SetWindowMinSize(800, 600);
	SetTargetFPS(60);
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetExitKey(KEY_NULL);
	
	devices[0] = new_device("Downstairs AHU", "192.168.0.12", 1502, 1, (Vector2){ 64, 64 });

	while ( !quit )
	{
		if ( WindowShouldClose() ) quit = true;
		BeginDrawing();
		ClearBackground(DARKBLUE);
		
		draw_device(&devices[0]);

		draw_main_menu();
		
		EndDrawing();
	}
	
	CloseWindow();
	return 0;
}
