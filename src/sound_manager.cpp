#include "sound_manager.h"

SoundManager::SoundManager() :
    current_burner_state(BurnerState::OFF),
    current_valve_state(ValveState::CLOSED),
    music_state(true),
    mixer(nullptr),
    sample_burner_init(nullptr),
    sample_burner_loop(nullptr),
    sample_burner_stop(nullptr),
    sample_valve_open(nullptr),
    sample_wind_noise(nullptr)
{
    // Create a new mixer
    mixer = al_create_mixer(
        44100,
        ALLEGRO_AUDIO_DEPTH_FLOAT32,
        ALLEGRO_CHANNEL_CONF_2);
}

ALLEGRO_SAMPLE_INSTANCE* SoundManager::load_sample_data(const char* filename)
{
    // Load the sample data and check that it is valid
    ALLEGRO_SAMPLE* sample_data = al_load_sample(filename);

    if (sample_data == nullptr)
    {
        return nullptr;
    }

    vec_sample_data.push_back(sample_data);

    // Otherwise, define the instance data
    ALLEGRO_SAMPLE_INSTANCE* sample = al_create_sample_instance(sample_data);

    if (sample == nullptr)
    {
        return nullptr;
    }

    vec_sample_instances.push_back(sample);

    // Attempt to attach to the mixer
    if (!al_attach_sample_instance_to_mixer(sample, mixer))
    {
        return nullptr;
    }

    // Return the sample if successful
    return sample;
}

ALLEGRO_AUDIO_STREAM* SoundManager::load_audio_stream(const char* filename)
{
    ALLEGRO_AUDIO_STREAM* stream = al_load_audio_stream(filename, 4, 2048);
    if (stream != nullptr)
    {
        vec_streams.push_back(stream);
        al_set_audio_stream_playing(stream, false);
        al_attach_audio_stream_to_mixer(stream, mixer);
    }
    return stream;
}

bool SoundManager::init()
{
    load_audio_stream("music/gymnopedie_no_1.ogg");
    load_audio_stream("music/gymnopedie_no_2.ogg");
    load_audio_stream("music/gymnopedie_no_3.ogg");

    sample_burner_init = load_sample_data("sounds/burner_start.ogg");
    sample_burner_loop = load_sample_data("sounds/burner_loop.ogg");
    sample_burner_stop = load_sample_data("sounds/burner_end.ogg");
    sample_valve_open = load_sample_data("sounds/hiss.ogg");
    sample_wind_noise = load_sample_data("sounds/wind.ogg");

    al_set_sample_instance_playmode(
        sample_burner_loop,
        ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_playmode(
        sample_wind_noise,
        ALLEGRO_PLAYMODE_LOOP);

    al_set_sample_instance_gain(
        sample_wind_noise,
        0.05);

    if (music_state && vec_streams.size() > 0)
    {
        al_set_audio_stream_playing(vec_streams.front(), true);
    }

    return 
        sample_burner_init != nullptr &&
        sample_burner_loop != nullptr &&
        sample_burner_stop != nullptr &&
        sample_valve_open != nullptr &&
        sample_wind_noise != nullptr;
}

ALLEGRO_MIXER* SoundManager::get_mixer()
{
    return mixer;
}

void SoundManager::set_burner_state(const BurnerState state)
{
    if (state != current_burner_state)
    {
        al_stop_sample_instance(sample_burner_init);
        al_stop_sample_instance(sample_burner_loop);
        al_stop_sample_instance(sample_burner_stop);

        if (state == BurnerState::ON)
        {
            al_play_sample_instance(sample_burner_init);
        }
        else if (state == BurnerState::OFF)
        {
            al_play_sample_instance(sample_burner_stop);
        }

        current_burner_state = state;
    }

    if (current_burner_state == BurnerState::ON && !al_get_sample_instance_playing(sample_burner_init) && !al_get_sample_instance_playing(sample_burner_loop))
    {
        al_play_sample_instance(sample_burner_loop);
    }
}

void SoundManager::set_valve_state(const ValveState state)
{
    if (state != current_valve_state)
    {
        if (state == ValveState::OPEN)
        {
            al_set_sample_instance_playmode(sample_valve_open, ALLEGRO_PLAYMODE_LOOP);
            al_play_sample_instance(sample_valve_open);
        }
        else if (state == ValveState::CLOSED)
        {
            al_stop_sample_instance(sample_valve_open);
        }

        current_valve_state = state;
    }
}

void SoundManager::set_music_state(const bool state)
{
    if (music_state != state)
    {
        ALLEGRO_AUDIO_STREAM* stream = vec_streams.front();

        if (state)
        {
            al_set_audio_stream_playing(stream, true);
        }
        else
        {
            al_set_audio_stream_playing(stream, false);
            al_rewind_audio_stream(stream);
        }

        music_state = state;
    }
}

bool SoundManager::get_music_state() const
{
    return music_state;
}

void SoundManager::update_background()
{
    if (!al_get_sample_instance_playing(sample_wind_noise))
    {
        al_play_sample_instance(sample_wind_noise);
    }

    if (music_state && vec_streams.size() > 0 && !al_get_audio_stream_playing(vec_streams.front()))
    {
        ALLEGRO_AUDIO_STREAM* stream = vec_streams.front();
        al_rewind_audio_stream(stream);
        vec_streams.erase(vec_streams.begin(), vec_streams.begin() + 1);
        vec_streams.push_back(stream);

        al_set_audio_stream_playing(vec_streams.front(), true);
    }
}

SoundManager::~SoundManager()
{
    // Clear all referenced items
    sample_burner_loop = nullptr;
    sample_valve_open = nullptr;
    sample_wind_noise = nullptr;
    sample_burner_init = nullptr;
    sample_burner_stop = nullptr;

    // Detach and destory all sample instances
    for (auto it = vec_sample_instances.begin(); it != vec_sample_instances.end(); ++it)
    {
        if (*it != nullptr)
        {
            al_destroy_sample_instance(*it);
        }
    }

    vec_sample_instances.clear();

    for (auto it = vec_sample_data.begin(); it != vec_sample_data.end(); ++it)
    {
        if (*it != nullptr)
        {
            al_destroy_sample(*it);
        }
    }

    vec_sample_data.clear();

    for (auto it = vec_streams.begin(); it != vec_streams.end(); ++it)
    {
        if (*it != nullptr)
        {
            al_destroy_audio_stream(*it);
        }
    }

    // Destroy the mixer
    al_destroy_mixer(mixer);
}
