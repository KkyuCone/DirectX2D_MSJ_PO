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


	// ��ų�̸�, �������ڵ�
	std::wstring m_SkillName;
	ItemCode::ItemCodeName m_ConsumeCodeName;

	SoundPlayer* m_SoundPlayer;

private:
	// �Һ��� ����
	My_PTR<GameObject> m_HundredNumber;
	My_PTR<GameObject> m_TensNumber;
	My_PTR<GameObject> m_UnitNumber;

	My_PTR<SpriteRenderer2D> HundredNumberRenderer;
	My_PTR<SpriteRenderer2D> TensNumberRenderer;
	My_PTR<SpriteRenderer2D> UnitNumberRenderer;

private:
	SkillManager* m_SkillManager;

private:
	My_PTR<SpriteRenderer2D> m_Renderer;		// �� ������Ʈ ������

	// ������ �����Կ��� ��Ÿ���� ��������Ѵ�. ��Ÿ�� ������Ʈ �߰�
	My_PTR<GameObject> m_CoolTimeObject;
	My_PTR<SpriteRenderer2D> m_CoolTimeRenderer;
	My_PTR<Animation2D> m_CoolTimeAniamtion;
	My_PTR<CoolTimeScript> m_CoolTimeScript;

	// �Һ����� ��� �κ��丮�� �ִ� �Һ��ۿ��� ������ �ֱ� ������ �˾ƾ��Ѵ�.
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

