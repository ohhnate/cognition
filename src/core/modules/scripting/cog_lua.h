#ifndef COG_LUA_H
#define COG_LUA_H

#include "../lua/include/lua.h"
#include "../lua/include/lualib.h"
#include "../lua/include/lauxlib.h"
#include "core/cognition.h"

typedef struct {
    lua_State* L;
    CognitionEngine* engine;
} ScriptSystem;

ScriptSystem* create_script_system(CognitionEngine* engine);
void destroy_script_system(ScriptSystem* system);
int load_script(ScriptSystem* system, const char* filename);
void update_scripts(ScriptSystem* system, float delta_time);

// Function to register C functions with Lua
void register_engine_functions(ScriptSystem* system);

#endif // COG_LUA_H