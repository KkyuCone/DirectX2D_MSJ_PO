#include "SBlazingExtinction.h"
#include "BELaser.h"
#include "BEEffect.h"

#include <Scene.h>
#include <GameObject.h>
#include <Time.h>
#include <RectCol2D.h>


SBlazingExtinction::SBlazingExtinction()
	: UseTime(UTIME), m_CurTime(0.0f), m_Dir(L""), m_MoveSpeed(MOVESPEED), m_MapWidth(0.0f)
	, m_CurLaserValue(0), m_bMoveStart(false), m_BlazeTime(0.0f)
{

}


SBlazingExtinction::~SBlazingExtinction()
{
}


//
bool SBlazingExtinction::Init()
{
	m_SphereRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_SphereRenderer)
	{
		EAssert(true);
		return false;
	}

	m_SpAnim = GetComponent<Animation2D>();

	if (nullptr == m_SpAnim)
	{
		EAssert(true);
		return false;
	}

	m_SpAnim->Animation_Create(L"Start", 0, 7, false, 0.1f);
	m_SpAnim->Animation_Create(L"Repeat", 8, 11, true, 0.1f);
	m_SpAnim->Animation_Change(L"Start");

	//m_EffAnim = GetComponent<Animation2D>();
	//m_EffAnim->Animation_Create(L"Effect", 12, 18, false, 0.1f);
	//m_EffAnim->Animation_ Change

	return true;
}

void SBlazingExtinction::Update()
{
	if (true == m_bMoveStart)
	{
		DirCheck();
		Move();


		m_BlazeTime += Time::DeltaTime();

		if (m_BlazeTime >= 1.5f)
		{
			m_BlazeTime = 0.0f;
			CreateBlaze();
		}
	}
	else
	{
		ChangeRepeat();
	}
}

void SBlazingExtinction::FinalUpdate()
{
	TimeCheck();
}

//
void SBlazingExtinction::Move()
{
	if (L"Left" == m_Dir)
	{
		GetTransform()->Local_Move(Vector4::Left * m_MoveSpeed * Time::DeltaTime());
	}
	else if (L"Right" == m_Dir)
	{
		GetTransform()->Local_Move(Vector4::Right * m_MoveSpeed * Time::DeltaTime());
	}
}

void SBlazingExtinction::DirCheck()
{
	if (L"Left" == m_Dir)
	{
		if (GetTransform()->World_Position().x - 150.0f <= ((-m_MapWidth) / 2))
		{
			m_Dir = L"Right";
		}
	}
	else if (L"Right" == m_Dir)
	{
		if (GetTransform()->World_Position().x + 150.0f >= (m_MapWidth / 2))
		{
			m_Dir = L"Left";
		}
	}
}

void SBlazingExtinction::TimeCheck()
{
	// 시간 체크
	if (m_CurTime >= UseTime)
	{
		GetGameObject()->Death();
		return;
	}
	else
	{
		m_CurTime += Time::DeltaTime();
	}
}

void SBlazingExtinction::ChangeRepeat()
{
	if (L"Start" != m_SpAnim->CurAnimationName())
		return;

	if (true == m_SpAnim->IsEndAnimation())
	{
		m_bMoveStart = true;
		m_SpAnim->Animation_Change(L"Repeat");
		CreateBEEffect();
	}

}

