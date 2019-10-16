#include "SkillManager.h"
#include <Input.h>
#include <SoundPlayer.h>
#include <GameObject.h>
#include <Scene.h>
SkillManager::SkillManager()
	: BEValue(0), m_MapWidth(0.0f), PCCount(0), SPRCount(0), CTCCount(0), BCount(0)
{
}


SkillManager::~SkillManager()
{
}

//
bool SkillManager::Init()
{
	EffectSoundLoading();
	VoiceSoundLoading();
	return true;
}

void SkillManager::Update()
{
	//UseSkill();
}

void SkillManager::FinalUpdate()
{

}

// private
void SkillManager::UseSkill(std::wstring _Name)
{
	if (_Name == L"BlazingExtinction")
	{
		// 블레이징 익스팅션
		Create_BlazingExtinction();
		++BEValue;
	}
	else if (_Name == L"Blizzard")
	{
		Create_Blizzard();
	}
	else if (_Name == L"PoisonMist")
	{
		Create_PoisonMist();
		++PCCount;
	}
	else if (_Name == L"SoulPenetration")
	{
		Create_SoulPenetration();
		++SPRCount;
	}
	else if (_Name == L"Cataclysm")
	{
		Create_Cataclysm();
		++CTCCount;
	}
}



// public

void SkillManager::MonsterGroupDelete()
{
	for (int i = (int)(m_Monsters.size()-1); i >= 0; --i)
	{
		m_Monsters.pop_back();
	}
}

// Get, Set
void SkillManager::SetMonster(GameObject* _Monster)
{
	m_Monsters.push_back(_Monster);
}

void SkillManager::SetPlayerScript(PlayerMove* _Script)
{
	m_pPlayerMoveScript = _Script;
}

void SkillManager::SetMapWidth(float _Width)
{
	m_MapWidth = _Width;
}


void SkillManager::EffectSoundLoading()
{
	m_EffectSoundObject = GetScene()->GameObject_Create(L"EffectSoundPlayer");
	m_EffectSoundObject->GetTransform()->Local_Position(Vector4::Zero);
	m_EffectSoundObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_EffectSoundPlayer = m_EffectSoundObject->Component_Add<SoundPlayer>();
}

void SkillManager::SoundPlay_Effect(std::wstring _SoundFileName)
{
	m_EffectSoundPlayer->Play(_SoundFileName.c_str());
}


void SkillManager::VoiceSoundLoading()
{
	m_VoiceSoundObject = GetScene()->GameObject_Create(L"EffectSoundPlayer");
	m_VoiceSoundObject->GetTransform()->Local_Position(Vector4::Zero);
	m_VoiceSoundObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_VoiceSoundPlayer = m_EffectSoundObject->Component_Add<SoundPlayer>();
}
void SkillManager::SoundPlaye_Voice(std::wstring _SoundFileName)
{
	m_VoiceSoundPlayer->Play(_SoundFileName.c_str());
}
