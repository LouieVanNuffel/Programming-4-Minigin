#pragma once
#include "SDL_AudioImpl.cpp"

namespace dae
{
    class AudioClip
    {
    public:
        AudioClip(const std::string& filepath)
        {
            m_pSDL_AudioImpl = new SDL_AudioImpl(filepath);
        }

        ~AudioClip()
        {
            delete m_pSDL_AudioImpl;
            m_pSDL_AudioImpl = nullptr;
        }

        bool IsLoaded() const 
        { 
            return m_pSDL_AudioImpl->IsLoaded();
        }

        void Load()
        {
            m_pSDL_AudioImpl->Load();
        }

        void Play()
        {
            m_pSDL_AudioImpl->Play();
        }

        void SetVolume(float volume)
        {
            m_pSDL_AudioImpl->SetVolume(volume);
        }

    private:
        SDL_AudioImpl* m_pSDL_AudioImpl;
    };
}

