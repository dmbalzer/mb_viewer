#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

bool quit = false;

Image img = { 0 };
char name[256] = { 0 };
Texture tex = { 0 };

static void load_dropped_img(const char* fn)
{
	if ( IsFileExtension(fn, ".png") ) {
		UnloadImage(img);
		UnloadTexture(tex);
		TextCopy(name, fn);
		img = LoadImage(fn);
		tex = LoadTextureFromImage(img);
	}
}

int main(void)
{
	InitWindow(960, 640, "Modbus Viewer");
	SetTargetFPS(30);

	while ( !quit ) {
		if ( WindowShouldClose() ) quit = true;
		
		if ( IsFileDropped() ) {
			FilePathList fpl = LoadDroppedFiles();
			for ( int i = 0; i < fpl.count; i++ ) load_dropped_img(fpl.paths[i]);
			UnloadDroppedFiles(fpl);
		}
		
		BeginDrawing();
		ClearBackground(WHITE);
		DrawTexture(tex, 24, 24, WHITE);
		EndDrawing();
	}

	UnloadImage(img);
	UnloadTexture(tex);
	CloseWindow();
	return 0;
}
