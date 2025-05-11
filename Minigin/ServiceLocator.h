#pragma once
#include <memory>
#include <cassert>
namespace dae
{
	class SoundSystem;
	class ServiceLocator final
	{
		static std::unique_ptr<SoundSystem> _ss_instance;
	public:
		static SoundSystem& get_sound_system() 
		{
			return *_ss_instance; 
		}

		static void register_sound_system(std::unique_ptr<SoundSystem>&& ss)
		{
			if (ss != nullptr) _ss_instance = std::move(ss);
			else
			{
				// Unregister it
				_ss_instance.release();
				_ss_instance = nullptr;
			}
		}
	};
}
