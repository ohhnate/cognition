#include "cog_lua.h"
#include "core/utils/cog_memory.h"
#include "core/utils/cog_error.h"
#include "core/graphics/cog_sprite.h"
#include "core/graphics/cog_render_components.h"
#include "core/resource/cog_resource.h"

// Lua wrapper functions for engine functionality
static int create_entity(lua_State* L) {
    ScriptSystem* system = (ScriptSystem*)lua_touserdata(L, lua_upvalueindex(1));
    Entity* entity = NULL;
    ECSResult result = cog_create_entity(system->engine->ecs, &entity);
    if (result == ECS_OK && entity != NULL) {
        lua_pushlightuserdata(L, entity);
        return 1;
    }
    return 0;
}

static int add_render_component(lua_State* L) {
    ScriptSystem* system = (ScriptSystem*)lua_touserdata(L, lua_upvalueindex(1));
    Entity* entity = (Entity*)lua_touserdata(L, 1);
    const char* sprite_name = lua_tostring(L, 2);
    
    Texture* texture = resource_manager_get_texture(system->engine->resource_manager, sprite_name);
    if (texture) {
        Sprite* sprite = sprite_create(texture);
        Component render_component = create_render_component(sprite, 0, 0);
        ECSResult result = cog_add_component(system->engine->ecs, entity, render_component);
        lua_pushboolean(L, result == ECS_OK);
    } else {
        lua_pushboolean(L, 0);
    }
    return 1;
}

static int set_position(lua_State* L) {
    Entity* entity = (Entity*)lua_touserdata(L, 1);
    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    transform_set_position(&entity->transform, (Vector2){x, y});
    return 0;
}

ScriptSystem* create_script_system(CognitionEngine* engine) {
    ScriptSystem* system = COG_NEW(ScriptSystem);
    if (!system) {
        cog_log_error("Failed to allocate memory for ScriptSystem");
        return NULL;
    }
    
    system->L = luaL_newstate();
    if (!system->L) {
        cog_log_error("Failed to create Lua state");
        COG_DELETE(system);
        return NULL;
    }
    
    luaL_openlibs(system->L);
    system->engine = engine;
    
    register_engine_functions(system);
    
    return system;
}

void destroy_script_system(ScriptSystem* system) {
    if (system) {
        if (system->L) {
            lua_close(system->L);
        }
        COG_DELETE(system);
    }
}

int load_script(ScriptSystem* system, const char* filename) {
    if (luaL_dofile(system->L, filename) != LUA_OK) {
        cog_log_error("Failed to load Lua script: %s", lua_tostring(system->L, -1));
        lua_pop(system->L, 1);
        return 0;
    }
    return 1;
}

void update_scripts(ScriptSystem* system, float delta_time) {
    lua_getglobal(system->L, "update");
    if (lua_isfunction(system->L, -1)) {
        lua_pushnumber(system->L, delta_time);
        if (lua_pcall(system->L, 1, 0, 0) != LUA_OK) {
            cog_log_error("Failed to call update function: %s", lua_tostring(system->L, -1));
            lua_pop(system->L, 1);
        }
    } else {
        lua_pop(system->L, 1);
    }
}

void register_engine_functions(ScriptSystem* system) {
    lua_pushlightuserdata(system->L, system);
    
    lua_pushcclosure(system->L, create_entity, 1);
    lua_setglobal(system->L, "create_entity");
    
    lua_pushlightuserdata(system->L, system);
    lua_pushcclosure(system->L, add_render_component, 1);
    lua_setglobal(system->L, "add_render_component");
    
    lua_pushcclosure(system->L, set_position, 1);
    lua_setglobal(system->L, "set_position");
    
    // Add more function registrations as needed
}