#include "BGFade.h"

#include <Sprite.h>
#include <Texture.h>

BGFade::BGFade() : m_bFade(false), m_CurTime(1.0f), m_bFadeDown(false)
{
}


BGFade::~BGFade()
{
}
//

bool BGFade::Init()
{
	m_bFadeDown = false;
	m_Renderer = GetComponent<SpriteRenderer2D>();

	m_Renderer->GetSkillBuffer()->CataclysmValue.y = 1.0f;

	return true;
}

void BGFade::Update()
{
	if (m_bFade == false)
	{
		if (m_CurTime < 0.0f)
		{
			m_CurTime = 0.0f;
			m_Renderer->GetSkillBuffer()->CataclysmValue.w = m_CurTime;
			m_bFade = true;
			return;
		}
		m_CurTime -= (Time::DeltaTime() * 0.1f);
		m_Renderer->GetSkillBuffer()->CataclysmValue.w = m_CurTime;
	}


	if (true == m_bFadeDown)
	{
		if (m_CurTime > 1.0f)
		{
			m_CurTime = 1.0f;
			m_Renderer->GetSkillBuffer()->CataclysmValue.w = m_CurTime;
			m_bFade = true;
			return;
		}
		m_CurTime += (Time::DeltaTime() * 0.4f);
		m_Renderer->GetSkillBuffer()->CataclysmValue.w = m_CurTime;
	}
}

void BGFade::FinalUpdate()
{

}
