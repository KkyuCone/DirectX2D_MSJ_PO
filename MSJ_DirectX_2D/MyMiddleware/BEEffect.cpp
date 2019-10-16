#include "BEEffect.h"



BEEffect::BEEffect() : m_LifeTime(0.6f)
{
}


BEEffect::~BEEffect()
{
}

//

bool BEEffect::Init()
{
	m_BlazeEffectRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_BlazeEffectRenderer)
	{
		EAssert(true);
		return false;
	}

	m_BlazeEffectAnim = GetComponent<Animation2D>();

	if (nullptr == m_BlazeEffectAnim)
	{
		EAssert(true);
		return false;
	}

	m_BlazeEffectAnim->Animation_Create(L"BEEffect", 12, 17, false, 0.1f);
	m_BlazeEffectAnim->Animation_Change(L"BEEffect");

	return true;
}

void BEEffect::Update()
{
	DeathTimer();
}

void BEEffect::FinalUpdate()
{

}

//
void BEEffect::DeathTimer()
{
	if (true == m_BlazeEffectAnim->IsEndAnimation())
	{
		GetGameObject()->Death();
	}
}
