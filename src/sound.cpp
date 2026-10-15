#include <print>

#include "sound.hpp"

// @Section Public

SoundManager& SoundManager::get(void) {
    static SoundManager sm;
    return sm;
}

void SoundManager::remove_all_sounds(void) {
    for (auto& sfx : m_sfx) {
        UnloadSound(sfx.second);
    }
    m_sfx.clear();
}

void SoundManager::add_sound(const unsigned char* sfx_mem, std::size_t sfx_mem_size, std::string sfx_name) {
    if (m_sfx.find(sfx_name) != m_sfx.end()) {
        std::println(stderr, "SFX: {} already exists", sfx_name);
        return;
    }

    // TODO: Write die wrapper for exit
    Wave wav = LoadWaveFromMemory(".ogg", sfx_mem, sfx_mem_size);
    if (!IsWaveValid(wav)) exit(-1);
    Sound sfx = LoadSoundFromWave(wav);
    if (!IsSoundValid(sfx)) exit(-1);
    UnloadWave(wav);

    SetSoundVolume(sfx, m_volume);
    m_sfx.insert({sfx_name, sfx});
}

void SoundManager::set_volume(float volume) {
    m_volume = volume;
    for (auto& sfx : m_sfx) {
        SetSoundVolume(sfx.second, volume);
    }
}

float SoundManager::get_volume(void) {
    return m_volume;
}

void SoundManager::play_sound(std::string sfx_name) {
    std::optional<std::reference_wrapper<Sound>> sfx = get_sound(sfx_name);
    if (sfx.has_value()) {
        PlaySound(sfx.value());
    }
}

// @Section Private

SoundManager::SoundManager(void) {
    m_volume = 1.0f;
}

std::optional<std::reference_wrapper<Sound>> SoundManager::get_sound(std::string sfx_name) {
    if (m_sfx.find(sfx_name) != m_sfx.end()) {
        return m_sfx.at(sfx_name);
    }
    return {};
}
