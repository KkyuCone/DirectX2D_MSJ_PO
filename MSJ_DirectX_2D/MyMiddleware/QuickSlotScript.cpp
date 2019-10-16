#include "QuickSlotScript.h"
#include <Scene.h>
#include "CoolTimeScript.h"
#include "Inven_Info.h"
#include "SkillManager.h"
#include "Inven_Info.h"
#include "RenderOrder.h"
#include <SoundPlayer.h>

QuickSlotScript::QuickSlotScript() 
	: m_CoolTimeObject(nullptr), m_InventoryItem(nullptr), m_SeteKey(L""), m_CoolTime(0.1f), m_SkillManager(nullptr)
	, m_SkillName(L""), m_ConsumeCodeName(ItemCode::ItemCodeName::EMPTY), m_Count(0), m_SoundPlayer(nullptr)
{
}


QuickSlotScript::~QuickSlotScript()
{
}

//

bool QuickSlotScript::Init()
{
	m_Renderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_Renderer)
	{
		EAssert(true);
		return false;
	}

	// ��Ÿ�� ������Ʈ ����� (�ִϸ��̼Ǳ���..!)
	Vector4 CurObjPos = GetTransform()->Local_Position();
	CurObjPos.z -= 1.0f;

	m_CoolTimeObject = GetScene()->GameObject_Create(L"CoolTimeObject");
	m_CoolTimeObject->GetTransform()->Local_Position(CurObjPos);
	m_CoolTimeObject->GetTransform()->Local_Scale(GetGameObject()->GetTransform()->Local_Scale());
	m_CoolTimeRenderer = m_CoolTimeObject->Component_Add<SpriteRenderer2D>(RenderOrder::UI_ITEM);
	m_CoolTimeRenderer->SetSprite(L"QuickSlot_CoolTime.png", 0);
	m_CoolTimeAniamtion = m_CoolTimeObject->Component_Add<Animation2D>();
	m_CoolTimeScript = m_CoolTimeObject->Component_Add<CoolTimeScript>();

	// �Һ��� ���� (�ѹ�)
	CurObjPos = GetTransform()->Local_Position();
	CurObjPos.y -= 10.0f;
	CurObjPos.x -= 5.0f;
	CurObjPos.z -= 1.0f;

	m_HundredNumber = GetScene()->GameObject_Create(L"QuickSlot_HundredNumber");
	m_HundredNumber->GetTransform()->Local_Position(CurObjPos);
	m_HundredNumber->GetTransform()->Local_Scale(Vector4(7.0f, 9.0f, 1.0f, 1.0f));
	HundredNumberRenderer = m_HundredNumber->Component_Add<SpriteRenderer2D>(RenderOrder::UI_ITEM);
	HundredNumberRenderer->SetSprite(L"EmptyItem.png" , 0);

	CurObjPos.x += 7.0f;
	m_TensNumber = GetScene()->GameObject_Create(L"QuickSlot_TensNumber");
	m_TensNumber->GetTransform()->Local_Position(CurObjPos);
	m_TensNumber->GetTransform()->Local_Scale(Vector4(7.0f, 9.0f, 1.0f, 1.0f));
	TensNumberRenderer = m_TensNumber->Component_Add<SpriteRenderer2D>(RenderOrder::UI_ITEM);
	TensNumberRenderer->SetSprite(L"EmptyItem.png", 0);

	CurObjPos.x += 7.0f;
	m_UnitNumber = GetScene()->GameObject_Create(L"QuickSlot_UnitNumber");
	m_UnitNumber->GetTransform()->Local_Position(CurObjPos);
	m_UnitNumber->GetTransform()->Local_Scale(Vector4(7.0f, 9.0f, 1.0f, 1.0f));
	UnitNumberRenderer = m_UnitNumber->Component_Add<SpriteRenderer2D>(RenderOrder::UI_ITEM);
	UnitNumberRenderer->SetSprite(L"EmptyItem.png", 0);

	return true;
}

void QuickSlotScript::Update()
{
	if (L"" == m_SeteKey)
	{
		return;
	}

	// Ű �Է½�
	if (true == Input::Down(m_SeteKey.c_str()))
	{
		PushKeyCheck();
	}

}

void QuickSlotScript::FinalUpdate()
{

}
//
CoolTimeScript* QuickSlotScript::GetCoolTimeScript()
{
	return m_CoolTimeScript;
}


ItemCode::ItemCodeName QuickSlotScript::GetCodeName()
{
	ItemCode::ItemCodeName aaa = m_ConsumeCodeName;

	return m_ConsumeCodeName;
}

