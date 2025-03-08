#ifndef MB_INCLUDED
#define MB_INCLUDED

#include <stdbool.h>

void* mb_ctx_new(const char* ip, int port);
void mb_ctx_free(void* ctx);
bool mb_ctx_connect(void* ctx);
void mb_ctx_close(void* ctx);

#endif // MB_INCLUDED
