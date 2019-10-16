#include "MonsterDamageSkin.h"
#include "DamageSkinMoveScript.h"
#include <Scene.h>
#include "RenderOrder.h"

MonsterDamageSkin::MonsterDamageSkin()
{
}


MonsterDamageSkin::~MonsterDamageSkin()
{
}

//

bool MonsterDamageSkin::Init()
{
	return true;
}

void MonsterDamageSkin::Update()
{

}

void MonsterDamageSkin::FinalUpdate()
{

}

//

void MonsterDamageSkin::Create_DamageSkin(int _Damage)
{
	// �ڸ����� ���� ��������Ų �־��ֱ�
	int HitDamage = _Damage;
	int NumberCount = 0;

	// �ڸ��� �˱�
	while (HitDamage > 0)
	{
		HitDamage /= 10;
		++NumberCount;
	}

	// �ڸ����� �˰����� 
	// �� �ڸ��� �°� ��������Ų ��ġ�� �����ϰ� ������ش�.
	// ¦����°�� ���ڴ� y�������� ���� �����ش�.
	Vector4 CurPos = GetGameObject()->GetTransform()->Local_Position();
	Vector4 DamageSkinScale = Vector4(38.0f, 44.0f, 1.0f, 1.0f);

	// ������ ������ ������.
	int HalfNumberCount = NumberCount / 2;

	// ù��° ������ ��ġ ����
	Vector4 FirstNumberPos = CurPos;
	FirstNumberPos.x -= (28.0f * HalfNumberCount);
	FirstNumberPos.z = 10.0f;

	std::wstring NumberString = std::to_wstring(_Damage);
	int NumberStringCount = (int)(NumberString.length());

	for (int i = 0; i < NumberStringCount; ++i)
	{
		Vector4 NewCreatePos = FirstNumberPos;
		if (0 < i)
		{
			NewCreatePos.x += (20.0f * i);
			NewCreatePos.z -= (1.0f * i);

			if ( 0 == (i/2))
			{
				NewCreatePos.y -= 10.0f;
			}
		}


		// �������� ���� ��������Ų �̹��� ����ֱ� (�����)
		wchar_t CreateNumberValue = NumberString.at(i);
		const wchar_t* NumberPointer = &CreateNumberValue;

		int TextureNumber = _wtoi(NumberPointer);			// �ش� �ؽ�ó ��ȣ

		My_PTR<GameObject> NewDamageSkin = GetScene()->GameObject_Create(L"DamageSkin");
		NewDamageSkin->GetTransform()->Local_Position(NewCreatePos);
		NewDamageSkin->GetTransform()->Local_Scale(DamageSkinScale);
		My_PTR<SpriteRenderer2D> NewDamageSkinRenderer = NewDamageSkin->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF13);
		NewDamageSkinRenderer->SetSprite(L"DamageSkin_Chick.png", TextureNumber);
		My_PTR<DamageSkinMoveScript> NewDamageSkinScript = NewDamageSkin->Component_Add<DamageSkinMoveScript>();
	}
}
