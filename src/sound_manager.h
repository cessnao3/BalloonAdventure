#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <vector>

#include <allegro5/allegro_audio.h>

class SoundManager
{
public:
    enum class BurnerState
    {
        OFF = 0,
        ON = 1
    };

    enum class ValveState
    {
        CLOSED = 0,
        OPEN = 1
    };

public:
    SoundManager();

    bool init();

    void set_burner_state(const BurnerState state);

    void set_valve_state(const ValveState state);

    void set_music_state(const bool state);

    bool get_music_state() const;

    void update_background();

    ALLEGRO_MIXER* get_mixer();

    ~SoundManager();

protected:
    ALLEGRO_SAMPLE_INSTANCE* load_sample_data(const char* filename);
    ALLEGRO_AUDIO_STREAM* load_audio_stream(const char* filename);

protected:
    BurnerState current_burner_state;
    ValveState current_valve_state;

    bool music_state;

    ALLEGRO_MIXER* mixer;

    std::vector<ALLEGRO_SAMPLE*> vec_sample_data;
    std::vector<ALLEGRO_SAMPLE_INSTANCE*> vec_sample_instances;

    std::vector<ALLEGRO_AUDIO_STREAM*> vec_streams;

    ALLEGRO_SAMPLE_INSTANCE* sample_burner_init;
    ALLEGRO_SAMPLE_INSTANCE* sample_burner_loop;
    ALLEGRO_SAMPLE_INSTANCE* sample_burner_stop;
    ALLEGRO_SAMPLE_INSTANCE* sample_valve_open;
    ALLEGRO_SAMPLE_INSTANCE* sample_wind_noise;
};

#endif // SOUND_MANAGER_H
