#include "QuestAlarm.h"



QuestAlarm::QuestAlarm()
{
}


QuestAlarm::~QuestAlarm()
{
}

//

bool QuestAlarm::Init()
{
	m_Renderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_Renderer)
	{
		EAssert(true);
		return false;
	}

	m_Animation = GetComponent<Animation2D>();

	if (nullptr == m_Animation) 
	{
		EAssert(true);
		return false;
	}

	m_Animation->Animation_Create(L"Quest_Alarm", 0, 7, true, 0.15f);
	m_Animation->Animation_Create(L"Quest_Doing", 0, 7, true, 0.15f);
	m_Animation->Animation_Create(L"Quest_Completion", 0, 7, true, 0.15f);
	m_Animation->Animation_Change(L"Quest_Alarm");

	return true;
}

void QuestAlarm::Update()
{

}

void QuestAlarm::FinalUpdate()
{

}

//

void QuestAlarm::ChangeAnimation(std::wstring _AniName)
{
	if (L"Quest_Alarm" == _AniName)
	{
		m_Renderer->SetSprite(L"Quest_Alarm.png", 0);
	}
	else if (L"Quest_Doing" == _AniName)
	{
		m_Renderer->SetSprite(L"Quest_Ing.png", 0);
	}
	else if (L"Quest_Completion" == _AniName)
	{
		m_Renderer->SetSprite(L"Quest_Complication.png", 0);
	}
	else
	{
		return;
	}
	m_Animation->Animation_Change(_AniName.c_str());

}
