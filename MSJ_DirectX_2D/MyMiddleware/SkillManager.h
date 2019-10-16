#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include <GameObject.h>

#include "RenderOrder.h"

#define BLIZZARDRANGE 500

class PlayerMove;
class SoundPlayer;
class SkillManager : public Script, public RenderOrder
{
private:
	My_PTR<GameObject> m_EffectSoundObject;
	My_PTR<SoundPlayer> m_EffectSoundPlayer;

	My_PTR<GameObject> m_VoiceSoundObject;
	My_PTR<SoundPlayer> m_VoiceSoundPlayer;

private:
	float m_MapWidth;
	int BEValue;			// ����¡ �ͽ��ü�
	int PCCount;			// ������ �̽�Ʈ
	int SPRCount;			// �ҿ� �۳�Ʈ���̼�
	int CTCCount;			// īŸŬ����
	int BCount;				// ���ڵ�
	
private:
	PlayerMove* m_pPlayerMoveScript;
	std::vector<GameObject*> m_Monsters;

private:
	void EffectSoundLoading();
	void SoundPlay_Effect(std::wstring _SoundFileName);

	void VoiceSoundLoading();
	void SoundPlaye_Voice(std::wstring _SoundFileName);

private:
	void Create_BlazingExtinction();
	void Create_Blizzard();
	void Create_PoisonMist();
	void Create_SoulPenetration();
	void Create_Cataclysm();

public:
	void UseSkill(std::wstring _Name);

public:
	void MonsterGroupDelete();
	void SetMonster(GameObject* _Monster);
	void SetPlayerScript(PlayerMove* _Script);
	void SetMapWidth(float _Width);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	SkillManager();
	~SkillManager();
};

