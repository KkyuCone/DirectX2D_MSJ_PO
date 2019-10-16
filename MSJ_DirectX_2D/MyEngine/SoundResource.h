#pragma once
#include "Resource.h"
#include <fmod.hpp>

#ifdef Debugx32
#pragma comment(lib, "fmod_vc")
#elif Debugx64
#pragma comment(lib, "fmod64_vc.lib")
#elif Releasex32
#pragma comment(lib, "fmod_vc")
#elif Releasex64
#pragma comment(lib, "fmod64_vc.lib")
#endif

class SoundResource : public Resource
{
public:
	friend class SoundPlayer;
	friend class Core;

private:
	class Destroy
	{
	public:
		~Destroy()
		{
			SoundResource::SoundDestroy();
			SoundResource::SoundBGMDestroy();
		}
	};
	friend SoundResource::Destroy;
	static Destroy m_Destroy;

private:
	static FMOD::System* m_pSoundSystem;
	static FMOD::System* m_pBGMSoundSystem;

private:
	static void SoundInit();
	static void SoundBGMInit();

	static void SoundDestroy();
	static void SoundBGMDestroy();

public:
	static void SoundUpdate();
	static void SoundBGMUpdate();

private:
	FMOD::Sound* m_pSound;
	FMOD::Sound* m_pSoundBGM;

	UINT Length;

public:
	DEFINITION_LOADER(SoundResource, std::wstring)
		DEFINITION_PATH_LOADER_FUNCTION()
		DEFINITION_PATH_LOADER_FUNCTION_PARAMETER()
	DEFINITION_LOADER_END

public:
	FMOD::Sound* Sound()
	{
		return m_pSound;
	}


	FMOD::Sound* SoundBGM()
	{
		return m_pSoundBGM;
	}
public:
	bool Load();

public:
	SoundResource();
	~SoundResource();
};

