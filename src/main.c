#include <raylib.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#include "mb.h"

int main(void)
{
	InitWindow(960, 640, "Modbus Viewer");
	SetTargetFPS(30);
	void* ctx = mb_ctx_new("192.168.0.12", 1502);
	bool status = mb_ctx_connect(ctx);
	Font font = LoadFontEx("assets/Roboto-Regular.ttf", 18, NULL, 0);
	while ( !WindowShouldClose() ) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawTextEx(font, TextFormat("%s", status ? "ONLINE" : "OFFLINE"), (Vector2){ 16, 16 }, 18, 0, status ? DARKGREEN : DARKGRAY);
		EndDrawing();
	}
	UnloadFont(font);
	mb_ctx_close(ctx);
	mb_ctx_free(ctx);
	CloseWindow();
	return 0;
}
