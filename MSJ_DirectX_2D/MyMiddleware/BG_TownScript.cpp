#include "BG_TownScript.h"



BG_TownScript::BG_TownScript()
	: m_Renderer(nullptr), m_Animaiton(nullptr), m_bBGAni1(true)
{
}


BG_TownScript::~BG_TownScript()
{
}


//

bool BG_TownScript::Init()
{
	m_Renderer = GetGameObject()->GetComponent<SpriteRenderer2D>();

	if (nullptr == m_Renderer)
	{
		EAssert(true);
		return false;
	}

	m_Animaiton = GetGameObject()->GetComponent<Animation2D>();

	if (nullptr == m_Animaiton)
	{
		EAssert(true);
		return false;
	}

	m_Animaiton->Animation_Create(L"ArcanaTownAni_1", 0, 0, true, 0.25f);

	m_Renderer->SetSprite(L"ArcanaBGAni_2.png", 0);
	m_Animaiton->Animation_Create(L"ArcanaTownAni_2", 3, 3, true , 0.25f);

	m_Animaiton->Animation_Change(L"ArcanaTownAni_2");
	//m_Renderer->SetSprite(L"ArcanaBGAni_1.png", 0);
	//m_Animaiton->Animation_Change(L"ArcanaTownAni_1");


	return true;
}

void BG_TownScript::Update()
{
	//Check_BGAni();
}

void BG_TownScript::FinalUpdate()
{

}

//

void BG_TownScript::Check_BGAni()
{
	if (true == m_Animaiton->IsEndAnimation())
	{
		ChangeBGSprite();
	}
}


void BG_TownScript::ChangeBGSprite()
{
	if (true == m_bBGAni1)
	{
		m_bBGAni1 = false;
		m_Renderer->SetSprite(L"ArcanaBGAni_2.png" , 0);
		m_Animaiton->Animation_Change(L"ArcanaTownAni_2");
	}
	else
	{
		m_bBGAni1 = true;
		m_Renderer->SetSprite(L"ArcanaBGAni_1.png", 0);
		m_Animaiton->Animation_Change(L"ArcanaTownAni_1");
	}
}