#include "Weather_Snow.h"



Weather_Snow::Weather_Snow()
	: m_bSnowAni1(true), m_bSnowAni2(false), m_bSnowAni3(false), m_bSnowAni4(false), m_bSnowAni5(false), m_bSnowAni6(false)
{
}


Weather_Snow::~Weather_Snow()
{
}

bool Weather_Snow::Init()
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



	m_Animation->Animation_Create(L"Snow01", 0, 5, true, 0.15f);

	m_Renderer->SetSprite(L"Weather_Ani_2.png", 0);
	m_Animation->Animation_Create(L"Snow02", 0, 5, true, 0.15f);

	m_Renderer->SetSprite(L"Weather_Ani_3.png", 0);
	m_Animation->Animation_Create(L"Snow03", 0, 5, true, 0.15f);

	m_Renderer->SetSprite(L"Weather_Ani_4.png", 0);
	m_Animation->Animation_Create(L"Snow04", 0, 5, true, 0.15f);

	m_Renderer->SetSprite(L"Weather_Ani_5.png", 0);
	m_Animation->Animation_Create(L"Snow05", 0, 5, true, 0.15f);

	m_Renderer->SetSprite(L"Weather_Ani_6.png", 0);
	m_Animation->Animation_Create(L"Snow06", 0, 1, true, 0.15f);

	m_Renderer->SetSprite(L"Weather_Ani_1.png", 0);
	m_Animation->Animation_Change(L"Snow01");

	return true;
}

void Weather_Snow::Update()
{
	SnowEndAni();
}

void Weather_Snow::FinalUpdate()
{

}


//
void Weather_Snow::SnowEndAni()
{
	if (true == m_Animation->IsEndAnimation())
	{
		SnowChange();
	}
}

void Weather_Snow::SnowChange()
{
	if (true == m_bSnowAni1)
	{
		m_Renderer->SetSprite(L"Weather_Ani_2.png", 0);
		m_Animation->Animation_Change(L"Snow02");
		m_bSnowAni1 = false;
		m_bSnowAni2 = true;
		return;
	}
	
	if (true == m_bSnowAni2)
	{
		m_Renderer->SetSprite(L"Weather_Ani_3.png", 0);
		m_Animation->Animation_Change(L"Snow03");
		m_bSnowAni2 = false;
		m_bSnowAni3 = true;
		return;
	}

	if (true == m_bSnowAni3)
	{
		m_Renderer->SetSprite(L"Weather_Ani_4.png", 0);
		m_Animation->Animation_Change(L"Snow04");
		m_bSnowAni3 = false;
		m_bSnowAni4 = true;
		return;
	}

	if (true == m_bSnowAni4)
	{
		m_Renderer->SetSprite(L"Weather_Ani_5.png", 0);
		m_Animation->Animation_Change(L"Snow05");
		m_bSnowAni4 = false;
		m_bSnowAni5 = true;
		return;
	}

	if (true == m_bSnowAni5)
	{
		m_Renderer->SetSprite(L"Weather_Ani_6.png", 0);
		m_Animation->Animation_Change(L"Snow06");
		m_bSnowAni5 = false;
		m_bSnowAni6 = true;
		return;
	}

	if (true == m_bSnowAni6)
	{
		m_Renderer->SetSprite(L"Weather_Ani_1.png", 0);
		m_Animation->Animation_Change(L"Snow01");
		m_bSnowAni6 = false;
		m_bSnowAni1 = true;
		return;
	}
}