#pragma once
#include "Component.h"
#include <fmod.hpp>

class SoundPlayer : public Component
{
private:
	FMOD::Channel* m_pChannel;
	FMOD::Channel* m_pBGMChannel;

public:
	void Play(const wchar_t* _SoundName);
	void Play_BGM(const wchar_t* _SoundName);
	void Stop();
	void Stop_BGM();

public:
	SoundPlayer();
	~SoundPlayer();
};

