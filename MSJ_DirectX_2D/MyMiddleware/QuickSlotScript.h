#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>
#include "ItemCode.h"

class GameObject;
class CoolTimeScript;
class SkillManager;
class SoundPlayer;
class QuickSlotScript : public Script
{
private:
	float m_CoolTime;
	int m_Count;
	std::wstring m_SeteKey;


	// 스킬이름, 아이템코드
	std::wstring m_SkillName;
	ItemCode::ItemCodeName m_ConsumeCodeName;

	SoundPlayer* m_SoundPlayer;

private:
	// 소비템 개수
	My_PTR<GameObject> m_HundredNumber;
	My_PTR<GameObject> m_TensNumber;
	My_PTR<GameObject> m_UnitNumber;

	My_PTR<SpriteRenderer2D> HundredNumberRenderer;
	My_PTR<SpriteRenderer2D> TensNumberRenderer;
	My_PTR<SpriteRenderer2D> UnitNumberRenderer;

private:
	SkillManager* m_SkillManager;

private:
	My_PTR<SpriteRenderer2D> m_Renderer;		// 본 오브젝트 렌더러

	// 각각의 퀵슬롯에서 쿨타임을 돌려줘야한다. 쿨타임 오브젝트 추가
	My_PTR<GameObject> m_CoolTimeObject;
	My_PTR<SpriteRenderer2D> m_CoolTimeRenderer;
	My_PTR<Animation2D> m_CoolTimeAniamtion;
	My_PTR<CoolTimeScript> m_CoolTimeScript;

	// 소비템일 경우 인벤토리에 있는 소비템에도 영향을 주기 때문에 알아야한다.
	GameObject* m_InventoryItem;

private:
	void PushKeyCheck();
	void ConsumNumberCount(int _Count);

public:
	ItemCode::ItemCodeName GetCodeName();
	float GetCoolTime();
	std::wstring GetSkillName();
	int GetCount();
	GameObject* GetInventoryItem();
	SkillManager* GetSkillManager();
	CoolTimeScript* GetCoolTimeScript();

public:
	void SetInventoryItem(GameObject* _Obj);
	void SetKey(std::wstring _Key);
	void SetCoolTime(float _Time);
	void SetSkillManager(SkillManager* _SkillManager);
	void SetSkillName(std::wstring _SkillName);
	void SetConsumeCodeName(ItemCode::ItemCodeName _Code);
	void SetSoundPlayer(SoundPlayer* _SoundPlayer);
	
public:
	void ChangeTexture(std::wstring _ImageName);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;


public:
	QuickSlotScript();
	~QuickSlotScript();
};

