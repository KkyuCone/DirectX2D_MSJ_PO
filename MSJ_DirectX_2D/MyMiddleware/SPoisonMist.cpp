#include "SPoisonMist.h"
#include "PoisonCloud.h"

#include <Scene.h>
#include <GameObject.h>



SPoisonMist::SPoisonMist() : m_bCreatePC(false)
{
}


SPoisonMist::~SPoisonMist()
{
}

//

bool SPoisonMist::Init()
{
	m_SPRenderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_SPRenderer)
	{
		EAssert(true);
		return false;
	}

	m_SPAnim = GetComponent<Animation2D>();

	if (nullptr == m_SPAnim)
	{
		EAssert(true);
		return false;
	}

	m_SPAnim->Animation_Create(L"SPBaseAnim", 0, 18, false, 0.1f);
	m_SPAnim->Animation_Change(L"SPBaseAnim");
	//CreatePCManager();
	return true;
}

void SPoisonMist::Update()
{
	if (false == m_bCreatePC)
	{
		SPCreateEffectCheck();
	}

	if (true == m_SPAnim->IsEndAnimation())
	{
		SPDeath();
	}
}

void SPoisonMist::FinalUpdate()
{

}

//
void SPoisonMist::SPCreateEffectCheck()
{
	if (10 == m_SPAnim->CurAnimationFram())
	{
		CreatePCManager();
		m_bCreatePC = true;
	}
}

void SPoisonMist::CreatePoisonCloud(std::wstring _TexName, std::wstring _SCount, int _PoisonCount, RenderOrder::RENDER_ORDER _RenderOrder, Vector4 _Pos, Vector4 _Scale)
{
	std::wstring BaseName = GetGameObject()->GetName();

	std::wstring PoisonName = BaseName + L"PoisonCloud" + _SCount;
	My_PTR<GameObject> NewPoisonCloud = GetGameObject()->GetScene()->GameObject_Create(PoisonName.c_str());
	NewPoisonCloud->GetTransform()->Local_Position(_Pos);
	NewPoisonCloud->GetTransform()->Local_Scale(_Scale);
	My_PTR<SpriteRenderer2D> PCRenderer = NewPoisonCloud->Component_Add<SpriteRenderer2D>(_RenderOrder);
	PCRenderer->SetSprite(_TexName.c_str(), 0);
	PCRenderer->Component_Add<Animation2D>();

	My_PTR<PoisonCloud> NewPCScript = NewPoisonCloud->Component_Add<PoisonCloud>();
	NewPCScript->SetPosionCount(_PoisonCount);

}

void SPoisonMist::CreatePCManager()
{
	Vector4 BasePos = GetGameObject()->GetTransform()->Local_Position();
	BasePos.y -= 150.0f;

	CreatePoisonCloud(L"PoisonMist_Tile8.png", L"1", 7, RenderOrder::SKILL_EFF1,Vector4(BasePos.x, BasePos.y, 15.0f, 1.0f), Vector4(138.0f, 130.0f, 1.0f, 1.0f));
	CreatePoisonCloud(L"PoisonMist_Tile7.png", L"2", 6, RenderOrder::SKILL_EFF2, Vector4(BasePos.x + 70.0f, BasePos.y + 80.0f, 14.0f, 1.0f), Vector4(109.0f, 109.0f, 1.0f, 1.0f));
	CreatePoisonCloud(L"PoisonMist_Tile6.png", L"3", 6, RenderOrder::SKILL_EFF3, Vector4(BasePos.x - 60.0f, BasePos.y + 70.0f, 13.0f, 1.0f), Vector4(101.0f, 84.0f, 1.0f, 1.0f));
	CreatePoisonCloud(L"PoisonMist_Tile5.png", L"4", 7, RenderOrder::SKILL_EFF4, Vector4(BasePos.x - 90.0f, BasePos.y - 50.0f, 12.0f, 1.0f), Vector4(103.0f, 73.0f, 1.0f, 1.0f));
	CreatePoisonCloud(L"PoisonMist_Tile4.png", L"5", 6, RenderOrder::SKILL_EFF5, Vector4(BasePos.x + 80.0f, BasePos.y - 80.0f, 11.0f, 1.0f), Vector4(98.0f, 83.0f, 1.0f, 1.0f));
	CreatePoisonCloud(L"PoisonMist_Tile3.png", L"6", 5, RenderOrder::SKILL_EFF6, Vector4(BasePos.x + 70.0f, BasePos.y + 20.0f, 10.0f, 1.0f), Vector4(96.0f, 72.0f, 1.0f, 1.0f));
	CreatePoisonCloud(L"PoisonMist_Tile2.png", L"7", 5, RenderOrder::SKILL_EFF7, Vector4(BasePos.x - 30.0f, BasePos.y - 70.0f, 9.0f, 1.0f), Vector4(91.0f, 58.0f, 1.0f, 1.0f));
	CreatePoisonCloud(L"PoisonMist_Tile1.png", L"8", 6, RenderOrder::SKILL_EFF8, Vector4(BasePos.x - 90.0f, BasePos.y - 75.0f, 8.0f, 1.0f), Vector4(84.0f, 61.0f, 1.0f, 1.0f));
	CreatePoisonCloud(L"PoisonMist_Tile0.png", L"9", 5, RenderOrder::SKILL_EFF9, Vector4(BasePos.x + 50.0f, BasePos.y - 50.0f, 7.0f, 1.0f), Vector4(58.0f, 41.0f, 1.0f, 1.0f));
	CreatePoisonCloud(L"PoisonMist_Tile7.png", L"10", 6, RenderOrder::SKILL_EFF10, Vector4(BasePos.x + 100.0f, BasePos.y - 40.0f, 6.0f, 1.0f), Vector4(109.0f, 109.0f, 1.0f, 1.0f));
	CreatePoisonCloud(L"PoisonMist_Tile8.png", L"11", 7, RenderOrder::SKILL_EFF11, Vector4(BasePos.x - 120.0f, BasePos.y + 20.0f, 5.0f, 1.0f), Vector4(138.0f, 130.0f, 1.0f, 1.0f));
	CreatePoisonCloud(L"PoisonMist_Tile3.png", L"6", 5, RenderOrder::SKILL_EFF12, Vector4(BasePos.x - 150.0f, BasePos.y - 20.0f, 4.0f, 1.0f), Vector4(96.0f, 72.0f, 1.0f, 1.0f));
	CreatePoisonCloud(L"PoisonMist_Tile7.png", L"2", 6, RenderOrder::SKILL_EFF13, Vector4(BasePos.x + 160.0f, BasePos.y + 10.0f, 5.0f, 1.0f), Vector4(109.0f, 109.0f, 1.0f, 1.0f));
}

void SPoisonMist::SPDeath()
{
	GetGameObject()->Death();
}