float QuickSlotScript::GetCoolTime()
{
	return m_CoolTime;
}

std::wstring QuickSlotScript::GetSkillName()
{
	return m_SkillName;
}

int QuickSlotScript::GetCount()
{
	return m_Count;
}

GameObject* QuickSlotScript::GetInventoryItem()
{
	return m_InventoryItem;
}

//
void QuickSlotScript::SetInventoryItem(GameObject* _Obj)
{
	m_InventoryItem = _Obj;

	if (nullptr == _Obj)
	{
		ConsumNumberCount(0);
		return;
	}

	// ������ ǥ���ؾ��Ѵ�.
	int ItemCount = _Obj->GetComponent<Inven_Info>()->GetItemCount();
	ConsumNumberCount(ItemCount);
}

void QuickSlotScript::SetKey(std::wstring _Key)
{
	m_SeteKey = _Key;
}

void QuickSlotScript::SetCoolTime(float _Time)
{
	m_CoolTime = _Time;

	// �����Ӱ� �ʴ� �ð� ����� �ѱ�� (���̵���..)
	float ChangeTime = m_CoolTime / 17;
}

void QuickSlotScript::SetSkillManager(SkillManager* _SkillManager)
{
	m_SkillManager = _SkillManager;


}

SkillManager* QuickSlotScript::GetSkillManager()
{
	return m_SkillManager;
}

void QuickSlotScript::SetSkillName(std::wstring _SkillName)
{
	// ��ų �̸� �ֱ�..
	// ����¡ �ͽ��ü�, ������ �̽�Ʈ ���
	m_SkillName = _SkillName;

	if (L"" == _SkillName)
	{
		return;
	}
	else
	{
		if (L"BlazingExtinction" == _SkillName)
		{
			SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_BE);
		}
		else if (L"PoisonMist" == _SkillName)
		{
			SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_PM);
		}
		else if (L"Blizzard" == _SkillName)
		{
			SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_BZ);
		}
		else if (L"SoulPenetration" == _SkillName)
		{
			SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_SP);
		}
		else if (L"Cataclysm" == _SkillName)
		{
			SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_CC);
		}
		return;
	}
}

void QuickSlotScript::SetConsumeCodeName(ItemCode::ItemCodeName _Code)
{
	// �Һ��� �ڵ� �ֱ�
	m_ConsumeCodeName = _Code;

	switch (m_ConsumeCodeName)
	{
	case ItemCode::EMPTY:
		m_Renderer->SetSprite(L"EmptyItem.png", 0);
		break;
	case ItemCode::POTION_RED:
		m_Renderer->SetSprite(L"RedPoiton.png", 0);
		break;
	case ItemCode::POTION_BLUE:
		m_Renderer->SetSprite(L"BluePoiton.png", 0);
		break;
	case ItemCode::POTION_WHITE:
		m_Renderer->SetSprite(L"WhitePotion.png", 0);
		break;
	case ItemCode::POTION_ORANGE:
		m_Renderer->SetSprite(L"OrangePotion.png", 0);
		break;
	case ItemCode::POTION_ELIXIR:
		m_Renderer->SetSprite(L"02000006.info.icon.png", 0);
		break;
	case ItemCode::SKILL_BE:
		m_Renderer->SetSprite(L"BlazingExtinction_Icon.png", 0);
		break;
	case ItemCode::SKILL_PM:
		m_Renderer->SetSprite(L"PoisonMist_Icon.png", 0);
		break;
	case ItemCode::SKILL_SP:
		m_Renderer->SetSprite(L"SoulPenetration_Icon.png", 0);
		break;
	case ItemCode::SKILL_CC:
		m_Renderer->SetSprite(L"Cataclysm_Icon.png", 0);
		break;
	case ItemCode::SKILL_BZ:
		m_Renderer->SetSprite(L"Blizzard_Icon.png", 0);
		break;
	default:
		break;
	}
}


