#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>

#define MIN_ALLOCATION_SIZE sizeof(FreeBlock)

typedef struct FreeBlock {
    size_t size;
    struct FreeBlock* next;
} FreeBlock;

typedef struct {
    void* memory;
    size_t total_size;
    size_t used;
    FreeBlock* free_list;
} CogAllocator;

CogAllocator* cog_allocator_create(size_t size);
void* cog_allocator_alloc(CogAllocator* allocator, size_t size);
void cog_allocator_free(CogAllocator* allocator, void* ptr);
void cog_allocator_destroy(CogAllocator* allocator);

typedef struct {
    void* ptr;
    size_t size;
    const char* file;
    int line;
} MemoryAllocation;

void cog_memory_track_allocation(void* ptr, size_t size, const char* file, int line);
void cog_memory_track_deallocation(void* ptr);
void cog_memory_print_leaks();

typedef struct {
    size_t total_allocated;
    size_t peak_allocated;
    size_t current_allocated;
    int allocation_count;
} MemoryStats;

MemoryStats cog_memory_get_stats();
void cog_memory_print_stats();

void* cog_malloc(size_t size);
void* cog_calloc(size_t num, size_t size);
void* cog_realloc(void* ptr, size_t new_size);
char* cog_strdup(const char* str);
void cog_free(void* ptr);

#define COG_NEW(type) ((type*)cog_malloc(sizeof(type)))
#define COG_DELETE(ptr) do { cog_free(ptr); ptr = NULL; } while(0)

#endif // MEMORY_MANAGER_H