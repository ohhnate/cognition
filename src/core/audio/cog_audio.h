#ifndef COG_AUDIO_H
#define COG_AUDIO_H

#include <SDL_mixer.h>
#include <stdbool.h>

#define MAX_SOUNDS 32
#define MAX_MUSIC 8

typedef struct {
    Mix_Chunk* chunk;
    int channel;
} Sound;

typedef struct {
    Mix_Music* music;
    char* name;
} Music;

typedef struct {
    Sound sounds[MAX_SOUNDS];
    Music music[MAX_MUSIC];
    int sound_count;
    int music_count;
    float master_volume;
    float sound_volume;
    float music_volume;
} AudioSystem;

bool audio_system_init(AudioSystem* system);
void audio_system_cleanup(AudioSystem* system);

int audio_load_sound(AudioSystem* system, const char* file_path);
int audio_load_music(AudioSystem* system, const char* file_path, const char* name);

void audio_play_sound(AudioSystem* system, int sound_id, int loops);
void audio_play_music(AudioSystem* system, const char* name, int loops);
void audio_stop_music(AudioSystem* system);

void audio_set_master_volume(AudioSystem* system, float volume);
void audio_set_sound_volume(AudioSystem* system, float volume);
void audio_set_music_volume(AudioSystem* system, float volume);

#endif // COG_AUDIO_H