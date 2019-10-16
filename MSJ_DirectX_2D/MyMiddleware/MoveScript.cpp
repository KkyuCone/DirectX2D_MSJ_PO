#include "MoveScript.h"
#include <Input.h>
#include <SpriteRenderer2D.h>
#include <math.h>



MoveScript::MoveScript() : MoveSpeed(100.0f), m_Index(0)
{

}


MoveScript::~MoveScript()
{
}

bool MoveScript::Init()
{
	//CentralTransform = (*GetTransform());
	GetTransform()->Local_Position(Vector4(250.0f, 0.0f, 0.0f));

	m_SpriteRender = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_SpriteRender)
	{
		EAssert(true);
	}

	m_Animation = GetComponent<Animation2D>();
	m_Animation->Animation_Create(L"Idle", 0, 2);
	m_Animation->Animation_Create(L"Run", 3, 6, true);
	m_Animation->Animation_Create(L"Jump", 29, 29, false);
	m_Animation->Animation_Change(L"Idle");
	return true;
}

float de = 0.f;


void MoveScript::Update()
{
	int a = 0;
	// FSMㅁ ㅏㄴ들기

	if (Input::DownStay(L"Left"))
	{
		int a = 0;
		m_Animation->Animation_Change(L"Run");
		GetTransform()->Local_Move(Vector4::Left * MoveSpeed * Time::DeltaTime());

		Vector4 Size = GetTransform()->Local_Scale();
		Size.x = 128.0f;
		Size.y = 128.0f;
		GetTransform()->Local_Scale(Size);
	}

	//if (Input::Up(L"Left"))
	//{
	//	m_Animation->Animation_Change(L"Idle");
	//}

	//if (Input::DownStay(L"Q"))
	//{
	//	m_Animation->Animation_Change(L"Jump_Up");
	//}


	//if (L"Run" != m_Animation->CurAnimationName())
	//{
	//	if (L"Idle" == m_Animation->CurAnimationName())
	//	{
	//		if (true == m_Animation->IsEndAnimation())
	//		{
	//			m_Animation->Animation_Change(L"Jump_Up");
	//		}
	//	}
	//}

	if (Input::DownStay(L"Right"))
	{
		m_Animation->Animation_Change(L"Run");
		GetTransform()->Local_Move(Vector4::Right * MoveSpeed * Time::DeltaTime());

		// 반대로 뒤집기
		Vector4 Size = GetTransform()->Local_Scale();
		Size.x = -128.0f;
		Size.y = 128.0f;
		GetTransform()->Local_Scale(Size);
	}

	if (Input::DownStay(L"Up"))
	{
		GetTransform()->Local_Move(Vector4::Up * MoveSpeed * Time::DeltaTime());
	}

	if (Input::DownStay(L"Down"))
	{
		GetTransform()->Local_Move(Vector4::Down * MoveSpeed * Time::DeltaTime());
	}

	//if (Input::DownStay(L"Rotation"))
	//{
	//	GetTransform()->Local_Rotate(Vector4::AxisZ * 20.0f * Time::DeltaTime());
	//}


	//// 원점을 기준으로하는 공전
	//if (Input::DownStay(L"Revolution"))
	//{
	//	de += 10.0f * Time::DeltaTime();		// 세타값

	//	GetTransform()->Local_Revolve(Vector4::AxisZ * 150.0f * Time::DeltaTime());
	//	GetTransform()->Local_Position(Vector4(0.0f, 300.0f + 100.f * sinf(de), 0.0f));
	//}

	if (Input::DownStay(L"Q"))
	{
		//GetTransform()->Local_RotateZ(360.0f * Time::DeltaTime());
		//Vector4 Rot = GetTransform()->Local_Rotation();

		//Rot.z += MoveSpeed * Time::DeltaTime();

		//GetTransform()->Local_Rotation(Rot);
		//int a = 0;
		//GetGameObject()->Death();

		//GetTransform()->Local_Rotate(Vector4::AxisZ * 20.0f * Time::DeltaTime());

		//++m_Index;
		//m_SpriteRender->Index(m_Index);
	}

	if (Input::DownStay(L"T"))
	{
		//GetTransform()->Local_RotateZ(-360.0f * Time::DeltaTime());
	}
	


}