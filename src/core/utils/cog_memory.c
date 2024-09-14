#include "cog_memory.h"
#include "cog_error.h"
#include <stdlib.h>
#include <string.h>

#define MAX_ALLOCATIONS 1024

static MemoryAllocation allocations[MAX_ALLOCATIONS];
static int allocation_count = 0;
static MemoryStats memory_stats = {0};

CogAllocator* cog_allocator_create(size_t size) {
    CogAllocator* allocator = (CogAllocator*)malloc(sizeof(CogAllocator));
    if (!allocator) {
        cog_log_error("Failed to create allocator");
        return NULL;
    }
    allocator->memory = malloc(size);
    if (!allocator->memory) {
        cog_log_error("Failed to allocate memory for allocator");
        free(allocator);
        return NULL;
    }
    allocator->total_size = size;
    allocator->used = 0;
    allocator->free_list = NULL;
    return allocator;
}

void* cog_allocator_alloc(CogAllocator* allocator, size_t size) {
    // Align size to multiple of MIN_ALLOCATION_SIZE
    size = (size + MIN_ALLOCATION_SIZE - 1) & ~(MIN_ALLOCATION_SIZE - 1);

    FreeBlock* prev = NULL;
    FreeBlock* current = allocator->free_list;

    // Search for a suitable free block
    while (current != NULL) {
        if (current->size >= size) {
            // Found a suitable block
            if (current->size >= size + MIN_ALLOCATION_SIZE) {
                // Split the block
                FreeBlock* new_block = (FreeBlock*)((char*)current + size);
                new_block->size = current->size - size;
                new_block->next = current->next;
                
                if (prev) {
                    prev->next = new_block;
                } else {
                    allocator->free_list = new_block;
                }
            } else {
                // Use the entire block
                if (prev) {
                    prev->next = current->next;
                } else {
                    allocator->free_list = current->next;
                }
            }
            return current;
        }
        prev = current;
        current = current->next;
    }

    // No suitable free block found, allocate from the end
    if (allocator->used + size > allocator->total_size) {
        cog_log_error("Allocator out of memory");
        return NULL;
    }
    void* ptr = (char*)allocator->memory + allocator->used;
    allocator->used += size;
    return ptr;
}

void cog_allocator_free(CogAllocator* allocator, void* ptr) {
    if (!ptr || ptr < allocator->memory || ptr >= (char*)allocator->memory + allocator->total_size) {
        cog_log_error("Invalid pointer passed to cog_allocator_free");
        return;
    }

    FreeBlock* block = (FreeBlock*)ptr;
    block->size = 0;  // We'll calculate the correct size later

    // Find the correct position to insert the free block
    FreeBlock* prev = NULL;
    FreeBlock* current = allocator->free_list;
    while (current != NULL && current < block) {
        prev = current;
        current = current->next;
    }

    // Insert the block into the free list
    block->next = current;
    if (prev) {
        prev->next = block;
    } else {
        allocator->free_list = block;
    }

    // Merge with next block if adjacent
    if ((char*)block + block->size == (char*)current) {
        block->size += current->size;
        block->next = current->next;
    }

    // Merge with previous block if adjacent
    if (prev && (char*)prev + prev->size == (char*)block) {
        prev->size += block->size;
        prev->next = block->next;
    }

    // Update the block size if it's at the end of the used memory
    if ((char*)block + block->size == (char*)allocator->memory + allocator->used) {
        size_t freed_size = (char*)allocator->memory + allocator->used - (char*)block;
        block->size = freed_size;
        allocator->used -= freed_size;
    }
}

void cog_allocator_destroy(CogAllocator* allocator) {
    if (allocator) {
        free(allocator->memory);
        free(allocator);
    }
}

void cog_memory_track_allocation(void* ptr, size_t size, const char* file, int line) {
    if (allocation_count >= MAX_ALLOCATIONS) {
        cog_log_error("Max allocations reached");
        return;
    }
    allocations[allocation_count].ptr = ptr;
    allocations[allocation_count].size = size;
    allocations[allocation_count].file = file;
    allocations[allocation_count].line = line;
    allocation_count++;

    memory_stats.total_allocated += size;
    memory_stats.current_allocated += size;
    memory_stats.allocation_count++;
    if (memory_stats.current_allocated > memory_stats.peak_allocated) {
        memory_stats.peak_allocated = memory_stats.current_allocated;
    }
}

void cog_memory_track_deallocation(void* ptr) {
    for (int i = 0; i < allocation_count; i++) {
        if (allocations[i].ptr == ptr) {
            memory_stats.current_allocated -= allocations[i].size;
            allocations[i] = allocations[allocation_count - 1];
            allocation_count--;
            return;
        }
    }
    cog_log_error("Attempted to free untracked memory");
}

void cog_memory_print_leaks() {
    for (int i = 0; i < allocation_count; i++) {
        cog_log_error("Memory leak: %zu bytes allocated at %s:%d",
                      allocations[i].size, allocations[i].file, allocations[i].line);
    }
}

MemoryStats cog_memory_get_stats() {
    return memory_stats;
}

void cog_memory_print_stats() {
    cog_log_info("Memory Stats: Total: %zu, Peak: %zu, Current: %zu, Count: %d",
                 memory_stats.total_allocated, memory_stats.peak_allocated,
                 memory_stats.current_allocated, memory_stats.allocation_count);
}

void* cog_malloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        cog_log_error("Memory allocation failed");
        return NULL;
    }
    cog_memory_track_allocation(ptr, size, __FILE__, __LINE__);
    return ptr;
}

void* cog_calloc(size_t num, size_t size) {
    void* ptr = calloc(num, size);
    if (!ptr) {
        cog_log_error("Memory allocation failed");
        return NULL;
    }
    cog_memory_track_allocation(ptr, num * size, __FILE__, __LINE__);
    return ptr;
}

void* cog_realloc(void* ptr, size_t new_size) {
    cog_memory_track_deallocation(ptr);
    void* new_ptr = realloc(ptr, new_size);
    if (!new_ptr) {
        cog_log_error("Memory reallocation failed");
        return NULL;
    }
    cog_memory_track_allocation(new_ptr, new_size, __FILE__, __LINE__);
    return new_ptr;
}

char* cog_strdup(const char* str) {
    if (str == NULL) {
        cog_log_error("Attempted to duplicate NULL string");
        return NULL;
    }
    
    size_t len = strlen(str) + 1;
    char* new_str = (char*)cog_malloc(len);
    
    if (new_str == NULL) {
        cog_log_error("Failed to allocate memory for string duplication");
        return NULL;
    }
    
    memcpy(new_str, str, len);
    return new_str;
}

void cog_free(void* ptr) {
    if (ptr) {
        cog_memory_track_deallocation(ptr);
        free(ptr);
    }
}
