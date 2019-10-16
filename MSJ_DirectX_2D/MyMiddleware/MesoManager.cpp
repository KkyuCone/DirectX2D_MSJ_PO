#include "MesoManager.h"
#include <Scene.h>
#include <GameObject.h>

int MesoManager::m_Meso = 1000000;			// 백만


MesoManager::MesoManager()
{
}


MesoManager::~MesoManager()
{
}

//

bool MesoManager::Init()
{
	Vector4 ParentPos = GetGameObject()->GetParent()->GetTransform()->Local_Position();

	Vector4 MoneyScale = Vector4(7.0f, 9.0f, 1.0f, 1.0f);
	Vector4 CurPos = GetGameObject()->GetTransform()->Local_Position();
	CurPos.z = -9.0f;
	CurPos.x -= 1.0f;



	// 만들기 ! 천만단위까지
	for (size_t i = 0; i < 9; i++)
	{
		Vector4 MoneyPos = CurPos;
		MoneyPos.x -= (6.0f * i);

		if (2 < i && i < 6)
		{
			if (3 == i)
			{
				Vector4 CommaPos = MoneyPos;
				CommaPos.x += 1.0f;

				My_PTR<GameObject> Comma = GetScene()->GameObject_Create(L"Comma");
				Comma->SetParent(GetGameObject()->GetParent());
				Comma->GetTransform()->Local_Position(CommaPos);
				Comma->GetTransform()->Local_Scale(MoneyScale);
				My_PTR<SpriteRenderer2D> CommaRenderer = Comma->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
				CommaRenderer->SetSprite(L"UI_MoneyNumberUnit.png", 0);		// EmptyItem

				m_CommaRenderer.push_back(CommaRenderer);
			}

			MoneyPos.x -= 3.0f;
		}

		if (5 < i && i < 9)
		{
			MoneyPos.x -= 6.0f;

			if (6 == i)
			{
				Vector4 CommaPos = MoneyPos;
				CommaPos.x += 4.0f;

				My_PTR<GameObject> Comma = GetScene()->GameObject_Create(L"Comma");
				Comma->SetParent(GetGameObject()->GetParent());
				Comma->GetTransform()->Local_Position(CommaPos);
				Comma->GetTransform()->Local_Scale(MoneyScale);
				My_PTR<SpriteRenderer2D> CommaRenderer = Comma->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
				CommaRenderer->SetSprite(L"UI_MoneyNumberUnit.png", 0);		// EmptyItem

				m_CommaRenderer.push_back(CommaRenderer);
			}
		}


		My_PTR<GameObject> MoenyObj = GetScene()->GameObject_Create(L"Money_Obj");
		MoenyObj->SetParent(GetGameObject()->GetParent());
		MoenyObj->GetTransform()->Local_Position(MoneyPos);
		MoenyObj->GetTransform()->Local_Scale(MoneyScale);
		My_PTR<SpriteRenderer2D> MoneyRenderer = MoenyObj->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
		MoneyRenderer->SetSprite(L"EmptyItem.png", 0);		// EmptyItem

		m_MoneyNumbers.push_back(MoenyObj);
		m_MoneyRenderer.push_back(MoneyRenderer);
	}

	ChangeMeso();

	return true;
}

void MesoManager::Update()
{
	ChangeMeso();
}

void MesoManager::FinalUpdate()
{

}

//
void MesoManager::AddMeso(int _Value)
{
	m_Meso += _Value;
}

void MesoManager::DecMeso(int _Value)
{
	m_Meso -= _Value;
}

//

void MesoManager::ChangeMeso()
{
	std::wstring NumberToString = std::to_wstring(m_Meso);
	int NumberCount = (int)(NumberToString.length());			// 개수
	int MoneyVectorValue = 0;

	for (int i = (NumberCount-1); i >= 0; --i)
	{
		wchar_t MoneyNumberValue = NumberToString.at(i);
		const wchar_t* CompareNumber = &MoneyNumberValue;
		int Number = _wtoi(CompareNumber);

		if (0 == Number || 1 == Number ||
			2 == Number || 3 == Number ||
			4 == Number || 5 == Number ||
			6 == Number || 7 == Number ||
			8 == Number || 9 == Number)
		{
			m_MoneyRenderer[MoneyVectorValue]->SetSprite(L"UI_MoneyNumber.png", Number);
		}
		++MoneyVectorValue;
	}
}

void MesoManager::Money_Off()
{
	for (size_t i = 0; i < m_MoneyNumbers.size(); ++i)
	{
		m_MoneyNumbers[i]->Update_Off();
	}
}

void MesoManager::Money_On()
{
	for (size_t i = 0; i < m_MoneyNumbers.size(); ++i)
	{
		m_MoneyNumbers[i]->Update_On();
	}
}