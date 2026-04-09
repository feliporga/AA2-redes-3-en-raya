#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>

#define AM AudioManager::GetInstance()

class AudioManager
{
private:
    std::unordered_map<std::string, sf::SoundBuffer*> _clips;
    std::unordered_map<std::string, sf::Music*> _songs;

    // SFML 3.0: Usamos punteros porque no podemos crear un sf::Sound vacío
    std::vector<sf::Sound*> _channels;

    bool _muted = false;

    AudioManager() {}

    ~AudioManager()
    {
        for (auto* channel : _channels) delete channel;
        for (auto const& [name, buffer] : _clips) delete buffer;
        for (auto const& [name, music] : _songs) delete music;
        _channels.clear();
        _clips.clear();
        _songs.clear();
    }

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

public:
    inline static AudioManager& GetInstance()
    {
        static AudioManager am;
        return am;
    }

    inline void LoadClip(std::string name)
    {
        if (_clips.find(name) != _clips.end()) return;

        sf::SoundBuffer* loadedChunk = new sf::SoundBuffer();
        if (!loadedChunk->loadFromFile("resources/audio/sfx/" + name + ".wav")) {
            std::cerr << "Error cargando SFX: " << name << std::endl;
            delete loadedChunk;
            return;
        }

        _clips.emplace(name, loadedChunk);
    }

    inline void LoadSong(std::string name)
    {
        if (_songs.find(name) != _songs.end()) return;

        sf::Music* loadedMusic = new sf::Music();
        if (!loadedMusic->openFromFile("resources/audio/music/" + name + ".mp3")) {
            std::cerr << "Error cargando Musica: " << name << std::endl;
            delete loadedMusic;
            return;
        }

        _songs.emplace(name, loadedMusic);
    }

    inline void PlayClip(std::string name, int repetitions)
    {
        if (_muted || _clips.find(name) == _clips.end()) return;

        // 1. Buscamos un canal que ya exista y haya terminado de sonar
        for (auto* channel : _channels) {
            if (channel->getStatus() == sf::Sound::Status::Stopped) {
                channel->setBuffer(*_clips[name]);
                channel->setLooping(repetitions == -1);
                channel->play();
                return;
            }
        }

        // 2. Si todos los canales están ocupados, creamos uno nuevo (Límite 32)
        if (_channels.size() < 32) {
            sf::Sound* newSound = new sf::Sound(*_clips[name]);
            newSound->setLooping(repetitions == -1);
            newSound->play();
            _channels.push_back(newSound);
        }
    }

    inline void PlaySong(std::string name)
    {
        if (_muted || _songs.find(name) == _songs.end()) return;

        for (auto const& [songName, music] : _songs) {
            music->stop();
        }

        _songs[name]->setLooping(true);
        _songs[name]->play();
    }

    inline void Mute()
    {
        _muted = true;
        StopAudio();
    }

    inline void UnMute()
    {
        _muted = false;
    }

    inline bool GetMuted() { return _muted; }

    inline void StopAudio()
    {
        for (auto* channel : _channels) {
            channel->stop();
        }
        for (auto const& [songName, music] : _songs) {
            music->stop();
        }
    }
};