void SBlazingExtinction::CreateBlaze()
{
	// 상대의 거리 계산
	// 상대와의 거리가 1 < lengh <= 1133 이면 생성조건OK

	// 불꽃의 생성위치 설정
	// 상대의 방향벡터 구하기 (상대의 중심점 - 구체의 중심점)
	// 그 값을 정규화
	// 후에 그 벡터에 567를 곱한다. (생성위치)

	// 불꽃의 각도구하기
	// 상대와의 각도구한 후에
	// 불꽃에 각도를 넣어준다.Rotate

	if (0 < m_MonstersVec.size())
	{
		float Lazerz = 80.0f;

		for (int i = 0; i < m_MonstersVec.size(); ++i)
		{

			Vector4 SphereCentralPoint = GetTransform()->Local_Position();

			//if (L"Right" == m_Dir)
			//{
			//	SphereCentralPoint.x -= 50.0f;
			//}
			//else if (L"Left" == m_Dir)
			//{
			//	SphereCentralPoint.x -= 50.0f;
			//}


			// 나중에 구체 아래에 있는애들의 경우에는 각도를 판단해서 음방향인지 양방향인지 알아야한다.
			Vector4 MonsterCetralPoint = m_MonstersVec[i]->GetTransform()->World_Position();

			Vector4 NewVector = MonsterCetralPoint - SphereCentralPoint;
			NewVector.z = 2.0f;
			float Norm = NewVector.Norm();


			if (0.0 < Norm && IMAGEWIDTH >= Norm)
			{
				if (i > 12)
				{
					int RandValue = rand() % 4;

					if (0 != RandValue && 1 != RandValue)
					{
						i += 3;
					}
					
				}

				Vector4 DirVector = Vector4{ NewVector.x / Norm, NewVector.y / Norm, NewVector.z / Norm, 1.0f };
				//Vector4 BlazePos = DirVector * 567.0f;
				Vector4 BlazePos = GetTransform()->World_Position();

				Vector4 MyVector = SphereCentralPoint;
				MyVector.x = NewVector.x;

				int DirValue = MyVector.IsDir2D(NewVector);

				float DeSign = 0.0f;
				float AngleDeg = 0.0f;

				DeSign = MyVector.Angle3DRad(NewVector);
				AngleDeg = DeSign * RadianToDegree;


				BlazePos.z = Lazerz;

				float Degree = 0.0f;
			
				float a = Norm;
				//float b = fabsf(MonsterCetralPoint.x - SphereCentralPoint.x);
				//float c = fabsf(MonsterCetralPoint.y - SphereCentralPoint.y);
				float b = MonsterCetralPoint.x - SphereCentralPoint.x;
				float c = MonsterCetralPoint.y - SphereCentralPoint.y;

				float RadianAn = acosf(((a*a) + (b*b) - (c*c)) / (2*a*b));
				Degree = RadianAn *  RadianToDegree;			//  각도


				std::wstring VValue = std::to_wstring(m_CurLaserValue);

				std::wstring BlazeName = L"Blaze" + VValue;

				My_PTR<GameObject> NewBlaze = GetGameObject()->GetScene()->GameObject_Create(BlazeName.c_str());
				NewBlaze->GetTransform()->Local_Position(BlazePos);


				NewBlaze->GetTransform()->Local_RotateZ(Degree);
				NewBlaze->GetTransform()->Local_Scale(Vector4{ 1133.0f, 76.0f, 1.0f });
				My_PTR<SpriteRenderer2D> Ren = NewBlaze->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF0);
				Ren->SetSprite(L"BlazingExtinction_Ball.png", 0);
				Ren->ChangeMesh(L"TextureMesh2");
				NewBlaze->Component_Add<Animation2D>();
				NewBlaze->Component_Add<BELaser>();		// 블레이즈스크립트
				NewBlaze->GetComponent<BELaser>()->SetDegree(Degree);
				NewBlaze->GetComponent<BELaser>()->SetCutWidth(Norm);

				My_PTR<RectCol2D> m_RectCol = NewBlaze->Component_Add<RectCol2D>(L"Skill_BlazingExtinction");
				m_RectCol->Local_SubPosition(Vector4(b, c, -1.0f, 1.0f));
				m_RectCol->Local_SubScale(Vector4(100.0f, 76.0f, 1.0f, 1.0f));

				--Lazerz;
			}
		}
	}
}

void SBlazingExtinction::CreateBEEffect()
{
	std::wstring BEEffectName = GetGameObject()->GetName();
	BEEffectName += L"BEEffect";
	My_PTR<GameObject> NewBEEffect = GetGameObject()->GetScene()->GameObject_Create(BEEffectName.c_str());
	Vector4 BePos = GetTransform()->World_Position();
	Vector4 BeScale = GetTransform()->Local_Scale();

	if (L"Left" == m_Dir)
	{
		BePos.x += 50.0f;
	}
	else
	{
		BeScale.x *= -1.0f;
		BePos.x -= 200.0f;
	}
	NewBEEffect->GetTransform()->Local_Position(BePos);
	NewBEEffect->GetTransform()->Local_Scale(BeScale);
	My_PTR<SpriteRenderer2D> BESpRenderer = NewBEEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF2);
	BESpRenderer->SetSprite(L"BlazingExtinction_Effect.png", 0);
	NewBEEffect->Component_Add<Animation2D>();
	NewBEEffect->Component_Add<BEEffect>();			// 후 이펙트 스크립트
}

//
void SBlazingExtinction::SetDir(std::wstring _Dir)
{
	m_Dir = _Dir;

	if ( L"Left" == m_Dir )
	{
		Vector4 MoveX = { -100.0f, 0.0f, 0.0f, 0.0f };
		GetTransform()->Local_Move(MoveX);
	}
	else if ( L"Right" == m_Dir )
	{
		Vector4 MoveX = { 250.0f, 0.0f, 0.0f, 0.0f };
		GetTransform()->Local_Move(MoveX);
	}
}

void SBlazingExtinction::SetMapWidth(float _Width)
{
	m_MapWidth = _Width;
}

void SBlazingExtinction::SetMonsters(std::vector<GameObject*> _Monsters)
{
	m_MonstersVec = _Monsters;
}
