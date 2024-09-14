#include "cog_config.h"
#include "core/utils/cog_memory.h"
#include "core/utils/cog_error.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

CogConfig* cog_config_create() {
    CogConfig* config = (CogConfig*)cog_malloc(sizeof(CogConfig));
    if (!config) {
        cog_log_error("Failed to allocate memory for CogConfig");
        return NULL;
    }
    config->entries = (CogConfigEntry*)cog_malloc(sizeof(CogConfigEntry) * MAX_CONFIG_ENTRIES);
    if (!config->entries) {
        cog_log_error("Failed to allocate memory for CogConfig entries");
        cog_free(config);
        return NULL;
    }
    config->count = 0;
    config->capacity = MAX_CONFIG_ENTRIES;
    cog_log_info("Config created");
    return config;
}

void cog_config_destroy(CogConfig* config) {
    if (!config) return;
    for (int i = 0; i < config->count; i++) {
        COG_DELETE(config->entries[i].key);
        if (config->entries[i].value.type == COG_CONFIG_STRING) {
            COG_DELETE(config->entries[i].value.string_value);
        } else if (config->entries[i].value.type == COG_CONFIG_OBJECT) {
            cog_config_destroy((CogConfig*)config->entries[i].value.object.values);
        }
    }
    COG_DELETE(config->entries);
    COG_DELETE(config);
    cog_log_info("Config destroyed");
}

static void cog_config_set_value(CogConfig* config, const char* key, CogConfigValue value) {
    for (int i = 0; i < config->count; i++) {
        if (strcmp(config->entries[i].key, key) == 0) {
            if (config->entries[i].value.type == COG_CONFIG_STRING) {
                COG_DELETE(config->entries[i].value.string_value);
            } else if (config->entries[i].value.type == COG_CONFIG_OBJECT) {
                cog_config_destroy((CogConfig*)config->entries[i].value.object.values);
            }
            config->entries[i].value = value;
            return;
        }
    }
    if (config->count < config->capacity) {
        config->entries[config->count].key = cog_strdup(key);
        config->entries[config->count].value = value;
        config->count++;
    } else {
        cog_log_error("Config is full, cannot add more entries");
    }
}

void cog_config_set_int(CogConfig* config, const char* key, int value) {
    CogConfigValue val = {.type = COG_CONFIG_INT, .int_value = value};
    cog_config_set_value(config, key, val);
}

void cog_config_set_float(CogConfig* config, const char* key, float value) {
    CogConfigValue val = {.type = COG_CONFIG_FLOAT, .float_value = value};
    cog_config_set_value(config, key, val);
}

void cog_config_set_string(CogConfig* config, const char* key, const char* value) {
    CogConfigValue val = {.type = COG_CONFIG_STRING, .string_value = cog_strdup(value)};
    cog_config_set_value(config, key, val);
}

void cog_config_set_bool(CogConfig* config, const char* key, bool value) {
    CogConfigValue val = {.type = COG_CONFIG_BOOL, .bool_value = value};
    cog_config_set_value(config, key, val);
}

void cog_config_set_object(CogConfig* config, const char* key, CogConfig* object) {
    CogConfigValue val = {.type = COG_CONFIG_OBJECT, .object = {.values = (struct CogConfigValue*)object, .count = object->count}};
    cog_config_set_value(config, key, val);
}

CogConfigValue* cog_config_get(CogConfig* config, const char* key) {
    for (int i = 0; i < config->count; i++) {
        if (strcmp(config->entries[i].key, key) == 0) {
            return &config->entries[i].value;
        }
    }
    return NULL;
}

// TODO: The JSON loading and saving functions are not implemented here.
// use a JSON parsing library like cJSON or something.

bool cog_config_load_json(CogConfig* config, const char* filename) {
    cog_log_error("JSON loading not implemented");
    return false;
}

bool cog_config_save_json(CogConfig* config, const char* filename) {
    cog_log_error("JSON saving not implemented");
    return false;
}