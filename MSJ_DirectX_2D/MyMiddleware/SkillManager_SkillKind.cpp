#include "SkillManager.h"
#include "PlayerMove.h"
#include "SBlazingExtinction.h"

#include "SBlizzard.h"
#include "BlizzardIcePillar.h"
#include "BlizzardIce.h"

#include "SPoisonMist.h"
#include "SSoulPenetration.h"

#include "SCataclysm.h"
#include "CataclysmEffectValue.h"
#include "CataclysmSceneBase.h"

#include <map>
#include <Scene.h>
#include <GameObject.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

#include "BlizzardHitEffect.h"
#include <Scene.h>

#include "PlayerMove.h"
#include <SoundPlayer.h>

// 블레이징 익스팅션
void SkillManager::Create_BlazingExtinction()
{
	SoundPlay_Effect(L"Use_CC4.mp3");
	std::wstring Value = std::to_wstring(BEValue);
	std::wstring SkillName = L"SKILL_BE" + Value;

	My_PTR<GameObject> NewBE = GetGameObject()->GetScene()->GameObject_Create(SkillName.c_str());
	NewBE->GetTransform()->Local_Position(Vector4(GetGameObject()->GetTransform()->Local_Position().x, GetGameObject()->GetTransform()->Local_Position().y + 20.0f, 10.0f, 1.0f));
	NewBE->GetTransform()->Local_Scale(Vector4(471.0f, 392.0f, 4.0f, 1.0f));
	NewBE->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF2)->SetSprite(L"BlazingExtinction_Effect.png", 0);
	NewBE->Component_Add<Animation2D>();
	NewBE->Component_Add<SBlazingExtinction>();
	NewBE->GetComponent<SBlazingExtinction>()->SetMapWidth(m_MapWidth);
	NewBE->GetComponent<SBlazingExtinction>()->SetDir(m_pPlayerMoveScript->GetDir());
	NewBE->GetComponent<SBlazingExtinction>()->SetMonsters(m_Monsters);
	NewBE->DebugMode_On();

	PlayerMove* CurPlayerMoveScript = GetGameObject()->GetComponent<PlayerMove>();
	CurPlayerMoveScript->AttackAniChange();
}

