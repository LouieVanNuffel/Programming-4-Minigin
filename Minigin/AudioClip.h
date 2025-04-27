#pragma once
#include <SDL_mixer.h>
#include <string>
#include <cassert>
#include <iostream>

namespace dae
{
    class AudioClip
    {
    public:
        AudioClip(const std::string& filepath)
            : m_FilePath(filepath), m_Chunk(nullptr) {}

        ~AudioClip()
        {
            if (m_Chunk)
                Mix_FreeChunk(m_Chunk);
        }

        bool IsLoaded() const 
        { 
            return m_Chunk != nullptr; 
        }

        void Load()
        {
            if (!m_Chunk)
                m_Chunk = Mix_LoadWAV(m_FilePath.c_str());

            if (!m_Chunk)
                std::cerr << "Mix_LoadWAV failed: " << Mix_GetError() << std::endl;
        }

        void Play()
        {
            if (m_Chunk) Mix_PlayChannel(-1, m_Chunk, 0); // Play once on a free channel
        }

        void SetVolume(float volume)
        {
            if (m_Chunk)
            {
                Mix_VolumeChunk(m_Chunk, static_cast<int>(volume));
            }
        }

    private:
        std::string m_FilePath;
        Mix_Chunk* m_Chunk;
    };
}

