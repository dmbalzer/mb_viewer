#ifndef DEVICE_INCLUDED
#define DEVICE_INCLUDED

#include <raylib.h>

typedef struct {
	bool active;
	char name[128];
	char ip[128];
	int port;
	int addr;
	Vector2 pos;
	bool status;
} Device;

Device new_device(
		const char* name,
		const char* ip,
		int port,
		int addr,
		Vector2 pos);
void device_window(Device* d);
void draw_device(Device* d);

#endif // DEVICE_INCLUDED

#if defined(DEVICE_IMPLEMENTATION)

#undef RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <string.h>

Device new_device(
		const char* name,
		const char* ip,
		int port,
		int addr,
		Vector2 pos)
{
	Device d = { 0 };
	d.active = true;
	strcpy(d.name, name);
	strcpy(d.ip, ip);
	d.port = port;
	d.addr = addr;
	d.pos = pos;
	return d;
}

int draw_device_window(Device* d)
{
	/* TODO */
	GuiEnable();
	int result = 0;
	Rectangle bounds = (Rectangle){ 0, 0, 120, 120 };
	bounds.x = GetScreenWidth()/2 - bounds.width/2;
	bounds.y = GetScreenHeight()/2 - bounds.height/2;
	if ( GuiWindowBox(bounds, "New Device") ) result = 1;
	if ( !result ) GuiDisable();
	return result;
}

void draw_device(Device* d)
{
	if ( !d->active ) return;
	Rectangle bounds = (Rectangle){ d->pos.x, d->pos.y, 120, 24 };
	GuiPanel(bounds, d->name);
}

#endif // DEVICE_IMPLEMENTATION
