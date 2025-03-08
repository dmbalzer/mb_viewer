#include "mb.h"
#include <modbus/modbus.h>

void* mb_ctx_new(const char* ip, int port)
{
	return (void*)modbus_new_tcp(ip, port);
}
void mb_ctx_free(void* ctx)
{
	modbus_free((modbus_t*)ctx);
}
bool mb_ctx_connect(void* ctx)
{
	bool result = false;
	if ( modbus_connect((modbus_t*)ctx) == 0 ) result = true;
	return result;
}
void mb_ctx_close(void* ctx)
{
	modbus_close((modbus_t*)ctx);
}
