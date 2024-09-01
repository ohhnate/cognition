#ifndef COGNITION_CONFIG_MANAGER_H
#define COGNITION_CONFIG_MANAGER_H

#include <stdbool.h>

#define MAX_CONFIG_ENTRIES 256
#define MAX_KEY_LENGTH 64
#define MAX_VALUE_LENGTH 256

typedef struct {
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
} ConfigEntry;

typedef struct {
    ConfigEntry entries[MAX_CONFIG_ENTRIES];
    int count;
} ConfigManager;

void config_manager_init(ConfigManager* manager);
bool config_manager_load_from_file(ConfigManager* manager, const char* filename);
bool config_manager_save_to_file(ConfigManager* manager, const char* filename);
bool config_manager_set(ConfigManager* manager, const char* key, const char* value);
const char* config_manager_get(ConfigManager* manager, const char* key, const char* default_value);
int config_manager_get_int(ConfigManager* manager, const char* key, int default_value);
float config_manager_get_float(ConfigManager* manager, const char* key, float default_value);
bool config_manager_get_bool(ConfigManager* manager, const char* key, bool default_value);

#endif // COGNITION_CONFIG_MANAGER_H