// 블리자드
void SkillManager::Create_Blizzard()
{
	SoundPlay_Effect(L"Use_Blizzard02.mp3");
	PlayerMove* CurPlayerMoveScript = GetGameObject()->GetComponent<PlayerMove>();
	CurPlayerMoveScript->AttackAniChange();

	// 플레이어 포스
	Vector4 PlayerPos = GetGameObject()->GetTransform()->Local_Position();

	// 이름
	std::wstring SBCount = std::to_wstring(BCount);
	std::wstring SkillName = L"BlizzardEffect_";
	SkillName += SBCount;

	// 캐릭터주변의 블리자드 이펙트는 그냥생성 (2개임)
	My_PTR<GameObject> BEffectObj = GetGameObject()->GetScene()->GameObject_Create(SkillName.c_str());
	BEffectObj->GetTransform()->Local_Position(Vector4(PlayerPos.x, PlayerPos.y, 4.0f, 1.0f));
	BEffectObj->GetTransform()->Local_Scale(Vector4(244.0f, 203.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> BESpriteRenderer = BEffectObj->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF2);
	BESpriteRenderer->SetSprite(L"Blizzard_Effect.png", 0);
	My_PTR<Animation2D> BEAnimation = BEffectObj->Component_Add<Animation2D>();
	My_PTR<SBlizzard> BEffectScript = BEffectObj->Component_Add<SBlizzard>();

	SkillName = L"";
	SkillName = L"BlizzardEffect0_" + SBCount;
	My_PTR<GameObject> BEffect0Obj = GetGameObject()->GetScene()->GameObject_Create(SkillName.c_str());
	BEffect0Obj->GetTransform()->Local_Position(Vector4(PlayerPos.x, PlayerPos.y, 120.0f, 1.0f));
	BEffect0Obj->GetTransform()->Local_Scale(Vector4(306.0f, 213.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> BE0SpriteRenderer = BEffect0Obj->Component_Add<SpriteRenderer2D>(RenderOrder::PLAYER_CHILD);
	BE0SpriteRenderer->SetSprite(L"Blizzard_Effect0.png", 0);
	My_PTR<Animation2D> BE0Animation = BEffect0Obj->Component_Add<Animation2D>();
	My_PTR<BlizzardIcePillar> BEffect0Script = BEffect0Obj->Component_Add<BlizzardIcePillar>();


	// 블리자드 스킬은 랜덤으로 얼음덩어리 2개중 한개를 사용한다.
	// 그리고 몬스터들과 거리를재서 제일가까운 애들한테 스킬 사용, 최대 8마리까지
	
	// 모든 몬스터들과 캐릭터 거리재기 (최대 거리 가로 800, 세로 600)
	// 해당 몬스터를 map로 넣기 첫번째값은 거리값
	// 랜덤한 얼음덩어리 캐릭터 방향에 따라 생성
	// 얼음 덩어리 위치조절(방향에 따른)
	
	// 몬스터 수 판별
	if ( 0 == m_Monsters.size())
	{
		return;
	}

	std::map<float, GameObject*> m_MonsterOrderMap;

	// 몬스터와 거리재서 순서대로 넣기
	for (int i = 0; i < m_Monsters.size(); ++i)
	{
		Vector4 MonsterPos =  m_Monsters[i]->GetTransform()->World_Position();
		Vector4 MyPos = GetGameObject()->GetTransform()->World_Position();

		Vector4 MPVector = MonsterPos - MyPos;

		MPVector.z = 1.0f;
		MPVector.w = 1.0f;

		// 거리계산
		float PMLengh = MPVector.Norm();

		if (PMLengh <= BLIZZARDRANGE)
		{
			m_MonsterOrderMap.insert(std::map<float, GameObject*>::value_type(PMLengh, m_Monsters[i]));
		}
	}

	int IceCount = 0;

	if ((int)(m_MonsterOrderMap.size()) < 8)
	{
		// 최대몹의 수보다 적으면.. ( 타겟이 8명 미만 )
		IceCount = (int)(m_MonsterOrderMap.size());
	}
	else
	{
		// 최대수 보다 크거나 같으면 ( 타겟이 8명 이상 )
		IceCount = 8;
	}

	std::map<float, GameObject*>::iterator MonsterStart = m_MonsterOrderMap.begin();

	std::wstring CurDir = GetGameObject()->GetComponent<PlayerMove>()->GetDir();		// 생성될 방향체크

	float PosZ = 15.0f;

	for (int i = 0; i < IceCount; ++i)
	{
		if (MonsterStart == m_MonsterOrderMap.end())
		{
			return;
		}

		Vector4 PresentMonsterPos = MonsterStart->second->GetTransform()->Local_Position();
		Vector4 PresentMonsterScale = MonsterStart->second->GetTransform()->Local_Scale();

		std::wstring SIceCount = std::to_wstring(i);
		SkillName = L"BliardIce_" + SIceCount;
		My_PTR<GameObject> BlizzardIceObj = GetGameObject()->GetScene()->GameObject_Create(SkillName.c_str());
		

		int BlizzardKind = rand() % 2;  // 0, 1


		if (0 == BlizzardKind)
		{
			// Blizzard_ObjectHit
			if (L"Left" == CurDir)
			{
				BlizzardIceObj->GetTransform()->Local_Position(Vector4(PresentMonsterPos.x + (PresentMonsterScale.x / 2), PresentMonsterPos.y + (PresentMonsterScale.y/2) + 50.0f, PosZ, 1.0f));
				BlizzardIceObj->GetTransform()->Local_Scale(Vector4(363.0f, 478.0f, 4.0f, 1.0f));
			}
			else
			{
				BlizzardIceObj->GetTransform()->Local_Position(Vector4(PresentMonsterPos.x - (PresentMonsterScale.x / 2), PresentMonsterPos.y + (PresentMonsterScale.y/2) + 50.0f, PosZ, 1.0f));
				BlizzardIceObj->GetTransform()->Local_Scale(Vector4(-363.0f, 478.0f, 4.0f, 1.0f));
			}

			My_PTR<SpriteRenderer2D> BSpriteRen = BlizzardIceObj->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF2);
			BSpriteRen->SetSprite(L"Blizzard_ObjectHit.png", 0);
			My_PTR<Animation2D> BAnim = BlizzardIceObj->Component_Add<Animation2D>();
			My_PTR<BlizzardIce> NewBliScript = BlizzardIceObj->Component_Add<BlizzardIce>();

			My_PTR<GameObject> NewBlizzardColEffectObj = GetGameObject()->GetScene()->GameObject_Create(L"BlizzardEffect");
			NewBlizzardColEffectObj->GetTransform()->Local_Position(BlizzardIceObj->GetTransform()->Local_Position());
			My_PTR<SpriteRenderer2D> ColEffectRenderer = NewBlizzardColEffectObj->Component_Add<SpriteRenderer2D>(RENDER_ORDER::SKILL_EFF3);
			ColEffectRenderer->SetSprite(L"Blizzard_ObjectShortHit.png", 0);
			My_PTR<Animation2D> ColEffectAnim = NewBlizzardColEffectObj->Component_Add<Animation2D>();
			My_PTR<BlizzardHitEffect> NewBlizzard = NewBlizzardColEffectObj->Component_Add<BlizzardHitEffect>();
			NewBlizzard->SetBlizzardIceAniScript(NewBliScript);
			NewBlizzard->SetBlizzardKind(1);
		}
		else
		{
			// Blizzard_ObjectShortHit
			if (L"Left" == CurDir)
			{
				BlizzardIceObj->GetTransform()->Local_Position(Vector4(PresentMonsterPos.x, PresentMonsterPos.y, PosZ, 1.0f));
				BlizzardIceObj->GetTransform()->Local_Scale(Vector4(195.0f, 216.0f, 4.0f, 1.0f));
			}
			else
			{
				BlizzardIceObj->GetTransform()->Local_Position(Vector4(PresentMonsterPos.x, PresentMonsterPos.y, PosZ, 1.0f));
				BlizzardIceObj->GetTransform()->Local_Scale(Vector4(-195.0f, 216.0f, 4.0f, 1.0f));
			}

			My_PTR<SpriteRenderer2D> BSpriteRen = BlizzardIceObj->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF2);
			BSpriteRen->SetSprite(L"Blizzard_ObjectShortHit.png", 0);
			My_PTR<Animation2D> BAnim = BlizzardIceObj->Component_Add<Animation2D>();
			My_PTR<BlizzardIce> NewBliScript = BlizzardIceObj->Component_Add<BlizzardIce>();

			My_PTR<GameObject> NewBlizzardColEffectObj = GetGameObject()->GetScene()->GameObject_Create(L"BlizzardEffect");
			NewBlizzardColEffectObj->GetTransform()->Local_Position(BlizzardIceObj->GetTransform()->Local_Position());
			My_PTR<SpriteRenderer2D> ColEffectRenderer = NewBlizzardColEffectObj->Component_Add<SpriteRenderer2D>(RENDER_ORDER::SKILL_EFF3);
			ColEffectRenderer->SetSprite(L"Blizzard_ObjectShortHit.png", 0);
			My_PTR<Animation2D> ColEffectAnim = NewBlizzardColEffectObj->Component_Add<Animation2D>();
			My_PTR<BlizzardHitEffect> NewBlizzard = NewBlizzardColEffectObj->Component_Add<BlizzardHitEffect>();
			NewBlizzard->SetBlizzardIceAniScript(NewBliScript);
			NewBlizzard->SetBlizzardKind(2);
		}

		--PosZ;
		++MonsterStart;
	}
}

// 포이즌 미스트
void SkillManager::Create_PoisonMist()
{
	SoundPlay_Effect(L"Use_PoisonMist.mp3");
	PlayerMove* CurPlayerMoveScript = GetGameObject()->GetComponent<PlayerMove>();
	CurPlayerMoveScript->AttackAniChange();

	Vector4 PBasePos = GetGameObject()->GetTransform()->Local_Position();

	std::wstring SCount = std::to_wstring(PCCount);
	std::wstring SkillName = L"PoisonMist" + SCount;
	My_PTR<GameObject> PC1 = GetGameObject()->GetScene()->GameObject_Create(SkillName.c_str());
	PC1->GetTransform()->Local_Position(Vector4(PBasePos.x, PBasePos.y + 150.0f, 10.0f, 1.0f));
	PC1->GetTransform()->Local_Scale(Vector4(309.0f, 277.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> PoisonMistSpRenderer = PC1->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF2);
	PoisonMistSpRenderer->SetSprite(L"PoisonMist_Effect.png", 0);
	My_PTR<Animation2D> PoisonMistAnim = PC1->Component_Add<Animation2D>();
	My_PTR<SSoulPenetration> SkillScript = PC1->Component_Add<SPoisonMist>();
}

// 소울 페네트래이션
void SkillManager::Create_SoulPenetration()
{
	SoundPlay_Effect(L"Use_SoulPenetration.mp3");
	PlayerMove* CurPlayerMoveScript = GetGameObject()->GetComponent<PlayerMove>();
	CurPlayerMoveScript->AttackAniChange();

	Vector4 PBasePos = GetGameObject()->GetTransform()->Local_Position();

	std::wstring SCount = std::to_wstring(SPRCount);
	std::wstring SkillName = L"SoulPenetration" + SCount;
	My_PTR<GameObject> SPR1 = GetGameObject()->GetScene()->GameObject_Create(SkillName.c_str());
	SPR1->GetTransform()->Local_Position(Vector4(PBasePos.x, PBasePos.y + 200.0f, 2.0f, 1.0f));
	SPR1->GetTransform()->Local_Scale(Vector4(920.0f, 548.0f, 1.0f, 1.0f));

	My_PTR<SpriteRenderer2D> SoulPenetrationRenderer = SPR1->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF2);
	SoulPenetrationRenderer->SetSprite(L"SoulPenetration_Effect.png", 0);
	My_PTR<Animation2D> SoulPenetrationAnim = SPR1->Component_Add<Animation2D>();
	My_PTR<SSoulPenetration> SkillScript = SPR1->Component_Add<SSoulPenetration>();
}

// 카타클리즘
void SkillManager::Create_Cataclysm()
{
	SoundPlaye_Voice(L"SKill_33.mp3");
	PlayerMove* CurPlayerMoveScript = GetGameObject()->GetComponent<PlayerMove>();
	CurPlayerMoveScript->AttackAniChange();

	Vector4 PBasePos = GetGameObject()->GetTransform()->Local_Position();

	std::wstring SCount = std::to_wstring(CTCCount);
	std::wstring SkillName = L"Cataclysm" + SCount;

	My_PTR<GameObject> CAC1 = GetGameObject()->GetScene()->GameObject_Create(SkillName.c_str());
	CAC1->GetTransform()->Local_Position(Vector4(PBasePos.x, PBasePos.y + 50.0f, 2.0f, 1.0f));
	CAC1->GetTransform()->Local_Scale(Vector4(249.0f, 285.0f, 1.0f, 1.0f));

	My_PTR<SpriteRenderer2D> CataRenderer = CAC1->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF13);
	CataRenderer->SetSprite(L"Cataclysm_Effect.png", 0);

	My_PTR<Animation2D> CataAnim = CAC1->Component_Add<Animation2D>();
	My_PTR<SCataclysm> SkillScript = CAC1->Component_Add<SCataclysm>();


	//
	SkillName = L"Effect0" + SCount;

	My_PTR<GameObject> CataEffectValueObj = GetGameObject()->GetScene()->GameObject_Create(SkillName.c_str());
	CataEffectValueObj->GetTransform()->Local_Position(Vector4(PBasePos.x, PBasePos.y - 40.0f, PBasePos.z + 8.0f, 1.0f));
	CataEffectValueObj->GetTransform()->Local_Scale(Vector4(201.0f, 57.0f, 1.0f, 1.0f));

	My_PTR<SpriteRenderer2D> CataEffectValueRenderer = CataEffectValueObj->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF2);
	CataEffectValueRenderer->SetSprite(L"Cataclysm_Effect0.png", 0);

	My_PTR<Animation2D> CataEffVAnim = CataEffectValueObj->Component_Add<Animation2D>();
	My_PTR<CataclysmEffectValue> EffSkillScript = CataEffectValueObj->Component_Add<CataclysmEffectValue>();
	

	// 운석..?
	std::wstring CataCount = std::to_wstring(CTCCount);
	SkillName = GetGameObject()->GetName();
	SkillName += L"Scene";
	SkillName += CataCount;

	My_PTR<GameObject> NewCataclysmSceneObj = GetGameObject()->GetScene()->GameObject_Create(SkillName.c_str());
	NewCataclysmSceneObj->GetTransform()->Local_Position(Vector4(PBasePos.x, PBasePos.y + 200.0f, 30.0f, 1.0f));
	NewCataclysmSceneObj->GetTransform()->Local_Scale(Vector4(1532.0f, 1100.0f, 1.0f, 1.0f));
	//NewCataclysmSceneObj->GetTransform()->Local_Scale(Vector4(1024.0f, 735.0f, 1.0f, 1.0f));
	
	My_PTR<SpriteRenderer2D> NewCataRenderer = NewCataclysmSceneObj->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF2);
	NewCataRenderer->SetSprite(L"Cataclysm_Scene01.png", 0);

	My_PTR<Animation2D> NewCataAnim = NewCataclysmSceneObj->Component_Add<Animation2D>();
	My_PTR<CataclysmScene> NewCataScript = NewCataclysmSceneObj->Component_Add<CataclysmScene>();
	NewCataScript->CreateAnimation(0);

	// 배경그림
	SkillName = L"CataBaseScene" + CataCount;
	My_PTR<GameObject> NewBaseSceneObj = GetGameObject()->GetScene()->GameObject_Create(SkillName.c_str());
	NewBaseSceneObj->GetTransform()->Local_Position(Vector4(PBasePos.x, PBasePos.y - 100.0f, 20.0f, 1.0f));
	NewBaseSceneObj->GetTransform()->Local_Scale(Vector4(2000.0f, 1500.0f, 1.0f, 1.0f));

	My_PTR<SpriteRenderer2D> NewBaseSceneRenderer = NewBaseSceneObj->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF9);
	NewBaseSceneRenderer->SetSprite(L"Cataclysm_Scene00.png", 0);
	My_PTR<Animation2D> NewBaseSceneAni = NewBaseSceneObj->Component_Add<Animation2D>();
	My_PTR<CataclysmSceneBase> NewBaseSceneScript = NewBaseSceneObj->Component_Add<CataclysmSceneBase>();
}