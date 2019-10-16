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

	// 쿨타임 오브젝트 만들기 (애니메이션까지..!)
	Vector4 CurObjPos = GetTransform()->Local_Position();
	CurObjPos.z -= 1.0f;

	m_CoolTimeObject = GetScene()->GameObject_Create(L"CoolTimeObject");
	m_CoolTimeObject->GetTransform()->Local_Position(CurObjPos);
	m_CoolTimeObject->GetTransform()->Local_Scale(GetGameObject()->GetTransform()->Local_Scale());
	m_CoolTimeRenderer = m_CoolTimeObject->Component_Add<SpriteRenderer2D>(RenderOrder::UI_ITEM);
	m_CoolTimeRenderer->SetSprite(L"QuickSlot_CoolTime.png", 0);
	m_CoolTimeAniamtion = m_CoolTimeObject->Component_Add<Animation2D>();
	m_CoolTimeScript = m_CoolTimeObject->Component_Add<CoolTimeScript>();

	// 소비템 개수 (넘버)
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

	// 키 입력시
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

	// 개수도 표기해야한다.
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

	// 프레임간 초당 시간 계산후 넘기기 (좀이따가..)
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
	// 스킬 이름 넣기..
	// 블레이징 익스팅션, 포이즌 미스트 등등
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
	// 소비템 코드 넣기
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
	// 소비템일 경우
	// 후에 HP, MP 증가도 만들어준다
	if (nullptr != m_InventoryItem)
	{
		// 체크해주기
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

			// 개수 변화 체크해줘야함(퀵슬롯)
			ConsumNumberCount(ConsumItemCount);

			// 사운드
			if (nullptr != m_SoundPlayer)
			{
				m_SoundPlayer->Play(L"Use_Potion.mp3");
			}
		}
	}
	else
	{
		// 소비템이 아닐 경우  - 빈칸도 체크
		if (nullptr == m_SkillManager)
		{
			return;
		}


		// 쿨타임 체크 -> 쿨타임ing면 스킬 사용X ㅇㅅㅇ
		if (L"Cool" == m_CoolTimeScript->GetCurAnimationName())
		{
			return;
		}

		// 누르면 스킬일 경우 쿨타임 애니메이션 ㄱㄱ
		if (true == Input::Down(m_SeteKey.c_str()))
		{
			// 해당되는 스킬 발사아~!
			if (L"BlazingExtinction" == m_SkillName)
			{
				// 블레이징 익스팅션
				m_SkillManager->UseSkill(m_SkillName);

				// 쿨타임
				m_CoolTimeScript->SetCoolTimeChange(m_CoolTime);		// 15초 쿨타임
				m_CoolTimeScript->ChangeCoolTimeAni();
			}
			else if (L"PoisonMist" == m_SkillName)
			{
				// 포이즌 미스트
				m_SkillManager->UseSkill(m_SkillName);

				m_CoolTimeScript->SetCoolTimeChange(m_CoolTime);		// 20초 쿨타임
				m_CoolTimeScript->ChangeCoolTimeAni();
			}
			else if (L"Blizzard" == m_SkillName)
			{
				// 블리자드
				m_SkillManager->UseSkill(m_SkillName);

				m_CoolTimeScript->SetCoolTimeChange(m_CoolTime);		// 8초 쿨타임
				m_CoolTimeScript->ChangeCoolTimeAni();
			}
			else if (L"SoulPenetration" == m_SkillName)
			{
				// 소울 페네트레이션
				m_SkillManager->UseSkill(m_SkillName);

				m_CoolTimeScript->SetCoolTimeChange(m_CoolTime);		// 5초 쿨타임
				m_CoolTimeScript->ChangeCoolTimeAni();
			}
			else if (L"Cataclysm" == m_SkillName)
			{
				// 카타클리즘
				m_SkillManager->UseSkill(m_SkillName);

				m_CoolTimeScript->SetCoolTimeChange(m_CoolTime);		// 180초 쿨타임
				m_CoolTimeScript->ChangeCoolTimeAni();
			}
			else
			{
				// 만약 해당되는 스킬이 아니라면.. (그럴일이 없도록 해야함)
				EAssert(true);
				return;
			}
		}
	}
}

void QuickSlotScript::ConsumNumberCount(int _Count)
{
	m_Count = _Count;

	int FirstNumber = _Count / 100;												// 100의 자리
	int SecondNumber = (_Count - (FirstNumber * 100)) / 10;						// 10의 자리
	int ThirdNumber = (_Count - (FirstNumber * 100) - (SecondNumber * 10));		// 1의자리

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