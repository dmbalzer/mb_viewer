#include "imgs.h"
#include "defs.h"
#undef RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <stdlib.h>
#include <SDL3_image/SDL_image.h>

static int id = 0;
static Img head = { 0 };
static Img* tail = &head;

static void _new_img(const char* path)
{
	Img* o = MemAlloc(sizeof(Img));
	if ( o == NULL ) { TraceLog(LOG_ERROR, "Error allocating memory for img."); return; }
	o->id = id++;
	o->image = LoadImageAnim(path, &o->count);
	if ( !IsImageValid(o->image) ) { TraceLog(LOG_ERROR, "Error creating img."); free(o); }
	o->texture = LoadTextureFromImage(o->image);
	IMG_Animation* anim = IMG_LoadAnimation(path);
	if ( anim == NULL ) { o->delay = 0.1f; }
	else { o->delay = anim->delays[0] / 1000.0f; IMG_FreeAnimation(anim); }
	TextCopy(o->name, GetFileName(path));
	tail->next = o;
	tail = o;
}

void load_imgs(void)
{
	
}

void unload_imgs(void)
{
	Img* o = head.next;
	while ( o != NULL ) {
		int _id = o->id;
		Img* tmp = o;
		o = o->next;
		free(tmp);
		TraceLog(LOG_INFO, TextFormat("IMG [ID %d] unloaded.", _id));
	}
}

void update_imgs(void)
{
	for ( Img* o = head.next; o != NULL; o = o->next ) {
		
	}
}

void process_dropped_imgs(const char* path)
{
	if ( !IsFileExtension(path, ".png") && !IsFileExtension(path, ".jpg") && !IsFileExtension(path, ".gif") ) return;
	_new_img(path);
}

int draw_img_list_win(void)
{
	GuiEnable();
	int result = 0;
	int x = GetScreenWidth()/2 - STD_WIN_W/2;
	int y = GetScreenHeight()/2 - STD_WIN_H/2;
	Rectangle bounds = (Rectangle){ x, y, STD_WIN_W, STD_WIN_H };
	if ( GuiWindowBox(bounds, "Image List") ) result = 1;
	
	/* Img name buffer for list items */
	char buff[1024] = { 0 };
	int cursor = 0;
	for ( Img* o = head.next; o != NULL; o = o->next ) {
		TextAppend(buff, o->name, &cursor);
		TextAppend(buff, ";", &cursor);
	}
	buff[cursor] = '\0';
	
	/* Selection List */
	static int scroll_idx = 0;
	static int active = 0;
	Rectangle bounds_list = (Rectangle){
		bounds.x + PAD,
		bounds.y + 24 + PAD,
		bounds.width - PAD*2,
		(bounds.height - 24 - PAD*2) - (STD_H * 2) - (PAD * 2)};
	GuiListView(bounds_list, buff, &scroll_idx, &active);
	
	/* Submit Button */
	Rectangle bounds_btn = (Rectangle){
		bounds.x + bounds.width/2 - STD_W/2,
		bounds.y + bounds.height - STD_H - PAD,
		STD_W,
		STD_H};
	if ( GuiButton(bounds_btn, "Submit") ) {
		result = 1;
	}
	
	GuiDisable();
	return result;
}