//
void QuickSlotScript::PushKeyCheck() 
{
	// �Һ����� ���
	// �Ŀ� HP, MP ������ ������ش�
	if (nullptr != m_InventoryItem)
	{
		// üũ���ֱ�
		if(ItemCode::ItemCodeName::EMPTY == m_ConsumeCodeName)
		{
			EAssert(true);
			return;
		}

		int ConsumItemCount = m_InventoryItem->GetComponent<Inven_Info>()->GetItemCount();
		if (0 == ConsumItemCount)
		{
			return;
		}
		else
		{
			--ConsumItemCount;

			m_InventoryItem->GetComponent<Inven_Info>()->SetItemCount(ConsumItemCount);

			// ���� ��ȭ üũ�������(������)
			ConsumNumberCount(ConsumItemCount);

			// ����
			if (nullptr != m_SoundPlayer)
			{
				m_SoundPlayer->Play(L"Use_Potion.mp3");
			}
		}
	}
	else
	{
		// �Һ����� �ƴ� ���  - ��ĭ�� üũ
		if (nullptr == m_SkillManager)
		{
			return;
		}


		// ��Ÿ�� üũ -> ��Ÿ��ing�� ��ų ���X ������
		if (L"Cool" == m_CoolTimeScript->GetCurAnimationName())
		{
			return;
		}

		// ������ ��ų�� ��� ��Ÿ�� �ִϸ��̼� ����
		if (true == Input::Down(m_SeteKey.c_str()))
		{
			// �ش�Ǵ� ��ų �߻��~!
			if (L"BlazingExtinction" == m_SkillName)
			{
				// ����¡ �ͽ��ü�
				m_SkillManager->UseSkill(m_SkillName);

				// ��Ÿ��
				m_CoolTimeScript->SetCoolTimeChange(m_CoolTime);		// 15�� ��Ÿ��
				m_CoolTimeScript->ChangeCoolTimeAni();
			}
			else if (L"PoisonMist" == m_SkillName)
			{
				// ������ �̽�Ʈ
				m_SkillManager->UseSkill(m_SkillName);

				m_CoolTimeScript->SetCoolTimeChange(m_CoolTime);		// 20�� ��Ÿ��
				m_CoolTimeScript->ChangeCoolTimeAni();
			}
			else if (L"Blizzard" == m_SkillName)
			{
				// ���ڵ�
				m_SkillManager->UseSkill(m_SkillName);

				m_CoolTimeScript->SetCoolTimeChange(m_CoolTime);		// 8�� ��Ÿ��
				m_CoolTimeScript->ChangeCoolTimeAni();
			}
			else if (L"SoulPenetration" == m_SkillName)
			{
				// �ҿ� ���Ʈ���̼�
				m_SkillManager->UseSkill(m_SkillName);

				m_CoolTimeScript->SetCoolTimeChange(m_CoolTime);		// 5�� ��Ÿ��
				m_CoolTimeScript->ChangeCoolTimeAni();
			}
			else if (L"Cataclysm" == m_SkillName)
			{
				// īŸŬ����
				m_SkillManager->UseSkill(m_SkillName);

				m_CoolTimeScript->SetCoolTimeChange(m_CoolTime);		// 180�� ��Ÿ��
				m_CoolTimeScript->ChangeCoolTimeAni();
			}
			else
			{
				// ���� �ش�Ǵ� ��ų�� �ƴ϶��.. (�׷����� ������ �ؾ���)
				EAssert(true);
				return;
			}
		}
	}
}

void QuickSlotScript::ConsumNumberCount(int _Count)
{
	m_Count = _Count;

	int FirstNumber = _Count / 100;												// 100�� �ڸ�
	int SecondNumber = (_Count - (FirstNumber * 100)) / 10;						// 10�� �ڸ�
	int ThirdNumber = (_Count - (FirstNumber * 100) - (SecondNumber * 10));		// 1���ڸ�

	if (0 == FirstNumber)
	{
		HundredNumberRenderer->SetSprite(L"EmptyItem.png", 0);
	}
	else
	{
		HundredNumberRenderer->SetSprite(L"UI_StateNumber.png", FirstNumber);
	}

	if (0 == FirstNumber && 0 == SecondNumber)
	{
		TensNumberRenderer->SetSprite(L"EmptyItem.png", 0);
	}
	else
	{
		TensNumberRenderer->SetSprite(L"UI_StateNumber.png", SecondNumber);
	}

	if (0 == FirstNumber && 0 == SecondNumber && 0 == ThirdNumber)
	{
		UnitNumberRenderer->SetSprite(L"EmptyItem.png", 0);
		m_Renderer->SetSprite(L"EmptyItem.png", 0);
	}
	else
	{
		UnitNumberRenderer->SetSprite(L"UI_StateNumber.png", ThirdNumber);
	}
}


//
void QuickSlotScript::ChangeTexture(std::wstring _ImageName)
{
	m_Renderer->SetSprite(_ImageName.c_str(), 0);
}

void QuickSlotScript::SetSoundPlayer(SoundPlayer* _SoundPlayer)
{
	m_SoundPlayer = _SoundPlayer;
}