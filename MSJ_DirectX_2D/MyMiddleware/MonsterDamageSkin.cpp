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
	// 자릿수에 따라서 데미지스킨 넣어주기
	int HitDamage = _Damage;
	int NumberCount = 0;

	// 자릿수 알기
	while (HitDamage > 0)
	{
		HitDamage /= 10;
		++NumberCount;
	}

	// 자릿수를 알고나서는 
	// 각 자리에 맞게 데미지스킨 위치를 설정하고 만들어준다.
	// 짝수번째의 숫자는 y포지션을 조금 내려준다.
	Vector4 CurPos = GetGameObject()->GetTransform()->Local_Position();
	Vector4 DamageSkinScale = Vector4(38.0f, 44.0f, 1.0f, 1.0f);

	// 개수를 반으로 나눈다.
	int HalfNumberCount = NumberCount / 2;

	// 첫번째 숫자의 위치 설정
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


		// 데미지에 따른 데미지스킨 이미지 띄워주기 (만들기)
		wchar_t CreateNumberValue = NumberString.at(i);
		const wchar_t* NumberPointer = &CreateNumberValue;

		int TextureNumber = _wtoi(NumberPointer);			// 해당 텍스처 번호

		My_PTR<GameObject> NewDamageSkin = GetScene()->GameObject_Create(L"DamageSkin");
		NewDamageSkin->GetTransform()->Local_Position(NewCreatePos);
		NewDamageSkin->GetTransform()->Local_Scale(DamageSkinScale);
		My_PTR<SpriteRenderer2D> NewDamageSkinRenderer = NewDamageSkin->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF13);
		NewDamageSkinRenderer->SetSprite(L"DamageSkin_Chick.png", TextureNumber);
		My_PTR<DamageSkinMoveScript> NewDamageSkinScript = NewDamageSkin->Component_Add<DamageSkinMoveScript>();
	}
}
