#include "memory_manager.h"
#include <stdlib.h>
#include <string.h>
#include "error_handling.h"

void* cog_malloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        cog_log_error("Memory allocation failed");
        return NULL;
    }
    return ptr;
}

void* cog_calloc(size_t num, size_t size) {
    void* ptr = calloc(num, size);
    if (!ptr) {
        cog_log_error("Memory allocation failed");
        return NULL;
    }
    return ptr;
}

void* cog_realloc(void* ptr, size_t new_size) {
    void* new_ptr = realloc(ptr, new_size);
    if (!new_ptr) {
        cog_log_error("Memory reallocation failed");
        return NULL;
    }
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
    free(ptr);
}