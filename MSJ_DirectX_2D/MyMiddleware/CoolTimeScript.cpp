#include "CoolTimeScript.h"
#include <GameObject.h>

CoolTimeScript::CoolTimeScript() : m_CoolEnd(true), m_CoolTime(0.0f), m_CurTime(0.0f)
{
}


CoolTimeScript::~CoolTimeScript()
{
}

//
bool CoolTimeScript::Init()
{
	m_Renderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_Renderer)
	{
		EAssert(true);
		return false;
	}

	m_Animaiton = GetComponent<Animation2D>();

	if (nullptr == m_Animaiton)
	{
		EAssert(true);
		return false;
	}

	m_Animaiton->Animation_Create(L"None", 18, 19, true, 0.1f);
	m_Animaiton->Animation_Create(L"Cool", 0, 17, true, 0.1f);
	m_Animaiton->Animation_Change(L"None");


	return true;
}

void CoolTimeScript::Update()
{
	if (false == m_CoolEnd)
	{
		CoolTimeEndCheck();
	}
}

void CoolTimeScript::FinalUpdate()
{
}

//

void CoolTimeScript::CoolTimeEndCheck()
{
	if (m_CoolTime <= m_CurTime)
	{
		m_Animaiton->Animation_Change(L"None");
		m_CoolEnd = true;
		m_CurTime = 0.0f;
		return;
	}

	m_CurTime += Time::DeltaTime();
}

// 호출 순서 타임정하기 -> 바꾸기

void CoolTimeScript::SetCoolTimeChange(float _Time)
{
	// 시간계산
	m_CoolTime = _Time;
	float ChangeTime = _Time / COOLTIMETEXCOUNT;
	m_Animaiton->Animation_Find(L"Cool")->m_FrameTime = ChangeTime;
}

void CoolTimeScript::SetCoolTimeAnimationFrame(int _Frame)
{
	if (L"Cool" == m_Animaiton->CurAnimationName())
	{
		m_Animaiton->SetFrame(_Frame);
	}
}

void CoolTimeScript::SetCoolTimeAnimationCurTime(float _Time)
{
	m_CurTime = _Time;
	m_Animaiton->SetCurTime(m_CurTime);
}

void CoolTimeScript::ChangeCoolTimeAni()
{
	if (L"None" == m_Animaiton->CurAnimationName())
	{
		m_Animaiton->Animation_Change(L"Cool");
		m_CoolEnd = false;
	}
}

//

bool CoolTimeScript::GetCoolEnd()
{
	return m_CoolEnd;
}

std::wstring CoolTimeScript::GetCurAnimationName()
{
	std::wstring CurAniName = m_Animaiton->CurAnimationName();
	return CurAniName;
}

float CoolTimeScript::GetCurCoolTime()
{
	return  m_CurTime;
}

int CoolTimeScript::GetCurCoolTimeFrame()
{
	return m_Animaiton->CurAnimationFram();
}