#include "config_manager.h"
#include "core/modules/utils/memory_manager.h"
#include "core/modules/utils/error_handling.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void config_manager_init(ConfigManager* manager) {
    manager->count = 0;
    cog_log_info("Config manager initialized");
}

bool config_manager_load_from_file(ConfigManager* manager, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        cog_log_error("Failed to open config file: %s", filename);
        return false;
    }

    char line[MAX_KEY_LENGTH + MAX_VALUE_LENGTH + 2];
    while (fgets(line, sizeof(line), file) && manager->count < MAX_CONFIG_ENTRIES) {
        char* equals = strchr(line, '=');
        if (equals) {
            *equals = '\0';
            char* key = line;
            char* value = equals + 1;
            
            char* newline = strchr(value, '\n');
            if (newline) *newline = '\0';

            config_manager_set(manager, key, value);
        }
    }

    fclose(file);
    cog_log_info("Config loaded from file: %s", filename);
    return true;
}

bool config_manager_save_to_file(ConfigManager* manager, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return false;

    for (int i = 0; i < manager->count; i++) {
        fprintf(file, "%s=%s\n", manager->entries[i].key, manager->entries[i].value);
    }

    fclose(file);
    return true;
}

bool config_manager_set(ConfigManager* manager, const char* key, const char* value) {
    if (manager->count >= MAX_CONFIG_ENTRIES) return false;

    // Check if key already exists
    for (int i = 0; i < manager->count; i++) {
        if (strcmp(manager->entries[i].key, key) == 0) {
            snprintf(manager->entries[i].value, MAX_VALUE_LENGTH, "%s", value);
            return true;
        }
    }

    // Add new entry
    snprintf(manager->entries[manager->count].key, MAX_KEY_LENGTH, "%s", key);
    snprintf(manager->entries[manager->count].value, MAX_VALUE_LENGTH, "%s", value);
    manager->count++;

    return true;
}

const char* config_manager_get(ConfigManager* manager, const char* key, const char* default_value) {
    for (int i = 0; i < manager->count; i++) {
        if (strcmp(manager->entries[i].key, key) == 0) {
            return manager->entries[i].value;
        }
    }
    return default_value;
}

int config_manager_get_int(ConfigManager* manager, const char* key, int default_value) {
    const char* value = config_manager_get(manager, key, NULL);
    return value ? atoi(value) : default_value;
}

float config_manager_get_float(ConfigManager* manager, const char* key, float default_value) {
    const char* value = config_manager_get(manager, key, NULL);
    return value ? (float)atof(value) : default_value;
}

bool config_manager_get_bool(ConfigManager* manager, const char* key, bool default_value) {
    const char* value = config_manager_get(manager, key, NULL);
    if (!value) return default_value;
    return (strcmp(value, "true") == 0 || strcmp(value, "1") == 0);
}