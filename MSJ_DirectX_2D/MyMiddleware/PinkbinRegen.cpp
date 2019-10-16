#include "PinkbinRegen.h"
#include "Pinkbin.h"

#include <GameObject.h>
#include <Scene.h>



PinkbinRegen::PinkbinRegen() 
	: m_bAniEnd(false), m_bUpTile(false)
	, BackgroundSize(Vector4::One), OriginScale(Vector4::One)
	, m_State(MonsterState::REGEN)
	, m_pPlayer(nullptr), m_pFloor(nullptr)
	, m_SpRenderer(nullptr), m_Animation(nullptr)
	, m_NewPinkbin(nullptr)
{
}


PinkbinRegen::~PinkbinRegen()
{
}

//

bool PinkbinRegen::Init()
{
	m_SpRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_SpRenderer)
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

	// GetComponent<SpriteRenderer2D>()->SetSprite(L"", 0);

	m_Animation->Animation_Create(L"Regen", 0, 16, true, 0.1f);
	m_Animation->Animation_Change(L"Regen");

	return true;
}

void PinkbinRegen::Update()
{
	CheckAnimationEnd();
}

void PinkbinRegen::FinalUpdate()
{
}

//

void PinkbinRegen::PinkbinDeath()
{
	m_NewPinkbin->GetComponent<Pinkbin>()->SetDie();
}

void PinkbinRegen::CheckAnimationEnd()
{
	if (false == m_bAniEnd)
	{
		if (11 == m_Animation->CurAnimationFram())
		{
			m_bAniEnd = true;
			Create_Pinkbin();
		}
	}

	if (true == m_Animation->IsEndAnimation())
	{
		GetGameObject()->Death();
	}
}

void PinkbinRegen::Create_Pinkbin()
{
	// 俏农后 积己
	m_NewPinkbin = GetScene()->GameObject_Create(L"Pinkbin");
	m_NewPinkbin->GetTransform()->Local_Position(GetTransform()->Local_Position());
	m_NewPinkbin->GetTransform()->Local_Scale(Vector4(199.0f, 139.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> NewPinkbinRenderer = m_NewPinkbin->Component_Add<SpriteRenderer2D>(RenderOrder::MONSTER);
	NewPinkbinRenderer->SetSprite(L"Pinkbin_Base.png", 0);
	My_PTR<Animation2D> NewPinkbinAni = m_NewPinkbin->Component_Add<Animation2D>();
	My_PTR<Pinkbin> NewPinkbinScript = m_NewPinkbin->Component_Add<Pinkbin>();
	NewPinkbinScript->SetUpTilePos(false);
	NewPinkbinScript->SetFloor(m_pFloor);
	NewPinkbinScript->SetPlayer(m_pPlayer);
	NewPinkbinScript->SetBackground(BackgroundSize);
}

//

void PinkbinRegen::SetPlayer(GameObject* _Player)
{
	if (nullptr == _Player)
		return;

	m_pPlayer = _Player;
}

void PinkbinRegen::SetFloor(GameObject* _Floor)
{
	if (nullptr == _Floor)
		return;

	m_pFloor = _Floor;
}

void PinkbinRegen::SetBackground(Vector4 _Size)
{
	BackgroundSize = _Size;
}

void PinkbinRegen::SetUpTilePos(bool _Value)
{
	m_bUpTile = _Value;
}

//

GameObject* PinkbinRegen::GetPinkbin()
{
	return m_NewPinkbin;
}