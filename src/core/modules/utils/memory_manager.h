#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>

void* cog_malloc(size_t size);
void* cog_calloc(size_t num, size_t size);
void* cog_realloc(void* ptr, size_t new_size);
char* cog_strdup(const char* str);
void cog_free(void* ptr);

#define COG_NEW(type) ((type*)cog_malloc(sizeof(type)))
#define COG_DELETE(ptr) do { cog_free(ptr); ptr = NULL; } while(0)

#endif // MEMORY_MANAGER_H