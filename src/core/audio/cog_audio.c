#include "cog_audio.h"
#include "core/utils/cog_error.h"
#include "core/utils/cog_memory.h"
#include <string.h>

bool audio_system_init(AudioSystem* system) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cog_log_error("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
        return false;
    }

    system->sound_count = 0;
    system->music_count = 0;
    system->master_volume = 1.0f;
    system->sound_volume = 1.0f;
    system->music_volume = 1.0f;

    Mix_AllocateChannels(MAX_SOUNDS);

    cog_log_info("Audio system initialized successfully");
    return true;
}

void audio_system_cleanup(AudioSystem* system) {
    for (int i = 0; i < system->sound_count; i++) {
        Mix_FreeChunk(system->sounds[i].chunk);
    }

    for (int i = 0; i < system->music_count; i++) {
        Mix_FreeMusic(system->music[i].music);
        COG_DELETE(system->music[i].name);
    }

    Mix_CloseAudio();
    cog_log_info("Audio system cleaned up");
}

int audio_load_sound(AudioSystem* system, const char* file_path) {
    if (system->sound_count >= MAX_SOUNDS) {
        cog_log_error("Maximum number of sounds reached");
        return -1;
    }

    Mix_Chunk* chunk = Mix_LoadWAV(file_path);
    if (!chunk) {
        cog_log_error("Failed to load sound: %s", Mix_GetError());
        return -1;
    }

    system->sounds[system->sound_count].chunk = chunk;
    system->sounds[system->sound_count].channel = -1;
    
    cog_log_info("Sound loaded: %s", file_path);
    return system->sound_count++;
}

int audio_load_music(AudioSystem* system, const char* file_path, const char* name) {
    if (system->music_count >= MAX_MUSIC) {
        cog_log_error("Maximum number of music tracks reached");
        return -1;
    }

    Mix_Music* music = Mix_LoadMUS(file_path);
    if (!music) {
        cog_log_error("Failed to load music: %s", Mix_GetError());
        return -1;
    }

    system->music[system->music_count].music = music;
    system->music[system->music_count].name = cog_strdup(name);
    
    cog_log_info("Music loaded: %s as %s", file_path, name);
    return system->music_count++;
}

void audio_play_sound(AudioSystem* system, int sound_id, int loops) {
    if (sound_id < 0 || sound_id >= system->sound_count) {
        cog_log_error("Invalid sound ID: %d", sound_id);
        return;
    }

    int channel = Mix_PlayChannel(-1, system->sounds[sound_id].chunk, loops);
    if (channel == -1) {
        cog_log_error("Failed to play sound: %s", Mix_GetError());
    } else {
        system->sounds[sound_id].channel = channel;
        Mix_Volume(channel, (int)(MIX_MAX_VOLUME * system->master_volume * system->sound_volume));
    }
}

void audio_play_music(AudioSystem* system, const char* name, int loops) {
    for (int i = 0; i < system->music_count; i++) {
        if (strcmp(system->music[i].name, name) == 0) {
            if (Mix_PlayMusic(system->music[i].music, loops) == -1) {
                cog_log_error("Failed to play music: %s", Mix_GetError());
            } else {
                Mix_VolumeMusic((int)(MIX_MAX_VOLUME * system->master_volume * system->music_volume));
            }
            return;
        }
    }
    cog_log_error("Music not found: %s", name);
}

void audio_stop_music(AudioSystem* system) {
    Mix_HaltMusic();
}

void audio_set_master_volume(AudioSystem* system, float volume) {
    system->master_volume = volume;
    Mix_VolumeMusic((int)(MIX_MAX_VOLUME * system->master_volume * system->music_volume));
    for (int i = 0; i < system->sound_count; i++) {
        if (system->sounds[i].channel != -1) {
            Mix_Volume(system->sounds[i].channel, (int)(MIX_MAX_VOLUME * system->master_volume * system->sound_volume));
        }
    }
}

void audio_set_sound_volume(AudioSystem* system, float volume) {
    system->sound_volume = volume;
    for (int i = 0; i < system->sound_count; i++) {
        if (system->sounds[i].channel != -1) {
            Mix_Volume(system->sounds[i].channel, (int)(MIX_MAX_VOLUME * system->master_volume * system->sound_volume));
        }
    }
}

void audio_set_music_volume(AudioSystem* system, float volume) {
    system->music_volume = volume;
    Mix_VolumeMusic((int)(MIX_MAX_VOLUME * system->master_volume * system->music_volume));
}