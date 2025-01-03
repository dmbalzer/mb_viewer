// Test
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

static bool quit = false;

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
