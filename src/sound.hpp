#ifndef SOUND_ENGINE_HPP_
#define SOUND_ENGINE_HPP_

#include <cstddef>
#include <cstdlib>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <raylib.h>

// Singleton
class SoundManager {
public:
    static SoundManager& get(void);

    void  unload_all_sounds(void);
    void  add_sound(const unsigned char* sfx_mem, std::size_t sfx_mem_size, std::string sfx_name);
    void  set_volume(float volume);
    float get_volume(void);
    void  play_sound(std::string sfx_name);
private:
    std::unordered_map<std::string, Sound> m_sfx;
    float m_volume;

    SoundManager(void);
    std::optional<std::reference_wrapper<Sound>> get_sound(std::string sfx_name);
};

#endif // SOUND_ENGINE_HPP_
