#include <raylib.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#include "mb.h"

Texture* textures = NULL;

void process_drop(void)
{
	FilePathList fpl = LoadDroppedFiles();
	for ( int i = 0; i < fpl.count; i++ ) {
		if ( IsFileExtension(fpl.paths[i], ".png") ) {
			arrput(textures, LoadTexture(fpl.paths[i]));
		}
	}
	UnloadDroppedFiles(fpl);
}

void draw_textures(void)
{
	for ( int i = 0; i < arrlen(textures); i++ ) {
		DrawTexture(textures[i], 16, 16, WHITE);
	}
}

int main(void)
{
	InitWindow(960, 640, "Modbus Viewer");
	SetTargetFPS(30);
	Font font = LoadFontEx("assets/Roboto-Regular.ttf", 18, NULL, 0);
	void* ctx = mb_ctx_new("192.168.0.12", 1502);

	while ( !WindowShouldClose() ) {
		
		if ( IsFileDropped() ) process_drop();

		BeginDrawing();
		ClearBackground(RAYWHITE);

		draw_textures();
		
		EndDrawing();
	}

	for ( int i = 0; i < arrlen(textures); i++ ) {
		UnloadTexture(textures[i]);
	}

	mb_ctx_close(ctx);
	mb_ctx_free(ctx);
	UnloadFont(font);
	CloseWindow();
	return 0;
}
