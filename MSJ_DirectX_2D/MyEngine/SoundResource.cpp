#include "SoundResource.h"

FMOD::System* SoundResource::m_pSoundSystem = nullptr;
FMOD::System* SoundResource::m_pBGMSoundSystem = nullptr;
SoundResource::Destroy SoundResource::m_Destroy;

//

void SoundResource::SoundInit()
{
	FMOD::System_Create(&m_pSoundSystem);

	if (nullptr == m_pSoundSystem)
	{
		EAssert(true);
		return;
	}

	// 최대 소리를 1000개 이상을 재생할수있음
	FMOD_RESULT Return = m_pSoundSystem->init(32, FMOD_DEFAULT, nullptr);
	
	if (FMOD_OK != Return)
	{
		EAssert(true);
	}
}

void SoundResource::SoundBGMInit()
{
	FMOD::System_Create(&m_pBGMSoundSystem);

	if (nullptr == m_pBGMSoundSystem)
	{
		EAssert(true);
		return;
	}

	// 최대 소리를 1000개 이상을 재생할수있음
	FMOD_RESULT Return = m_pBGMSoundSystem->init(32, FMOD_LOOP_NORMAL, nullptr);

	if (FMOD_OK != Return)
	{
		EAssert(true);
	}
}

void SoundResource::SoundDestroy()
{
	// 지우기
	if (nullptr != m_pSoundSystem)
	{
		m_pSoundSystem->release();
	}
}


void SoundResource::SoundBGMDestroy()
{
	// 지우기
	if (nullptr != m_pBGMSoundSystem)
	{
		m_pBGMSoundSystem->release();
	}
}
//

SoundResource::SoundResource()
{
}


SoundResource::~SoundResource()
{
	if (nullptr != m_pSound)
	{
		//m_pSound->release();
	}
}

//

bool SoundResource::Load()
{
	std::string SoundPath = CW2A(GetPath().c_str());

	if (FMOD_OK != m_pSoundSystem->createSound(SoundPath.c_str(), FMOD_DEFAULT, nullptr, &m_pSound))
	{
		EAssert(true);
		return false;
	}

	if (nullptr == m_pSound)
	{
		EAssert(true);
		return false;
	}

	m_pSound->getLength(&Length, FMOD_TIMEUNIT_MS);

	//

	std::string SoundBGMPath = CW2A(GetPath().c_str());

	if (FMOD_OK != m_pBGMSoundSystem->createSound(SoundBGMPath.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_pSoundBGM))
	{
		EAssert(true);
		return false;
	}

	if (nullptr == m_pSoundBGM)
	{
		EAssert(true);
		return false;
	}

	m_pSoundBGM->getLength(&Length, FMOD_TIMEUNIT_MS);

	return true;
}

void SoundResource::SoundUpdate()
{
	if (nullptr != m_pSoundSystem)
	{
		m_pSoundSystem->update();
	}
}


void SoundResource::SoundBGMUpdate()
{
	if (nullptr != m_pBGMSoundSystem)
	{
		m_pBGMSoundSystem->update();
	}
}