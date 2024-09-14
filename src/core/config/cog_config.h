#ifndef COGNITION_CONFIG_MANAGER_H
#define COGNITION_CONFIG_MANAGER_H

#include <stdbool.h>

#define MAX_CONFIG_ENTRIES 256
#define MAX_KEY_LENGTH 64
#define MAX_VALUE_LENGTH 256

typedef enum {
    COG_CONFIG_INT,
    COG_CONFIG_FLOAT,
    COG_CONFIG_STRING,
    COG_CONFIG_BOOL,
    COG_CONFIG_OBJECT
} CogConfigValueType;

typedef struct CogConfigValue {
    CogConfigValueType type;
    union {
        int int_value;
        float float_value;
        char* string_value;
        bool bool_value;
        struct {
            struct CogConfigValue* values;
            int count;
        } object;
    };
} CogConfigValue;

typedef struct {
    char* key;
    CogConfigValue value;
} CogConfigEntry;

typedef struct {
    CogConfigEntry* entries;
    int count;
    int capacity;
} CogConfig;

CogConfig* cog_config_create();
void cog_config_destroy(CogConfig* config);
void cog_config_set_int(CogConfig* config, const char* key, int value);
void cog_config_set_float(CogConfig* config, const char* key, float value);
void cog_config_set_string(CogConfig* config, const char* key, const char* value);
void cog_config_set_bool(CogConfig* config, const char* key, bool value);
void cog_config_set_object(CogConfig* config, const char* key, CogConfig* object);
CogConfigValue* cog_config_get(CogConfig* config, const char* key);
bool cog_config_load_json(CogConfig* config, const char* filename);
bool cog_config_save_json(CogConfig* config, const char* filename);

#endif // COGNITION_CONFIG_MANAGER_H