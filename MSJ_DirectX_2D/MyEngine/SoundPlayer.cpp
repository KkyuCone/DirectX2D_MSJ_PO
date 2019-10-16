#include "SoundPlayer.h"
#include "ResourceManager.h"
#include "SoundResource.h"


SoundPlayer::SoundPlayer() : m_pChannel(nullptr), m_pBGMChannel(nullptr)
{
}


SoundPlayer::~SoundPlayer()
{
}

void SoundPlayer::Play(const wchar_t* _SoundName)
{
	if (nullptr != m_pChannel)
	{
		m_pChannel->stop();
	}

	My_PTR<SoundResource> pSound = ResourceManager<SoundResource>::Find(_SoundName);

	if (nullptr == pSound)
	{
		EAssert(true);
		return;
	}

	SoundResource::m_pSoundSystem->playSound(pSound->Sound(), nullptr, false, &m_pChannel);
}


void SoundPlayer::Play_BGM(const wchar_t* _SoundName)
{
	if (nullptr != m_pBGMChannel)
	{
		m_pBGMChannel->stop();
	}

	My_PTR<SoundResource> pSound = ResourceManager<SoundResource>::Find(_SoundName);

	if (nullptr == pSound)
	{
		EAssert(true);
		return;
	}


	SoundResource::m_pBGMSoundSystem->playSound(pSound->SoundBGM(), nullptr, false, &m_pBGMChannel);
}

//

void SoundPlayer::Stop()
{
	if (nullptr != m_pChannel)
	{
		m_pChannel->stop();
	}
}

void SoundPlayer::Stop_BGM()
{
	if (nullptr != m_pBGMChannel)
	{
		m_pBGMChannel->stop();
	}
}