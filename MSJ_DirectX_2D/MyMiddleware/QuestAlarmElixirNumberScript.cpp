#include "QuestAlarmElixirNumberScript.h"
#include "Inven_Info.h"
#include "ItemCode.h"
#include <GameObject.h>
#include <Scene.h>

QuestAlarmElixirNumberScript::QuestAlarmElixirNumberScript() : m_Count(0), m_bQuestCom(false)
{
}


QuestAlarmElixirNumberScript::~QuestAlarmElixirNumberScript()
{
}


//

bool QuestAlarmElixirNumberScript::Init()
{
	Vector4 ParentCurPos = GetGameObject()->GetParent()->GetTransform()->Local_Position();
	Vector4 CurPos = GetGameObject()->GetTransform()->Local_Position();
	CurPos += ParentCurPos;
	CurPos.z = -10.0f;
	CurPos.w = 1.0f;

	Vector4 NumberScale = Vector4(7.0f, 9.0f, 1.0f, 1.0f);

	m_UnitNumber = GetScene()->GameObject_Create(L"Quest_ManaElixirUnitNumber");
	m_UnitNumber->GetTransform()->Local_Position(CurPos);
	m_UnitNumber->GetTransform()->Local_Scale(NumberScale);
	m_UnitNumberRenderer = m_UnitNumber->Component_Add<SpriteRenderer2D>(RenderOrder::UI_QUESTDIALOG);
	m_UnitNumberRenderer->SetSprite(L"UI_MoneyNumberWhite.png", 0);

	CurPos.x -= 7.0f;

	m_TensNumber = GetScene()->GameObject_Create(L"Quest_ManaElixirUnitNumber");
	m_TensNumber->GetTransform()->Local_Position(CurPos);
	m_TensNumber->GetTransform()->Local_Scale(NumberScale);
	m_TensNumberRenderer = m_TensNumber->Component_Add<SpriteRenderer2D>(RenderOrder::UI_QUESTDIALOG);
	m_TensNumberRenderer->SetSprite(L"UI_MoneyNumberWhite.png", 0);

	return true;
}

void QuestAlarmElixirNumberScript::Update()
{
	if (0 == m_ConsumeVector.size())
		return;

	NumberCheck();
}

void QuestAlarmElixirNumberScript::FinalUpdate()
{

}
//
void QuestAlarmElixirNumberScript::SetInvenConsumeVector(std::vector<GameObject*> _InvenConsumeVector)
{
	m_ConsumeVector = _InvenConsumeVector;
}

//
void QuestAlarmElixirNumberScript::SetCount(int _Count)
{
	m_Count = _Count;
}

//
void QuestAlarmElixirNumberScript::NumberCheck()
{
	m_Count = 0;

	ItemCode::ItemCodeName CurItemCode = ItemCode::ItemCodeName::EMPTY;

	for (size_t i = 0; i < m_ConsumeVector.size(); ++i)
	{
		CurItemCode = m_ConsumeVector[i]->GetComponent<Inven_Info>()->GetItemCode();

		if (ItemCode::ItemCode::POTION_ELIXIR == CurItemCode)
		{
			int NewCount = m_ConsumeVector[i]->GetComponent<Inven_Info>()->GetItemCount();
			m_Count += NewCount;
		}

	}

	if (m_Count >= 20)
	{
		m_bQuestCom = true;
		//m_Count = 20;
	}

	// 포션 개수 표시하기
	int TensNumberValue = m_Count / 10;

	if ( 0 == TensNumberValue)
	{
		m_TensNumberRenderer->SetSprite(L"EmptyItem.png", 0);
	}
	else
	{
		m_TensNumberRenderer->SetSprite(L"UI_MoneyNumberWhite.png", TensNumberValue);
	}

	int UnitNumberValue = m_Count - (10 * TensNumberValue);

	m_UnitNumberRenderer->SetSprite(L"UI_MoneyNumberWhite.png", UnitNumberValue);

}

//

void QuestAlarmElixirNumberScript::AllNumber_Off()
{
	m_UnitNumber->Update_Off();
	m_TensNumber->Update_Off();
}

void QuestAlarmElixirNumberScript::AllNumber_On()
{
	m_UnitNumber->Update_On();
	m_TensNumber->Update_On();
}

void QuestAlarmElixirNumberScript::Number_Off(int _Value)
{
	switch (_Value)
	{
	case 1:
		m_UnitNumber->Update_Off();
		break;
	case 10:
		m_TensNumber->Update_Off();
		break;
	default:
		break;
	}
}

void QuestAlarmElixirNumberScript::Number_On(int _Value)
{
	switch (_Value)
	{
	case 1:
		m_UnitNumber->Update_On();
		break;
	case 10:
		m_TensNumber->Update_On();
		break;
	default:
		break;
	}
}

//

bool QuestAlarmElixirNumberScript::GetQuestComp()
{
	return m_bQuestCom;
}