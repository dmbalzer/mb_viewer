#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

bool quit = false;

typedef struct _img_node ImgNode;
struct _img_node {
	char name[256];
	Image img;
	Texture tex;
	ImgNode* next;
};

ImgNode head = { 0 };
ImgNode* tail = &head;

static void load_img(const char* fn)
{
	if ( IsFileExtension(fn, ".png") ) {
		ImgNode* in = MemAlloc(sizeof(ImgNode));
		if ( in == 0 ) {
			TraceLog(LOG_ERROR, "Error allocating memory for img.");
			return;
		}
		TextCopy(in->name, GetFileName(fn));
		in->img = LoadImage(fn);
		in->tex = LoadTextureFromImage(in->img);
		tail->next = in;
		tail = in;
	}
}

static void unload_imgs(void)
{
	for ( ImgNode* in = head.next; in != 0; in = in->next ) {
		UnloadTexture(in->tex);
		UnloadImage(in->img);
		MemFree(in);
	}
}

static void list_imgs(void)
{
	int count = 0;
	for ( ImgNode* in = head.next; in != 0; in = in->next ) { count++; }
	int x = 0;
	int y = 0;
	int w = 120;
	int h = 24 * (count + 1);
	Rectangle bounds = (Rectangle){ x,y,w,h };
	GuiPanel(bounds, "Images");
	bounds.x += 8;
	bounds.width -= 16;
	bounds.y += 24;
	bounds.height = 24;
	for ( ImgNode* in = head.next; in != 0; in = in->next ) {
		GuiLabel(bounds, in->name);
		bounds.y += 24;
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
			for ( int i = 0; i < fpl.count; i++ ) load_img(fpl.paths[i]);
			UnloadDroppedFiles(fpl);
		}
		
		BeginDrawing();
		ClearBackground(WHITE);
		list_imgs();
		EndDrawing();
	}
	
	unload_imgs();	
	CloseWindow();
	return 0;
}
