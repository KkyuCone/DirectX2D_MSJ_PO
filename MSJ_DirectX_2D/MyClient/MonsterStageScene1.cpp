#include "MonsterStageScene1.h"
#include <Scene.h>
#include <TownPortalAniScript.h>

#include <ChildManneqin.h>
#include <WhiteSandRabbit.h>
#include <GeneralMonster_HP.h>
#include <HPBarMoveScript.h>

#include <InGameCamera.h>

#include <PlayerMove.h>
#include <PlayerAnimation.h>
#include <SkillManager.h>
#include <UIMove.h>

#include <Texture.h>
#include <Sprite.h>
#include <RectCol2D.h>
#include <GameDebug.h>
#include <InGameCamera.h>
#include <SubTransform.h>

#include <MapleGlobalValue.h>

// 인벤토리
#include <Inven_BtAnimation.h>
#include <Inven_BtKind.h>
#include <Inven_Info.h>
#include <ItemCode.h>
#include <Inven_MouseItem.h>
#include <Inven_MoveScript.h>

// 돈
#include <MesoManager.h>

// 커서
#include <MouseCursorScript.h>
#include <WorldMouseCursorScript.h>

// 퀵슬롯
#include <QuickSlotScript.h>
#include <CoolTimeScript.h>

// 몬스터힛
#include <MonsterHitScript.h>

// 데미지스킨
#include <MonsterDamageSkin.h>

// 아이템
#include <ItemMoveScript.h>

// 사운드
#include <SoundPlayer.h>

MonsterStageScene1::MonsterStageScene1()
	: m_PrePortal(nullptr), m_NextPortal(nullptr)
{
}


MonsterStageScene1::~MonsterStageScene1()
{
}


//

bool MonsterStageScene1::Loading()
{
	// 사운드
	BGMSoundLoading();
	EffectSoundLoading();
	VoiceSoundLoading();

	CollisionNameLink();

	BGLoading();
	UILoading();
	PortalLoading();
	CreateCamera();
	PlayerLoading();
	CameraScriptLoading();
	MonsterPosInit();
	MonsterLoading();


	// 인벤토리
	InventoryLoading();

	// 퀵슬롯
	QuickSlotLoading();


	// 마우스
	MouseLoading();

	// 돈 관련
	MoneyLoading();


	// 인벤토리 끄기
	InventoryOff();


	// 몬스터들 스킬매니저에 넣어주기
	for (size_t i = 0; i < m_ChildMannequinMonsterVector.size(); ++i)
	{
		m_SkillManager->SetMonster(m_ChildMannequinMonsterVector[i]);
	}

	for (size_t i = 0; i < m_WhiteRabbitMonsterVector.size(); ++i)
	{
		m_SkillManager->SetMonster(m_WhiteRabbitMonsterVector[i]);
	}


	return true;
}

void MonsterStageScene1::Start()
{
	m_GamePlayer->GetTransform()->Local_Position(Vector4(0.0f, 100.0f, 100.0f));
	m_MainCamera->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -10.0f));

	SoundPlay_BGM(L"itsmonstersLife.mp3");

	for (size_t i = 0; i < MapleGlobalValue::InvenItemConsum.size(); ++i)
	{
		ItemCode::ItemCodeName NewInvenItemCode = MapleGlobalValue::InvenItemConsum[i]->GetComponent<Inven_Info>()->GetItemCode();
		int NewCount = MapleGlobalValue::InvenItemConsum[i]->GetComponent<Inven_Info>()->GetItemCount();

		m_InvenItemConsum[i]->GetComponent<Inven_Info>()->ItemAdd(NewInvenItemCode);
		m_InvenItemConsum[i]->GetComponent<Inven_Info>()->SetItemCount(NewCount);
	}

	for (size_t i = 0; i < MapleGlobalValue::QuickSlotVector.size(); ++i)
	{
		ItemCode::ItemCodeName NewQuickSlotItemCode = MapleGlobalValue::QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName();
		float NewCoolTime = MapleGlobalValue::QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCoolTime();
		std::wstring NewSkilName = MapleGlobalValue::QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetSkillName();
		int NewCount = MapleGlobalValue::QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCount();


		m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetConsumeCodeName(NewQuickSlotItemCode);
		m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetCoolTime(NewCoolTime);
		m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillName(NewSkilName);

		SkillManager* NewSkillManagerScript = MapleGlobalValue::QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetSkillManager();
		if (nullptr != NewSkillManagerScript)
		{
			// 널이아니면 현재 캐릭터의 스킬매니저를 넣어준다.
			m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillManager(m_SkillManager);

			// 그리고 스킬이 현재 쿨타임인지 알아야한다. (쿨타임 체크) -> 똑같이 해주기

			CoolTimeScript* NewCoolTimeScirpt = MapleGlobalValue::QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCoolTimeScript();
			std::wstring CurSkillCoolAniName = NewCoolTimeScirpt->GetCurAnimationName();

			if (L"Cool" == CurSkillCoolAniName)
			{
				// 나도 쿨타임 넣어준다
				float NewCurCoolTime = NewCoolTimeScirpt->GetCurCoolTime();
				int NewCurAniFrame = NewCoolTimeScirpt->GetCurCoolTimeFrame();

				CoolTimeScript* NewCoolTimeScript = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCoolTimeScript();
				NewCoolTimeScript->SetCoolTimeChange(NewCoolTime);
				NewCoolTimeScript->ChangeCoolTimeAni();
				NewCoolTimeScript->SetCoolTimeAnimationCurTime(NewCurCoolTime);
				NewCoolTimeScript->SetCoolTimeAnimationFrame(NewCurAniFrame);
			}

		}
		else 
		{
			// null이라면 현재 스킬매니저에다가 nullptr을 넣어준다
			m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillManager(nullptr);

			// 소비템인지도 확인하기
			GameObject* NewInventory = MapleGlobalValue::QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetInventoryItem();
			if (nullptr != NewInventory)
			{
				// 소비템임 
				// 소비템일 경우 인벤토리도 검사해야한다.
				// 아이템 코드명이 일치해야하고 개수도 맞아야한다.
				for (size_t k = 0; k < m_InvenItemConsum.size(); ++k)
				{
					ItemCode::ItemCodeName InventoryItemCode = m_InvenItemConsum[k]->GetComponent<Inven_Info>()->GetItemCode();
					int InvenCount = m_InvenItemConsum[k]->GetComponent<Inven_Info>()->GetItemCount();

					if (InventoryItemCode == NewQuickSlotItemCode && InvenCount == NewCount)
					{
						m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetInventoryItem(m_InvenItemConsum[k]);
					}

				}
			}
			else
			{
				// 그냥 빈칸임
				m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetInventoryItem(nullptr);
			}

		}
	}

	// 캐릭터 포스
	m_GamePlayer->GetTransform()->Local_Position(MapleGlobalValue::PlayerPostion);
	m_CamScript->SetMovePos(MapleGlobalValue::CameraMovePos);
	
}

void MonsterStageScene1::Update()
{
	int a = 0;
	// 인벤토리 켜기
	if (true == Input::Down(L"Inventory"))
	{
		if (false == m_InventoryEmptyObj->IsUpdate())
		{
			InventoryOn();
		}
	}
}

void MonsterStageScene1::LateUpdate()
{

}

void MonsterStageScene1::End()
{

}

void MonsterStageScene1::DebugRender()
{
	// FPS
	//wchar_t CurFPS[256];
	//swprintf_s(CurFPS, L"FPS = %d", Time::FPS());
	//GameDebug::DrawStr(CurFPS, 20, { 10.0f, 30.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);

	//// 캐릭터 포스
	//wchar_t PosXString[256];
	//float PlayerX = m_GamePlayer->GetTransform()->Local_Position().x;
	//swprintf_s(PosXString, L"X = %d", (int)PlayerX);
	//GameDebug::DrawStr(PosXString, 20, { 10.0f, 60.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);

	//wchar_t PosYString[256];
	//float PlayerY = m_GamePlayer->GetTransform()->Local_Position().y;
	//swprintf_s(PosYString, L"Y = %d", (int)PlayerY);
	//GameDebug::DrawStr(PosYString, 20, { 150.0f, 60.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);
}

//

void MonsterStageScene1::BGLoading()
{
	// 뒷배경
	m_Background = GetScene()->GameObject_Create(L"ChewChewIsland_Bacground");
	m_Background->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 300.0f));
	m_Background->GetTransform()->Local_Scale(Vector4(2395.0f, 1256.0f, 1.0f));
	m_Background->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BACKGROUND)->SetSprite(L"ChewChewIsland_BG.png", 0);
	My_PTR<Animation2D> BGANi = m_Background->Component_Add<Animation2D>();

	// 충돌할 타일
	m_MoveTile = GetScene()->GameObject_Create(L"ChewChewIsland_MoveTile");
	m_MoveTile->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 400.0f));
	m_MoveTile->GetTransform()->Local_Scale(Vector4(2395.0f, 1256.0f, 1.0f));
	m_MoveTile->Component_Add<SpriteRenderer2D>(RENDER_ORDER::COLTILE)->SetSprite(L"ChewChewIsland_MoeTile.png", 0);
}

void MonsterStageScene1::UILoading()
{
	m_StatusBarBackground = GetScene()->GameObject_Create(L"StatusBackground");
	m_StatusBarBackground->GetTransform()->Local_Position(Vector4(0.0f, -349.0f, -2.0f));
	m_StatusBarBackground->GetTransform()->Local_Scale(Vector4(1024.0f, 71.0f, 1.0f));
	m_StatusBarBackground->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_BG)->SetSprite(L"StatusBarBaseBackground.png", 0);

	m_StatusBarBase = GetScene()->GameObject_Create(L"StatusBaseBackground");
	m_StatusBarBase->GetTransform()->Local_Position(Vector4(-225.0f, -349.0f, -4.0f));
	m_StatusBarBase->GetTransform()->Local_Scale(Vector4(570.0f, 71.0f, 1.0f));
	m_StatusBarBase->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_BG)->SetSprite(L"StatusBarBaseBackgrnd2.png", 0);

	m_StatusBarGauge = GetScene()->GameObject_Create(L"StatusBarGauge");
	m_StatusBarGauge->GetTransform()->Local_Position(Vector4(-124.0f, -366.0f, -6.0f));
	m_StatusBarGauge->GetTransform()->Local_Scale(Vector4(340.0f, 31.0f, 1.0f));
	m_StatusBarGauge->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT)->SetSprite(L"StatusBarGaugeBar.png", 0);

	m_HPGray = GetScene()->GameObject_Create(L"HPGray");
	m_HPGray->GetTransform()->Local_Position(Vector4(-185.0f, -373.0f, -8.0f));
	m_HPGray->GetTransform()->Local_Scale(Vector4(1.0f, 16.0f, 1.0f));
	m_HPGray->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT)->SetSprite(L"StatusBarGaugeGray.png", 0);

	m_MPGray = GetScene()->GameObject_Create(L"MPGray");
	m_MPGray->GetTransform()->Local_Position(Vector4(-77.0f, -373.0f, -8.0f));
	m_MPGray->GetTransform()->Local_Scale(Vector4(1.0f, 16.0f, 1.0f));
	m_MPGray->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT)->SetSprite(L"StatusBarGaugeGray.png", 0);

	m_ExpGray = GetScene()->GameObject_Create(L"EXPGray");
	m_ExpGray->GetTransform()->Local_Position(Vector4(44.0f, -373.0f, -8.0f));
	m_ExpGray->GetTransform()->Local_Scale(Vector4(1.0f, 16.0f, 1.0f));
	m_ExpGray->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT)->SetSprite(L"StatusBarGaugeGray.png", 0);

	m_GaugeGraduation = GetScene()->GameObject_Create(L"StatusBarGauge_Graduation");
	m_GaugeGraduation->GetTransform()->Local_Position(Vector4(-124.0f, -365.0f, -7.0f));
	m_GaugeGraduation->GetTransform()->Local_Scale(Vector4(340.0f, 31.0f, 1.0f));
	m_GaugeGraduation->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT)->SetSprite(L"StatusBarGaugeGraduation.png", 0);
}

void MonsterStageScene1::CreateCamera()
{
	// 메인 카메라
	m_MainCamera = GetScene()->GameObject_Create(L"MainCamera");
	m_CameraCom = m_MainCamera->Component_Add<Camera>(0);
	m_CameraCom->SettingViewGroup(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129,130, 131, 132, 133);
	m_CameraCom->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -10.0f));

	// UI 카메라
	m_UICamera = GetScene()->GameObject_Create(L"UICamera");
	m_UICameraCom = m_UICamera->Component_Add<Camera>(1);
	m_UICameraCom->SettingViewGroup(50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65);
	m_UICameraCom->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -20.0f));
}

void MonsterStageScene1::PortalLoading()
{
	m_PrePortal = GetScene()->GameObject_Create(L"PrePortal_ChewChewIsland");
	m_PrePortal->GetTransform()->Local_Position(Vector4(-1070.0f, -320.0f, 10.0f, 1.0f));
	m_PrePortal->GetTransform()->Local_Scale(Vector4(89.0f, 257.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> PrePortalRenderer = m_PrePortal->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_PORTAL);
	PrePortalRenderer->SetSprite(L"Portal.png", 0);
	My_PTR<Animation2D> PrePortalAnim = m_PrePortal->Component_Add<Animation2D>();
	My_PTR<TownPortalAniScript> PrePortalAniScript = m_PrePortal->Component_Add<TownPortalAniScript>();
	m_PrePortalCol = m_PrePortal->Component_Add<RectCol2D>(L"ChewChewIsland_PrePortalCol");

	m_NextPortal = GetScene()->GameObject_Create(L"NextPortal_ChewChewIsland");
	m_NextPortal->GetTransform()->Local_Position(Vector4(1065.0f, -320.0f, 10.0f, 1.0f));
	m_NextPortal->GetTransform()->Local_Scale(Vector4(89.0f, 257.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> NextPortalRenderer = m_NextPortal->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_PORTAL);
	NextPortalRenderer->SetSprite(L"Portal.png", 0);
	My_PTR<Animation2D> NextPortalAnim = m_NextPortal->Component_Add<Animation2D>();
	My_PTR<TownPortalAniScript> NextPortalAniScript = m_NextPortal->Component_Add<TownPortalAniScript>();
	m_NextPortalCol = m_NextPortal->Component_Add<RectCol2D>(L"ChewChewIsland_NextPortalCol");

}

void MonsterStageScene1::PlayerLoading()
{
	// 캐릭터 부모
	m_GamePlayer = GetScene()->GameObject_Create(L"ChewChewIsland_Player");
	m_GamePlayer->GetTransform()->Local_Position(Vector4(0.0f, 100.0f, 100.0f));
	m_GamePlayer->GetTransform()->Local_Scale(Vector4(128.0f, 128.0f, 1.0f));
	
	m_PlayerCol = m_GamePlayer->Component_Add<RectCol2D>(L"Player");
	m_PlayerCol->Local_SubPosition(Vector4(0.0f, -24.0f, 0.0f, 0.0f));
	m_PlayerCol->Local_SubScale(Vector4(60.0f, 80.0f, 1.0f, 1.0f));
	m_PlayerCol->PushCollisionEnter(&MonsterStageScene1::Col_PlayerEnter, this);
	m_PlayerCol->PushCollisionStay(&MonsterStageScene1::Col_PlayerStay, this);
	m_PlayerCol->PushCollisionExit(&MonsterStageScene1::Col_PlayerExit, this);

	m_PlayerMoveScript = m_GamePlayer->Component_Add<PlayerMove>();
	m_PlayerMoveScript->AddFloor(m_MoveTile);
	m_PlayerMoveScript->SetSoundPlayer(m_EffectSoundPlayer);
	m_SkillManager = m_GamePlayer->Component_Add<SkillManager>();
	m_SkillManager->SetPlayerScript(m_PlayerMoveScript->GetPlayerMoveScript());
	m_SkillManager->SetMapWidth(m_Background->GetTransform()->Local_Scale().x);
	m_GamePlayer->DebugMode_On();


	CharacterChildInit(m_PlayerCoatArm, L"ChewChewIsland_Player_CoatArm", L"CoatArm.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 1.0f);		// 코트 팔
	CharacterChildInit(m_PlayerArm, L"ChewChewIsland_Player_Arm", L"Arm.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 2.0f);					// 팔
	CharacterChildInit(m_PlayerArm, L"ChewChewIsland_Player_Weapon", L"Weapon_Rabbit.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 3.0f);		// 무기
	CharacterChildInit(m_PlayerArm, L"ChewChewIsland_Player_Eye", L"Eye.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 4.0f);					// 눈
	CharacterChildInit(m_PlayerArm, L"ChewChewIsland_Playe_Hair", L"Hair.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 5.0f);					// 헤어
	CharacterChildInit(m_PlayerArm, L"ChewChewIsland_Player_Head", L"Head.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 6.0f);					// 머리
	CharacterChildInit(m_PlayerArm, L"ChewChewIsland_Player_Coat", L"Coat.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 7.0f);					// 코트옷
	CharacterChildInit(m_PlayerArm, L"ChewChewIsland_Player_Body", L"Body.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 8.0f);					// 바디
}

void MonsterStageScene1::CameraScriptLoading()
{
	m_CamScript = m_MainCamera->Component_Add<InGameCamera>();
	m_CamScript->SetCurMap(m_Background);
	m_CamScript->SetPlayer(m_GamePlayer);
}

//

void MonsterStageScene1::CharacterChildInit(My_PTR<GameObject> _GameObject, const wchar_t* _ObjName, const wchar_t* _ImageName, My_PTR<GameObject> _Parent, int _RenderOrder, float _Z)
{
	_GameObject = GetScene()->GameObject_Create(_ObjName);
	_GameObject->SetParent(_Parent);
	_GameObject->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, _Z));
	_GameObject->GetTransform()->Local_Scale(Vector4(1.0f, 1.0f, 1.0f));

	_GameObject->Component_Add<SpriteRenderer2D>(_RenderOrder)->SetSprite(_ImageName, 0);
	_GameObject->Component_Add<Animation2D>();
	_GameObject->Component_Add<PlayerAnimation>();
	_GameObject->GetComponent<PlayerAnimation>()->SetPaleyrMoveScript(_Parent->GetComponent<PlayerMove>()->GetPlayerScript());
	_GameObject->DebugMode_On();
}


void MonsterStageScene1::MonsterPosInit()
{
	ChildMannequinPosInit();
	WhiteSandRabbitPosInit();
}

void MonsterStageScene1::ChildMannequinPosInit()
{
	std::vector<Vector4> NewPosVector;

	Vector4 NewPos = Vector4(1086.0f, -200.0f, 150.0f, 1.0f);
	NewPosVector.push_back(NewPos);

	NewPos.x = 1037.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 960.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 1521.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	//
	NewPos.y = 40.0f;

	NewPos.x = 503.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 668.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 638.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 721.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);


	//
	NewPos.y = 300.0f;

	NewPos.x = 815.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 965.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 1024.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	//
	NewPos.y = -196.0f;

	NewPos.x = -910.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -1020.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -816.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	//
	NewPos.y = 280.0f;

	NewPos.x = -856.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -980.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	for (size_t i = 0; i < NewPosVector.size(); ++i)
	{
		ChewChewIslandMonsterPos NewVelPos;
		NewVelPos.MonsterPos = NewPosVector[i];
		NewVelPos.m_bTile = true;
		NewVelPos.m_life = true;

		m_ChildMannequinMosnterPos.push_back(NewVelPos);
	}

}

void MonsterStageScene1::WhiteSandRabbitPosInit()
{
	std::vector<Vector4> NewPosVector;

	Vector4 NewPos = Vector4(-904.0f, -390.0f, 150.0f, 1.0f);
	NewPosVector.push_back(NewPos);

	NewPos.x = -753.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -656.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -537.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -435.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -309.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -180.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -78.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 100.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 204.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 337.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 448.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 555.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 699.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 801.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	for (size_t i = 0; i < NewPosVector.size(); ++i)
	{
		ChewChewIslandMonsterPos NewVelPos;
		NewVelPos.MonsterPos = NewPosVector[i];
		NewVelPos.m_bTile = true;
		NewVelPos.m_life = true;

		m_WhiteSandRabbitMonsterPos.push_back(NewVelPos);
	}
}


void MonsterStageScene1::MonsterLoading()
{
	// 몬스터 로딩
	// 아이 마네킹
	for (int i = 0; i < m_ChildMannequinMosnterPos.size(); ++i)
	{
		std::wstring MonName = L"ChildMannequin_";
		std::wstring MonNumber = std::to_wstring(i);
		MonName += MonNumber;

		My_PTR<GameObject> NewChildMannequin = GetScene()->GameObject_Create(MonName.c_str());
		NewChildMannequin->GetTransform()->Local_Position(m_ChildMannequinMosnterPos[i].MonsterPos);
		NewChildMannequin->GetTransform()->Local_Scale(Vector4(118.0f, 137.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> NewChildMannequinRenderer = NewChildMannequin->Component_Add<SpriteRenderer2D>(RENDER_ORDER::MONSTER);
		NewChildMannequinRenderer->SetSprite(L"ChildMannequin.png", 0);
		My_PTR<Animation2D> NewChildMannequinAnim = NewChildMannequin->Component_Add<Animation2D>();
		My_PTR<ChildManneqin> NewCHildMannequinScript = NewChildMannequin->Component_Add<ChildManneqin>();
		NewCHildMannequinScript->SetFloor(m_MoveTile);
		NewCHildMannequinScript->SetPlayer(m_GamePlayer);
		NewCHildMannequinScript->SetBackground(m_Background->GetTransform()->Local_Scale());
		My_PTR<MonsterDamageSkin> NewChildMannequinDamageSkinScript = NewChildMannequin->Component_Add<MonsterDamageSkin>();
		My_PTR<RectCol2D> ChildMannequinCol = NewChildMannequin->Component_Add<RectCol2D>(L"ChildMannequinCol");
		ChildMannequinCol->Local_SubPosition(Vector4(0.0f, -20.0f, 0.0f, 0.0f));
		ChildMannequinCol->Local_SubScale(Vector4(57.0f, 96.0f, 1.0f, 1.0f));
		ChildMannequinCol->PushCollisionEnter(&MonsterStageScene1::Col_MonsterEnter, this);
		ChildMannequinCol->PushCollisionStay(&MonsterStageScene1::Col_MonsterStay, this);
		ChildMannequinCol->PushCollisionExit(&MonsterStageScene1::Col_MonsterExit, this);


		// HP바
		// 베이스
		std::wstring EmptyName = MonName;
		EmptyName += L"_HPBarEmtpyOBjCM";
		My_PTR<GameObject> HPBarEmptyObj = GetScene()->GameObject_Create(EmptyName.c_str());
		HPBarEmptyObj->GetTransform()->Local_Position(NewChildMannequin->GetTransform()->Local_Position());
		HPBarEmptyObj->GetTransform()->Local_Scale(Vector4::One);
		My_PTR<HPBarMoveScript> HPBarEmtpyObjScript = HPBarEmptyObj->Component_Add<HPBarMoveScript>();
		HPBarEmtpyObjScript->SetMosnter(NewChildMannequin);
		HPBarEmtpyObjScript->SetPivot(40.0f);

		std::wstring BaseBarName = (MonName += L"HPBarBase");
		My_PTR<GameObject> HPBarBaseObj = GetScene()->GameObject_Create(BaseBarName.c_str());
		HPBarBaseObj->SetParent(HPBarEmptyObj);
		HPBarBaseObj->GetTransform()->Local_Position(Vector4::One);
		HPBarBaseObj->GetTransform()->Local_Scale(Vector4(50.0f, 8.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> BaseBarRenderer = HPBarBaseObj->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BG_Layer07);
		BaseBarRenderer->SetSprite(L"MonsterHP_FULL.png", 0);

		BaseBarName = (MonName += L"HPBarGray");
		My_PTR<GameObject> HPBarGrayObj = GetScene()->GameObject_Create(BaseBarName.c_str());
		HPBarGrayObj->SetParent(HPBarEmptyObj);
		Vector4 BasePosVector = HPBarBaseObj->GetTransform()->Local_Position();
		BasePosVector.z -= 1.0f;
		HPBarGrayObj->GetTransform()->Local_Position(BasePosVector);
		HPBarGrayObj->GetTransform()->Local_Scale(Vector4(50.0f, 8.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> GrayBarRenderer = HPBarGrayObj->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BG_Layer07);
		GrayBarRenderer->SetSprite(L"MonsterHP_Gray.png", 0);

		BaseBarName = (MonName += L"HPBarGray_Damage");
		My_PTR<GameObject> HPBarDamageObj = GetScene()->GameObject_Create(BaseBarName.c_str());
		HPBarDamageObj->SetParent(HPBarEmptyObj);
		Vector4 DamagePosVector = BasePosVector;
		DamagePosVector.x += 25.0f;
		DamagePosVector.z -= 1.0f;
		HPBarDamageObj->GetTransform()->Local_Position(DamagePosVector);
		HPBarDamageObj->GetTransform()->Local_Scale(Vector4(0.0f, 6.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> DamageBarRenderer = HPBarDamageObj->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BG_Layer07);
		DamageBarRenderer->SetSprite(L"MonsterHP_GrayDamage.png", 0);
		My_PTR<GeneralMonster_HP> DamageScript = HPBarDamageObj->Component_Add<GeneralMonster_HP>();
		DamageScript->SetHP(NewCHildMannequinScript->GetHP());
		DamageScript->SetHPBarEtc(HPBarGrayObj, HPBarBaseObj, HPBarEmptyObj);


		NewCHildMannequinScript->SetHPBar(DamageScript);

		m_ChildMannequinMonsterVector.push_back(NewChildMannequin);
		m_ChildMannequinMonsterCol.push_back(ChildMannequinCol);


	}


	// 흰모래토끼

	for (size_t i = 0; i < m_WhiteSandRabbitMonsterPos.size(); ++i)
	{
		std::wstring MonName = L"WhiteSandRabbit_";
		std::wstring MonNumber = std::to_wstring(i);
		MonName += MonNumber;

		My_PTR<GameObject> NewChildMannequin = GetScene()->GameObject_Create(MonName.c_str());
		NewChildMannequin->GetTransform()->Local_Position(m_WhiteSandRabbitMonsterPos[i].MonsterPos);
		NewChildMannequin->GetTransform()->Local_Scale(Vector4(106.0f, 86.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> NewChildMannequinRenderer = NewChildMannequin->Component_Add<SpriteRenderer2D>(RENDER_ORDER::MONSTER);
		NewChildMannequinRenderer->SetSprite(L"WhiteSandRabbit.png", 0);
		My_PTR<Animation2D> NewChildMannequinAnim = NewChildMannequin->Component_Add<Animation2D>();
		My_PTR<WhiteSandRabbit> NewCHildMannequinScript = NewChildMannequin->Component_Add<WhiteSandRabbit>();
		NewCHildMannequinScript->SetFloor(m_MoveTile);
		NewCHildMannequinScript->SetPlayer(m_GamePlayer);
		NewCHildMannequinScript->SetBackground(m_Background->GetTransform()->Local_Scale());
		My_PTR<MonsterDamageSkin> NewChildMannequinDamageSkinScript = NewChildMannequin->Component_Add<MonsterDamageSkin>();

		//
		My_PTR<RectCol2D> ChildMannequinCol = NewChildMannequin->Component_Add<RectCol2D>(L"WhiteSandRabbitCol");
		ChildMannequinCol->Local_SubPosition(Vector4(0.0f, -20.0f, 0.0f, 0.0f));
		ChildMannequinCol->Local_SubScale(Vector4(59.0f, 52.0f, 1.0f, 1.0f));
		ChildMannequinCol->PushCollisionEnter(&MonsterStageScene1::Col_MonsterEnter, this);
		ChildMannequinCol->PushCollisionStay(&MonsterStageScene1::Col_MonsterStay, this);
		ChildMannequinCol->PushCollisionExit(&MonsterStageScene1::Col_MonsterExit, this);


		// 베이스
		std::wstring EmptyName = MonName;
		EmptyName += L"_HPBarEmtpyOBj";
		My_PTR<GameObject> HPBarEmptyObj = GetScene()->GameObject_Create(EmptyName.c_str());
		HPBarEmptyObj->GetTransform()->Local_Position(NewChildMannequin->GetTransform()->Local_Position());
		HPBarEmptyObj->GetTransform()->Local_Scale(Vector4::One);
		My_PTR<HPBarMoveScript> HPBarEmtpyObjScript = HPBarEmptyObj->Component_Add<HPBarMoveScript>();
		HPBarEmtpyObjScript->SetMosnter(NewChildMannequin);
		HPBarEmtpyObjScript->SetPivot(30.0f);

		std::wstring BaseBarName = (MonName += L"HPBarBase");
		My_PTR<GameObject> HPBarBaseObj = GetScene()->GameObject_Create(BaseBarName.c_str());
		HPBarBaseObj->SetParent(HPBarEmptyObj);
		HPBarBaseObj->GetTransform()->Local_Position(Vector4::One);
		HPBarBaseObj->GetTransform()->Local_Scale(Vector4(50.0f, 8.0f, 1.0f, 1.0f ));
		My_PTR<SpriteRenderer2D> BaseBarRenderer = HPBarBaseObj->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BG_Layer07);
		BaseBarRenderer->SetSprite(L"MonsterHP_FULL.png", 0);

		BaseBarName = (MonName += L"HPBarGray");
		My_PTR<GameObject> HPBarGrayObj = GetScene()->GameObject_Create(BaseBarName.c_str());
		HPBarGrayObj->SetParent(HPBarEmptyObj);
		Vector4 BasePosVector = HPBarBaseObj->GetTransform()->Local_Position();
		BasePosVector.z -= 1.0f;
		HPBarGrayObj->GetTransform()->Local_Position(BasePosVector);
		HPBarGrayObj->GetTransform()->Local_Scale(Vector4(50.0f, 8.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> GrayBarRenderer = HPBarGrayObj->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BG_Layer07);
		GrayBarRenderer->SetSprite(L"MonsterHP_Gray.png", 0);
	
		BaseBarName = (MonName += L"HPBarGray_Damage");
		My_PTR<GameObject> HPBarDamageObj = GetScene()->GameObject_Create(BaseBarName.c_str());
		HPBarDamageObj->SetParent(HPBarEmptyObj);
		Vector4 DamagePosVector = BasePosVector;
		DamagePosVector.x += 25.0f;
		DamagePosVector.z -= 1.0f;
		HPBarDamageObj->GetTransform()->Local_Position(DamagePosVector);
		HPBarDamageObj->GetTransform()->Local_Scale(Vector4(0.0f, 6.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> DamageBarRenderer = HPBarDamageObj->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BG_Layer07);
		DamageBarRenderer->SetSprite(L"MonsterHP_GrayDamage.png", 0);
		My_PTR<GeneralMonster_HP> DamageScript = HPBarDamageObj->Component_Add<GeneralMonster_HP>();
		DamageScript->SetHP(NewCHildMannequinScript->GetHP());
		DamageScript->SetHPBarEtc(HPBarGrayObj, HPBarBaseObj, HPBarEmptyObj);


		NewCHildMannequinScript->SetHPBar(DamageScript);

		m_WhiteRabbitMonsterVector.push_back(NewChildMannequin);
		m_WhiteSandRabbitMonsterCol.push_back(ChildMannequinCol);
	}
}

void MonsterStageScene1::InventoryLoading()
{
	// 기본 오브젝트들
	m_InventoryEmptyObj = GetScene()->GameObject_Create(L"ChewChewIsland_InvenEmtpyObj");
	m_InventoryEmptyObj->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	m_InventoryEmptyObj->GetTransform()->Local_Scale(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	m_InvenMoveObj = GetScene()->GameObject_Create(L"ChewChewIsland_InventoryMoveObj");
	m_InvenMoveObj->SetParent(m_InventoryEmptyObj);
	m_InvenMoveObj->GetTransform()->Local_Position(Vector4(-20.0f, 154.0f, 1.0f, 1.0f));
	m_InvenMoveObj->GetTransform()->Local_Scale(Vector4(130.0f, 21.0f, 1.0f, 1.0f));

	InvenMoveObjectScript = m_InvenMoveObj->Component_Add<Inven_MoveScript>();				// 움직이는 애 스크립트
	InvenMoveObjectScript->SetCamera(m_UICameraCom);
	InvenMoveObjectScript->SetMoveObject(m_InventoryEmptyObj);

	InvenMoveCol = m_InvenMoveObj->Component_Add<RectCol2D>(L"Chu_InventoryMoveObjCol");		// 움직이는 애 충돌체
	InvenMoveCol->SetUICamera(m_UICameraCom);

	m_InvenBase = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBaseObj");
	m_InvenBase->SetParent(m_InventoryEmptyObj);
	m_InvenBase->GetTransform()->Local_Position(Vector4(1.0f, 1.0f, -1.0f, 1.0f));
	m_InvenBase->GetTransform()->Local_Scale(Vector4(172.0f, 335.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBaseRenderer = m_InvenBase->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBaseRenderer->SetSprite(L"Inven_Base.png", 0);

	m_InvenBtClose = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtClose");
	m_InvenBtClose->SetParent(m_InventoryEmptyObj);
	m_InvenBtClose->GetTransform()->Local_Position(Vector4(70.0f, 154.0f, -2.0f, 1.0f));
	m_InvenBtClose->GetTransform()->Local_Scale(Vector4(13.0f, 13.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtCloseRenderer = m_InvenBtClose->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtCloseRenderer->SetSprite(L"Inven_CloseBt.png", 0);
	My_PTR<Animation2D> InvenBtCloseAni = m_InvenBtClose->Component_Add<Animation2D>();
	InvenBtCloseAniScript = m_InvenBtClose->Component_Add<Inven_BtAnimation>();
	InvenBtCloseCol = m_InvenBtClose->Component_Add<RectCol2D>(L"Chu_InvenBtCloseCol");
	InvenBtCloseCol->SetUICamera(m_UICameraCom);


	// 하위 버튼들
	m_InvenBtDisassemble = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtDisassemble");
	m_InvenBtDisassemble->SetParent(m_InventoryEmptyObj);
	m_InvenBtDisassemble->GetTransform()->Local_Position(Vector4(-65.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtDisassemble->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtDisassembleRenderer = m_InvenBtDisassemble->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtDisassembleRenderer->SetSprite(L"Inven_DisassembleBt.png", 0);
	My_PTR<Animation2D> InvenBtDisassembleAni = m_InvenBtDisassemble->Component_Add<Animation2D>();
	InvenBtDisassembleAniScript = m_InvenBtDisassemble->Component_Add<Inven_BtAnimation>();
	InvenBtDisassembleCol = m_InvenBtDisassemble->Component_Add<RectCol2D>(L"Chu_InvenBtDisassembleCol");
	InvenBtDisassembleCol->SetUICamera(m_UICameraCom);

	m_InvenBtExtract = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtExtract");
	m_InvenBtExtract->SetParent(m_InventoryEmptyObj);
	m_InvenBtExtract->GetTransform()->Local_Position(Vector4(-39.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtExtract->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtExtractRenderer = m_InvenBtExtract->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtExtractRenderer->SetSprite(L"Inven_ExtractBt.png", 0);
	My_PTR<Animation2D> InvenBtExtractAni = m_InvenBtExtract->Component_Add<Animation2D>();
	InvenBtExtractAniScript = m_InvenBtExtract->Component_Add<Inven_BtAnimation>();
	InvenBtExtractCol = m_InvenBtExtract->Component_Add<RectCol2D>(L"Chu_InvenBtExtractCol");
	InvenBtExtractCol->SetUICamera(m_UICameraCom);

	m_InvenBtAppraise = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtAppraise");
	m_InvenBtAppraise->SetParent(m_InventoryEmptyObj);
	m_InvenBtAppraise->GetTransform()->Local_Position(Vector4(-13.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtAppraise->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtAppraise = m_InvenBtAppraise->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtAppraise->SetSprite(L"Inven_AppraiseBt.png", 0);
	My_PTR<Animation2D> InvenBtAppraiseAni = m_InvenBtAppraise->Component_Add<Animation2D>();
	InvenBtAppraiseAniScript = m_InvenBtAppraise->Component_Add<Inven_BtAnimation>();
	InvenBtAppraiseCol = m_InvenBtAppraise->Component_Add<RectCol2D>(L"Chu_InvenBtAppraiseCol");
	InvenBtAppraiseCol->SetUICamera(m_UICameraCom);

	m_InvenBtPot = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtPot");
	m_InvenBtPot->SetParent(m_InventoryEmptyObj);
	m_InvenBtPot->GetTransform()->Local_Position(Vector4(13.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtPot->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtPote = m_InvenBtPot->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtPote->SetSprite(L"Inven_PotBt.png", 0);

	m_InvenBtToad = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtToad");
	m_InvenBtToad->SetParent(m_InventoryEmptyObj);
	m_InvenBtToad->GetTransform()->Local_Position(Vector4(39.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtToad->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtToad = m_InvenBtToad->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtToad->SetSprite(L"Inven_ToadBt.png", 0);
	My_PTR<Animation2D> InvenBtToadAni = m_InvenBtToad->Component_Add<Animation2D>();
	InvenBtToadAniScript = m_InvenBtToad->Component_Add<Inven_BtAnimation>();
	InvenBtToadCol = m_InvenBtToad->Component_Add<RectCol2D>(L"Chu_InvenBtToadCol");
	InvenBtToadCol->SetUICamera(m_UICameraCom);

	m_InvenBtUpgrade = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtUpgrade");
	m_InvenBtUpgrade->SetParent(m_InventoryEmptyObj);
	m_InvenBtUpgrade->GetTransform()->Local_Position(Vector4(65.0f, -147.5f, -2.0f, 1.0f));
	m_InvenBtUpgrade->GetTransform()->Local_Scale(Vector4(24.0f, 24.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtUpgrade = m_InvenBtUpgrade->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtUpgrade->SetSprite(L"Inven_UpgradeBt.png", 0);
	My_PTR<Animation2D> InvenBtUpgradeAni = m_InvenBtUpgrade->Component_Add<Animation2D>();
	InvenBtUpgradeAniScript = m_InvenBtUpgrade->Component_Add<Inven_BtAnimation>();
	InvenBtUpgradeCol = m_InvenBtUpgrade->Component_Add<RectCol2D>(L"Chu__InvenBtUpgradeCol");
	InvenBtUpgradeCol->SetUICamera(m_UICameraCom);

	// 장비 소비 기타 설치 캐시
	m_InvenBtEquip = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtEquip");
	m_InvenBtEquip->SetParent(m_InventoryEmptyObj);
	m_InvenBtEquip->GetTransform()->Local_Position(Vector4(-60.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtEquip->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtEquipRenderer = m_InvenBtEquip->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtEquipRenderer->SetSprite(L"Inven_Kind_Equip.png", 0);
	My_PTR<Animation2D> InvenBtEquipAni = m_InvenBtEquip->Component_Add<Animation2D>();
	InvenBtEquipAniScript = m_InvenBtEquip->Component_Add<Inven_BtKind>();
	InvenBtEquipAniScript->On();
	InvenBtEquipCol = m_InvenBtEquip->Component_Add<RectCol2D>(L"Chu_InvenBtEquipCol");
	InvenBtEquipCol->SetName(L"InvenBtEquipCol");
	InvenBtEquipCol->SetUICamera(m_UICameraCom);

	m_InvenBtConsum = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtComsum");
	m_InvenBtConsum->SetParent(m_InventoryEmptyObj);
	m_InvenBtConsum->GetTransform()->Local_Position(Vector4(-29.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtConsum->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtComsumRenderer = m_InvenBtConsum->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtComsumRenderer->SetSprite(L"Inven_Kind_Comsum.png", 0);
	My_PTR<Animation2D> InvenBtComsumAni = m_InvenBtConsum->Component_Add<Animation2D>();
	InvenBtComsumAniScript = m_InvenBtConsum->Component_Add<Inven_BtKind>();
	InvenBtComsumAniScript->Off();
	InvenBtComsumCol = m_InvenBtConsum->Component_Add<RectCol2D>(L"Chu_InvenBtComsumCol");
	InvenBtComsumCol->SetUICamera(m_UICameraCom);

	m_InvenBtEtc = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtEtc");
	m_InvenBtEtc->SetParent(m_InventoryEmptyObj);
	m_InvenBtEtc->GetTransform()->Local_Position(Vector4(2.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtEtc->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtEtcRenderer = m_InvenBtEtc->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtEtcRenderer->SetSprite(L"Inven_Kind_Etc.png", 0);
	My_PTR<Animation2D> InvenBtEtcAni = m_InvenBtEtc->Component_Add<Animation2D>();
	InvenBtEtcAniScript = m_InvenBtEtc->Component_Add<Inven_BtKind>();
	InvenBtEtcAniScript->Off();
	InvenBtEtcCol = m_InvenBtEtc->Component_Add<RectCol2D>(L"Chu_InvenBtEtcCol");
	InvenBtEtcCol->SetUICamera(m_UICameraCom);

	m_InvenBtInstall = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtInstall");
	m_InvenBtInstall->SetParent(m_InventoryEmptyObj);
	m_InvenBtInstall->GetTransform()->Local_Position(Vector4(33.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtInstall->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtInstallRenderer = m_InvenBtInstall->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtInstallRenderer->SetSprite(L"Inven_Kind_Install.png", 0);
	My_PTR<Animation2D> InvenBtInstallAni = m_InvenBtInstall->Component_Add<Animation2D>();
	InvenBtInstallAniScript = m_InvenBtInstall->Component_Add<Inven_BtKind>();
	InvenBtInstallAniScript->Off();
	InvenBtInstallCol = m_InvenBtInstall->Component_Add<RectCol2D>(L"Chu_InvenBtInstallCol");
	InvenBtInstallCol->SetUICamera(m_UICameraCom);

	m_InvenBtCash = GetScene()->GameObject_Create(L"ChewChewIsland_InvenBtCash");
	m_InvenBtCash->SetParent(m_InventoryEmptyObj);
	m_InvenBtCash->GetTransform()->Local_Position(Vector4(64.0f, 132.0f, -2.0f, 1.0f));
	m_InvenBtCash->GetTransform()->Local_Scale(Vector4(30.0f, 19.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenBtCashRenderer = m_InvenBtCash->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_INVENTORY);
	InvenBtCashRenderer->SetSprite(L"Inven_Kind_Cash.png", 0);
	My_PTR<Animation2D> InvenBtCashAni = m_InvenBtCash->Component_Add<Animation2D>();
	InvenBtCashAniScript = m_InvenBtCash->Component_Add<Inven_BtKind>();
	InvenBtCashAniScript->Off();
	InvenBtCashCol = m_InvenBtCash->Component_Add<RectCol2D>(L"Chu_InvenBtCashCol");
	InvenBtCashCol->SetUICamera(m_UICameraCom);

	// 충돌체 묶기
	// 기본
	m_InventoryButtonColVector.push_back(InvenMoveCol);
	m_InventoryButtonColVector.push_back(InvenBtCloseCol);
	m_InventoryButtonColVector.push_back(InvenBtDisassembleCol);
	m_InventoryButtonColVector.push_back(InvenBtExtractCol);
	m_InventoryButtonColVector.push_back(InvenBtAppraiseCol);
	m_InventoryButtonColVector.push_back(InvenBtToadCol);
	m_InventoryButtonColVector.push_back(InvenBtUpgradeCol);
	m_InventoryButtonColVector.push_back(InvenBtEquipCol);
	m_InventoryButtonColVector.push_back(InvenBtComsumCol);
	m_InventoryButtonColVector.push_back(InvenBtEtcCol);
	m_InventoryButtonColVector.push_back(InvenBtInstallCol);
	m_InventoryButtonColVector.push_back(InvenBtCashCol);


	InventoryCreate();
}

void MonsterStageScene1::InventoryCreate()
{
	InventorySlotCreate();

	// 총 모음
	m_AllItems.push_back(m_InvenItemEquip);
	m_AllItems.push_back(m_InvenItemConsum);
	m_AllItems.push_back(m_InvenItemEtc);
	m_AllItems.push_back(m_InvenItemInstall);
	m_AllItems.push_back(m_InvenItemCash);

	// 사용하는 인벤토리 슬롯 온오프
	InventorySlotOn(m_InvenItemEquip);
	InventorySlotOff(m_InvenItemConsum);
	InventorySlotOff(m_InvenItemEtc);
	InventorySlotOff(m_InvenItemInstall);
	InventorySlotOff(m_InvenItemCash);
}

void MonsterStageScene1::InventorySlotCreate()
{
	// 소비
	for (size_t y = 0; y < 6; y++)
	{
		for (size_t x = 0; x < 4; x++)
		{
			Vector4 EmptyItemPos = Vector4(-59.0f, 100.5f, -7.0f, 1.0f);
			EmptyItemPos.x = -59.0f + (36.0f * x);
			EmptyItemPos.y = 100.5f - (35.0f * y);


			My_PTR<GameObject> EmptyItem = GetScene()->GameObject_Create(L"Item");
			EmptyItem->SetParent(m_InventoryEmptyObj);
			EmptyItem->GetTransform()->Local_Position(EmptyItemPos);
			EmptyItem->GetTransform()->Local_Scale(Vector4(30.0f, 31.0f, 1.0f, 1.0f));
			My_PTR<SpriteRenderer2D> EmptyItemRenderer = EmptyItem->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_OBJECT);
			EmptyItemRenderer->SetSprite(L"EmptyItem.png", 0);
			My_PTR<Animation2D> EmptyItemAni = EmptyItem->Component_Add<Animation2D>();
			My_PTR<RectCol2D> EmptyItemCol = EmptyItem->Component_Add<RectCol2D>(L"Chu_InventorySlot_Consume");
			EmptyItemCol->SetUICamera(m_UICameraCom);
			My_PTR<Inven_Info> EmptyItemScript = EmptyItem->Component_Add<Inven_Info>();
			EmptyItemScript->SetItemKind(ItemKind::ITEM_KIND::CONSUME);

			m_InvenItemConsum.push_back(EmptyItem);
			InvenItemConSumCol.push_back(EmptyItemCol);
		}
	}
}

void MonsterStageScene1::InventoryOff()
{
	m_InvenMoveObj->Update_Off();
	m_InvenBase->Update_Off();

	m_InvenBtClose->Update_Off();
	m_InvenBtDisassemble->Update_Off();
	m_InvenBtExtract->Update_Off();
	m_InvenBtAppraise->Update_Off();
	m_InvenBtPot->Update_Off();
	m_InvenBtToad->Update_Off();
	m_InvenBtUpgrade->Update_Off();

	m_InvenBtEquip->Update_Off();
	m_InvenBtConsum->Update_Off();
	m_InvenBtEtc->Update_Off();
	m_InvenBtInstall->Update_Off();
	m_InvenBtCash->Update_Off();

	m_InventoryEmptyObj->Update_Off();

	// 슬롯
	InventorySlotOff(m_InvenItemEquip);
	InventorySlotOff(m_InvenItemConsum);
	InventorySlotOff(m_InvenItemEtc);
	InventorySlotOff(m_InvenItemInstall);
	InventorySlotOff(m_InvenItemCash);

	// 돈
	m_InvenMoenyManagerScript->Money_Off();
}

void MonsterStageScene1::InventoryOn()
{
	m_InvenMoveObj->Update_On();
	m_InvenBase->Update_On();

	m_InvenBtClose->Update_On();
	m_InvenBtDisassemble->Update_On();
	m_InvenBtExtract->Update_On();
	m_InvenBtAppraise->Update_On();
	m_InvenBtPot->Update_On();
	m_InvenBtToad->Update_On();
	m_InvenBtUpgrade->Update_On();

	m_InvenBtEquip->Update_On();
	m_InvenBtConsum->Update_On();
	m_InvenBtEtc->Update_On();
	m_InvenBtInstall->Update_On();
	m_InvenBtCash->Update_On();

	m_InventoryEmptyObj->Update_On();

	// 슬롯
	InventorySlotOff(m_InvenItemEquip);
	InventorySlotOff(m_InvenItemConsum);
	InventorySlotOff(m_InvenItemEtc);
	InventorySlotOff(m_InvenItemInstall);
	InventorySlotOff(m_InvenItemCash);

	if (true == InvenBtEquipAniScript->IsOn())
	{
		InventorySlotOn(m_InvenItemEquip);
	}
	else if (true == InvenBtComsumAniScript->IsOn())
	{
		InventorySlotOn(m_InvenItemConsum);
	}
	else if (true == InvenBtEtcAniScript->IsOn())
	{
		InventorySlotOn(m_InvenItemEtc);
	}
	else if (true == InvenBtInstallAniScript->IsOn())
	{
		InventorySlotOn(m_InvenItemInstall);
	}
	else if (true == InvenBtCashAniScript->IsOn())
	{
		InventorySlotOn(m_InvenItemCash);
	}


	m_InvenMoenyManagerScript->Money_On();
}

void MonsterStageScene1::InventorySlotOff(std::vector<GameObject*> _Slot)
{
	for (size_t i = 0; i < _Slot.size(); i++)
	{
		//_Slot[i]->Update_Off();
		_Slot[i]->GetComponent<RectCol2D>()->Update_Off();
		_Slot[i]->GetComponent<Inven_Info>()->Number_Off();
		_Slot[i]->GetComponent<Inven_Info>()->RenderOff();
	}
}

void MonsterStageScene1::InventorySlotOn(std::vector<GameObject*> _Slot)
{
	for (size_t i = 0; i < _Slot.size(); i++)
	{
		//_Slot[i]->Update_On();
		_Slot[i]->GetComponent<Inven_Info>()->Number_On();
		_Slot[i]->GetComponent<Inven_Info>()->RenderOn();
		_Slot[i]->GetComponent<RectCol2D>()->Update_On();
	}

}

void MonsterStageScene1::ClickItemInfo(GameObject* _ItemObj)
{
	m_ClickItemInfo = _ItemObj;
}

void MonsterStageScene1::QuickSlotLoading()
{
	// 부모
	m_QuickSlotEmptyObj = GetScene()->GameObject_Create(L"QuickSlotEmptyObject");
	m_QuickSlotEmptyObj->GetTransform()->Local_Position(Vector4(400.0f, -280.0f, 1.0f, 1.0f));
	m_QuickSlotEmptyObj->GetTransform()->Local_Scale(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	// 퀵슬롯 - 뒷배경
	m_QuickSlotBGBack = GetScene()->GameObject_Create(L"QuickSlot_BG_Back");
	m_QuickSlotBGBack->SetParent(m_QuickSlotEmptyObj);
	m_QuickSlotBGBack->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -3.0f, 1.0f));
	m_QuickSlotBGBack->GetTransform()->Local_Scale(Vector4(172.0f, 67.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> QuickSlotBGBackRenderer = m_QuickSlotBGBack->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_QUICKSLOT);
	QuickSlotBGBackRenderer->SetSprite(L"quickSlot.backgrnd.png", 0);

	// 퀵슬롯 - 앞배경
	m_QuickSlotBGFront = GetScene()->GameObject_Create(L"QuickSlot_BG_Front");
	m_QuickSlotBGFront->SetParent(m_QuickSlotEmptyObj);
	m_QuickSlotBGFront->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -6.0f, 1.0f));
	m_QuickSlotBGFront->GetTransform()->Local_Scale(Vector4(176.0f, 73.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> QuickSlotBGFrontRenderer = m_QuickSlotBGFront->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_QUICKSLOT);
	QuickSlotBGFrontRenderer->SetSprite(L"quickSlot.layer_cover.png", 0);

	// 퀵슬롯들 (타일맵처럼 만듦 -> Vector에 모아주기)
	Vector4 BasePos = m_QuickSlotEmptyObj->GetTransform()->Local_Position();
	BasePos.x -= 70.0f;
	BasePos.y += 17.0f;
	BasePos.z = -5.0f;

	for (size_t y = 0; y < 2; ++y)
	{
		for (size_t x = 0; x < 5; ++x)
		{
			Vector4 NewCreatePos = BasePos;

			//NewCreatePos.x += (x * (16.0f + 3.0f));
			//NewCreatePos.y -= (y * (16.0f + 3.0f));

			NewCreatePos.x += (x * 35.0f);
			NewCreatePos.y -= (y * 35.0f);

			My_PTR<GameObject> NewQuickSlot = GetScene()->GameObject_Create(L"QuickSlot");
			NewQuickSlot->GetTransform()->Local_Position(Vector4(NewCreatePos));
			NewQuickSlot->GetTransform()->Local_Scale(Vector4(32.0f, 32.0f, 1.0f, 1.0f));
			My_PTR<SpriteRenderer2D> NewQuickSlotRenderer = NewQuickSlot->Component_Add<SpriteRenderer2D>(UI_QUICKSLOT);
			NewQuickSlotRenderer->SetSprite(L"EmptyItem.png", 0);
			My_PTR<QuickSlotScript> NewQuickSlotScript = NewQuickSlot->Component_Add<QuickSlotScript>();
			NewQuickSlotScript->SetSoundPlayer(m_EffectSoundPlayer);
			My_PTR<RectCol2D> NewQuickSlotCollision = NewQuickSlot->Component_Add<RectCol2D>(L"QuickSlotCol");
			NewQuickSlotCollision->SetUICamera(m_UICameraCom);
			NewQuickSlotCollision->Local_SubScale(NewQuickSlot->GetTransform()->Local_Scale());


			// 스킬 5개 넣어주기 ㅇㅅㅇ 
			if (0 == y)
			{
				if (0 == x)
				{
					NewQuickSlotRenderer->SetSprite(L"BlazingExtinction_Icon.png", 0);
					NewQuickSlotScript->SetKey(L"Key_Insert");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_BE);
					NewQuickSlotScript->SetCoolTime(15.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(m_SkillManager);
					NewQuickSlotScript->SetSkillName(L"BlazingExtinction");
				}
				else if (1 == x)
				{
					NewQuickSlotRenderer->SetSprite(L"Blizzard_Icon.png", 0);
					NewQuickSlotScript->SetKey(L"Key_Home");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_BZ);
					NewQuickSlotScript->SetCoolTime(12.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(m_SkillManager);
					NewQuickSlotScript->SetSkillName(L"Blizzard");
				}
				else if (2 == x)
				{
					NewQuickSlotRenderer->SetSprite(L"Cataclysm_Icon.png", 0);
					NewQuickSlotScript->SetKey(L"Key_PageUp");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_CC);
					NewQuickSlotScript->SetCoolTime(180.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(m_SkillManager);
					NewQuickSlotScript->SetSkillName(L"Cataclysm");
				}
				else if (3 == x)
				{
					NewQuickSlotScript->SetKey(L"Key_1");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::EMPTY);
					NewQuickSlotScript->SetCoolTime(0.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(nullptr);
					NewQuickSlotScript->SetSkillName(L"");
				}
				else if (4 == x)
				{
					NewQuickSlotScript->SetKey(L"Key_2");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::EMPTY);
					NewQuickSlotScript->SetCoolTime(0.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(nullptr);
					NewQuickSlotScript->SetSkillName(L"");
				}
			}
			else if (1 == y)
			{
				if (0 == x)
				{
					NewQuickSlotRenderer->SetSprite(L"PoisonMist_Icon.png", 0);
					NewQuickSlotScript->SetKey(L"Key_Delete");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_PM);
					NewQuickSlotScript->SetCoolTime(20.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(m_SkillManager);
					NewQuickSlotScript->SetSkillName(L"PoisonMist");

				}
				else if (1 == x)
				{
					NewQuickSlotRenderer->SetSprite(L"SoulPenetration_Icon.png", 0);
					NewQuickSlotScript->SetKey(L"Key_End");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::SKILL_SP);
					NewQuickSlotScript->SetCoolTime(2.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(m_SkillManager);
					NewQuickSlotScript->SetSkillName(L"SoulPenetration");
				}
				else if (2 == x)
				{
					NewQuickSlotScript->SetKey(L"Key_PageDown");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::EMPTY);
					NewQuickSlotScript->SetCoolTime(0.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(nullptr);
					NewQuickSlotScript->SetSkillName(L"");
				}
				else if (3 == x)
				{
					NewQuickSlotScript->SetKey(L"Key_3");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::EMPTY);
					NewQuickSlotScript->SetCoolTime(0.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(nullptr);
					NewQuickSlotScript->SetSkillName(L"");
				}
				else if (4 == x)
				{
					NewQuickSlotScript->SetKey(L"Key_4");
					NewQuickSlotScript->SetConsumeCodeName(ItemCode::ItemCodeName::EMPTY);
					NewQuickSlotScript->SetCoolTime(0.0f);
					NewQuickSlotScript->SetInventoryItem(nullptr);
					NewQuickSlotScript->SetSkillManager(nullptr);
					NewQuickSlotScript->SetSkillName(L"");
				}
			}

			// std::vector로 관리하기
			m_QuickSlotVector.push_back(NewQuickSlot);
			m_QuickSlotCol.push_back(NewQuickSlotCollision);

		}
	}
}


// 돈
void MonsterStageScene1::MoneyLoading()
{
	m_InvenMoneyManagerObject = GetScene()->GameObject_Create(L"ChewChewIsland_Inven_MoenyManager");
	m_InvenMoneyManagerObject->SetParent(m_InventoryEmptyObj);
	m_InvenMoneyManagerObject->GetTransform()->Local_Position(Vector4(35.5f, -108.0f, 0.0f, 0.0f));
	m_InvenMoneyManagerObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_InvenMoenyManagerScript = m_InvenMoneyManagerObject->Component_Add<MesoManager>();
}

// 마우스
void MonsterStageScene1::MouseLoading()
{
	//Input::Create_Key(L"MouseLeftButton", VK_LBUTTON);


	m_Mouse = GetScene()->GameObject_Create(L"ChewChewIsland_MouseCursor");
	m_Mouse->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -15.0f, 1.0f));
	m_Mouse->GetTransform()->Local_Scale(Vector4(30.0f, 30.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> MouseCursorRenderer = m_Mouse->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_MOUSECURSOR);
	MouseCursorRenderer->SetSprite(L"MouseCursor.png", 0);
	My_PTR<Animation2D> MouseCursorAni = m_Mouse->Component_Add<Animation2D>();
	MouseScript = m_Mouse->Component_Add<MouseCursorScript>();
	MouseScript->SetCamera(m_UICameraCom);

	CursorCol = m_Mouse->Component_Add<RectCol2D>(L"Chu_CursorCol");
	CursorCol->Local_SubPosition(Vector4(-10.0f, 10.0f, 1.0f, 1.0f));
	CursorCol->Local_SubScale(Vector4(3.0f, 3.0f, 1.0f, 1.0f));
	CursorCol->SetName(L"CursorCol");
	CursorCol->SetUICamera(m_UICameraCom);

	CursorCol->PushCollisionEnter(&MonsterStageScene1::Col_InventoryEnter, this);
	CursorCol->PushCollisionStay(&MonsterStageScene1::Col_InventoryStay, this);
	CursorCol->PushCollisionExit(&MonsterStageScene1::Col_InventoryExit, this);


	// 인벤토리에서 마우스 클릭시 아이템 이동하기 위한.. (아이템 이미지를 보여주기 위한 오브젝트 초기화)
	m_InvenClickMouseImage = GetScene()->GameObject_Create(L"ChewChewIsland_Inven_ClickItemImage");
	m_InvenClickMouseImage->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -8.0f, 1.0f));
	m_InvenClickMouseImage->GetTransform()->Local_Scale(Vector4(29.0f, 30.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> InvenClickImageRenderer = m_InvenClickMouseImage->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_ITEM);
	InvenClickImageRenderer->SetSprite(L"EmptyItem.png", 0);
	//InvenClickImageRenderer->SetSprite(L"RedPoiton.png", 0);
	InvenClickImageScript = m_InvenClickMouseImage->Component_Add<Inven_MouseItem>();
	InvenClickImageScript->SetMouseCamera(m_UICameraCom);
}

//
// UI 충돌
void MonsterStageScene1::Col_InventoryEnter(Collision* _This, Collision* _Other)
{
	if (_This == CursorCol)
	{
		if (_Other == InvenMoveCol)
		{
		}

		if (_Other == InvenBtCloseCol)
		{
			// 닫기
			InvenBtCloseAniScript->SetAnimation(L"MouseOver");

		}
		else if (_Other == InvenBtDisassembleCol)
		{
			// 톱니바퀴
			InvenBtDisassembleAniScript->SetAnimation(L"MouseOver");

		}
		else if (_Other == InvenBtExtractCol)
		{
			// 물약버튼
			InvenBtExtractAniScript->SetAnimation(L"MouseOver");
		}
		else if (_Other == InvenBtAppraiseCol)
		{
			// 돋보기
			InvenBtAppraiseAniScript->SetAnimation(L"MouseOver");
		}
		else if (_Other == InvenBtToadCol)
		{
			// 망치 화살표
			InvenBtToadAniScript->SetAnimation(L"MouseOver");
		}
		else if (_Other == InvenBtUpgradeCol)
		{
			// 강화
			InvenBtUpgradeAniScript->SetAnimation(L"MouseOver");

		}
		else if (_Other == InvenBtEquipCol)
		{
			// 장비

		}
		else if (_Other == InvenBtComsumCol)
		{
			// 소비

		}
		else if (_Other == InvenBtEtcCol)
		{
			// 기타

		}
		else if (_Other == InvenBtInstallCol)
		{
			// 설치

		}
		else if (_Other == InvenBtCashCol)
		{
			// 캐시
		}

	}
}

void MonsterStageScene1::Col_InventoryStay(Collision* _This, Collision* _Other)
{
	if (_This == CursorCol)
	{
		if (_Other == InvenMoveCol)
		{
			if (true == Input::DownStay(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				InvenMoveObjectScript->SetbMove(true);
			}
		}

		if (_Other == InvenBtCloseCol)
		{
			// 닫기
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				Vector4 CloseMoveInven = Vector4(0.0f, 1000.0f, 0.0f, 0.0f);

				InvenBtCloseAniScript->SetAnimation(L"Pressed");
				InventoryOff();
			}
		}
		else if (_Other == InvenBtDisassembleCol)
		{
			// 톱니바퀴
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				InvenBtDisassembleAniScript->SetAnimation(L"Pressed");
			}
		}
		else if (_Other == InvenBtExtractCol)
		{
			// 물약버튼
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				InvenBtExtractAniScript->SetAnimation(L"Pressed");
			}
		}
		else if (_Other == InvenBtAppraiseCol)
		{
			// 돋보기
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				InvenBtAppraiseAniScript->SetAnimation(L"Pressed");
			}
		}
		else if (_Other == InvenBtToadCol)
		{
			// 망치 화살표
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				InvenBtToadAniScript->SetAnimation(L"Pressed");
			}
		}
		else if (_Other == InvenBtUpgradeCol)
		{
			// 강화
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				InvenBtUpgradeAniScript->SetAnimation(L"Pressed");
			}
		}
		else if (_Other == InvenBtEquipCol)
		{
			// 장비
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				// 버튼 누르기
				InvenBtEquipAniScript->On();
				InvenBtComsumAniScript->Off();
				InvenBtEtcAniScript->Off();
				InvenBtInstallAniScript->Off();
				InvenBtCashAniScript->Off();

				// 슬롯
				InventorySlotOn(m_InvenItemEquip);
				InventorySlotOff(m_InvenItemConsum);
				InventorySlotOff(m_InvenItemEtc);
				InventorySlotOff(m_InvenItemInstall);
				InventorySlotOff(m_InvenItemCash);
			}
		}
		else if (_Other == InvenBtComsumCol)
		{
			// 소비
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				// 버튼 누르기
				InvenBtEquipAniScript->Off();
				InvenBtComsumAniScript->On();
				InvenBtEtcAniScript->Off();
				InvenBtInstallAniScript->Off();
				InvenBtCashAniScript->Off();

				// 슬롯
				InventorySlotOff(m_InvenItemEquip);
				InventorySlotOn(m_InvenItemConsum);
				InventorySlotOff(m_InvenItemEtc);
				InventorySlotOff(m_InvenItemInstall);
				InventorySlotOff(m_InvenItemCash);
			}
		}
		else if (_Other == InvenBtEtcCol)
		{
			// 기타
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				// 버튼 누르기
				InvenBtEquipAniScript->Off();
				InvenBtComsumAniScript->Off();
				InvenBtEtcAniScript->On();
				InvenBtInstallAniScript->Off();
				InvenBtCashAniScript->Off();

				// 슬롯
				InventorySlotOff(m_InvenItemEquip);
				InventorySlotOff(m_InvenItemConsum);
				InventorySlotOn(m_InvenItemEtc);
				InventorySlotOff(m_InvenItemInstall);
				InventorySlotOff(m_InvenItemCash);
			}
		}
		else if (_Other == InvenBtInstallCol)
		{
			// 설치
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				// 버튼 누르기
				InvenBtEquipAniScript->Off();
				InvenBtComsumAniScript->Off();
				InvenBtEtcAniScript->Off();
				InvenBtInstallAniScript->On();
				InvenBtCashAniScript->Off();

				// 슬롯
				InventorySlotOff(m_InvenItemEquip);
				InventorySlotOff(m_InvenItemConsum);
				InventorySlotOff(m_InvenItemEtc);
				InventorySlotOn(m_InvenItemInstall);
				InventorySlotOff(m_InvenItemCash);
			}
		}
		else if (_Other == InvenBtCashCol)
		{
			// 캐시
			if (true == Input::Down(L"MouseLeftButton"))
			{
				SoundPlay_Effect(L"btClick.mp3");
				// 버튼 누르기
				InvenBtEquipAniScript->Off();
				InvenBtComsumAniScript->Off();
				InvenBtEtcAniScript->Off();
				InvenBtInstallAniScript->Off();
				InvenBtCashAniScript->On();

				// 슬롯
				InventorySlotOff(m_InvenItemEquip);
				InventorySlotOff(m_InvenItemConsum);
				InventorySlotOff(m_InvenItemEtc);
				InventorySlotOff(m_InvenItemInstall);
				InventorySlotOn(m_InvenItemCash);
			}
		}


		// 클릭한 아이템 정보 - 인벤
		for (size_t i = 0; i < InvenItemConSumCol.size(); ++i)
		{
			// 소비
			if (_Other == InvenItemConSumCol[i])
			{
				if (true == Input::Down(L"MouseLeftButton"))
				{
					SoundPlay_Effect(L"btClick.mp3");
					if (nullptr == m_ClickItemInfo)
					{
						m_ClickItemInfo = m_InvenItemConsum[i];
						m_InvenClickMouseImage->GetComponent<Inven_MouseItem>()->SetClickItem(m_ClickItemInfo->GetComponent<Inven_Info>()->GetItemCode());

						if (L"EmptyItem.png" == m_ClickItemInfo->GetComponent<Inven_Info>()->GetInfo()->ItemName)
						{
							m_ClickItemInfo = nullptr;
							return;
						}

					}
					else
					{
						// 이미지 체인지하기 (아이템 스왑)
						ItemCode::ItemCodeName TempItem = m_InvenItemConsum[i]->GetComponent<Inven_Info>()->GetItemCode();
						ItemCode::ItemCodeName SelectItem = m_ClickItemInfo->GetComponent<Inven_Info>()->GetItemCode();

						// 개수 알기
						int TempItemCount = m_InvenItemConsum[i]->GetComponent<Inven_Info>()->GetItemCount();
						int SelectCount = m_ClickItemInfo->GetComponent<Inven_Info>()->GetItemCount();

						// 아이템 개수 변경 ( 추후 같은 이미지라면 합치기 )
						// 같은이미지의 경우와 다른 이미지의 경우로 나누어서 생각한다.
						std::wstring TempItemName = m_InvenItemConsum[i]->GetComponent<Inven_Info>()->GetInfo()->ItemName;
						std::wstring SelectItemName = m_ClickItemInfo->GetComponent<Inven_Info>()->GetInfo()->ItemName;

						if (TempItemName == SelectItemName)
						{
							// 동일한 이미지라면 -> 개수가 999개를 초과하는지 검사해야 한다.
							// 초과시 남은 여분의 아이템들은 기존의 아이템칸에 있는다.

							int TotalCount = TempItemCount + SelectCount;

							if (TotalCount > 999)
							{
								// 999개 초과
								m_InvenItemConsum[i]->GetComponent<Inven_Info>()->SetItemCount(999);
								TotalCount -= 999;
								m_ClickItemInfo->GetComponent<Inven_Info>()->SetItemCount(TotalCount);
							}
							else
							{
								// 999개 이하
								m_InvenItemConsum[i]->GetComponent<Inven_Info>()->SetItemCount(TotalCount);
								m_ClickItemInfo->GetComponent<Inven_Info>()->SetItemCount(0);

								// 아이템 이미지 변경
								m_ClickItemInfo->GetComponent<Inven_Info>()->ItemAdd(ItemCode::ItemCodeName::EMPTY);

							}

						}
						else
						{
							m_InvenItemConsum[i]->GetComponent<Inven_Info>()->SetItemCount(SelectCount);
							m_ClickItemInfo->GetComponent<Inven_Info>()->SetItemCount(TempItemCount);

							// 아이템 이미지 변경
							m_InvenItemConsum[i]->GetComponent<Inven_Info>()->ItemAdd(SelectItem);
							m_ClickItemInfo->GetComponent<Inven_Info>()->ItemAdd(TempItem);
						}

						// 빈 이미지인지 확인 -> 숫자 제거
						std::wstring InvenSlotItemName = m_ClickItemInfo->GetComponent<Inven_Info>()->GetInfo()->ItemName;
						if (L"EmptyItem.png" == InvenSlotItemName)
						{
							m_ClickItemInfo->GetComponent<Inven_Info>()->SetEmptyTexture();
						}

						// 마우스 따라다니는 이미지 지우기
						m_InvenClickMouseImage->GetComponent<Inven_MouseItem>()->SetClickItem(ItemCode::ItemCodeName::EMPTY);
						m_ClickItemInfo = nullptr;

					}

				}
			}
		}

		// 클릭한거.. 퀵슬롯 관련
		for (size_t i = 0; i < m_QuickSlotCol.size(); ++i)
		{
			if (_Other == m_QuickSlotCol[i])
			{
				// 클릭한게 인벤에서 클릭한건지 퀵슬롯에서 클릭한건지 검사
				// 선 인벤 후 퀵슬롯 -> 인벤토리의 소비템정보를 퀵슬롯에 넣어주기..
				// 선 퀵슬롯 후 퀵슬롯 -> 퀵슬롯의 정보를 서로 스왑
				if (true == Input::Down(L"MouseLeftButton"))
				{
					SoundPlay_Effect(L"btClick.mp3");
					if (nullptr == m_ClickItemInfo)
					{
						// 선 퀵슬롯 -> 퀵슬롯의 정보를 알려준다. 인벤에서 만약 이 정보가 퀵슬롯일 경우 인벤에 넣을떄 불가능하게한다.
						m_ClickItemInfo = m_QuickSlotVector[i];			// 퀵슬롯의 정보
						m_InvenClickMouseImage->GetComponent<Inven_MouseItem>()->SetClickItem(m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName());

						if (ItemCode::EMPTY == m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCodeName())
						{
							m_ClickItemInfo = nullptr;
							return;
						}

					}
					else
					{
						// 후 퀵슬롯 or 인벤 -> 선클릭한게 퀵슬롯인지 인벤인지 확인

						// 인벤
						for (size_t k = 0; k < m_InvenItemConsum.size(); ++k)
						{
							GameObject* FirstClickObj = m_InvenItemConsum[k];
							GameObject* MouseClickInfo = m_ClickItemInfo;

							// 선 인벤 후 퀵슬롯
							if (MouseClickInfo == FirstClickObj)
							{
								// 만약 클릭한 퀵슬롯자리가 스킬이라면 그냥 패스
								ItemCode::ItemCodeName CurQuickSlotClick = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName();

								// 스킬이라면
								if (CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_BE
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_BZ
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_CC
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_PM
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_SP)
								{
									m_InvenClickMouseImage->GetComponent<Inven_MouseItem>()->SetClickItem(ItemCode::ItemCodeName::EMPTY);
									m_ClickItemInfo = nullptr;
									return;
								}
								else
								{
									// 스킬이 아니라면 물약자리인지 빈칸인지.. 2종류임 (퀵슬롯 자리) 일단 인벤->퀵슬롯이니까 100% 소비템임
									// but 필요없음! 그냥 바꿔주기
									ItemCode::ItemCodeName SelectItemCode = FirstClickObj->GetComponent<Inven_Info>()->GetItemCode();		// 코드
									std::wstring SelectName = FirstClickObj->GetComponent<Inven_Info>()->GetInfo()->ItemName;


									m_QuickSlotCol[i]->GetComponent<QuickSlotScript>()->SetConsumeCodeName(SelectItemCode);
									m_QuickSlotCol[i]->GetComponent<QuickSlotScript>()->SetInventoryItem(m_InvenItemConsum[k]);		// 안에서 카운트까지해줌
									m_QuickSlotCol[i]->GetComponent<QuickSlotScript>()->SetCoolTime(0.0f);
									m_QuickSlotCol[i]->GetComponent<QuickSlotScript>()->SetSkillManager(nullptr);
									m_QuickSlotCol[i]->GetComponent<QuickSlotScript>()->SetSkillName(L"");

									// 바꾼후 마우스 이미지 원래대로..
									m_InvenClickMouseImage->GetComponent<Inven_MouseItem>()->SetClickItem(ItemCode::ItemCodeName::EMPTY);
									m_ClickItemInfo = nullptr;

									return;
								}
							}
						}

						// 퀵슬롯
						for (size_t q = 0; q < m_QuickSlotVector.size(); ++q)
						{
							GameObject* FirstClickObj = m_QuickSlotVector[q];
							GameObject* MouseClickInfo = m_ClickItemInfo;

							// 선 퀵 후 퀵
							if (MouseClickInfo == FirstClickObj)
							{
								// 퀵슬롯에서 자리바꾸는건 스킬, 물약 제약 받지 않고 이동한다.
								ItemCode::ItemCodeName CurQuickSlotClick = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName();

								// 스킬이라면
								if (CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_BE
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_BZ
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_CC
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_PM
									|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_SP)
								{
									// 선 스킬 후 스킬
									// 선 스킬 후 물약 일경우 따진다
									ItemCode::ItemCodeName LastItemCode = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCodeName();

									if (CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_BE
										|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_BZ
										|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_CC
										|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_PM
										|| CurQuickSlotClick == ItemCode::ItemCodeName::SKILL_SP)
									{
										// 스킬은 쿨타임, 스킬이름 변경, 코드 변경
										// 후클릭 이미지 임시로 저장
										float TempCoolTime = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCoolTime();
										std::wstring TempSkillName = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetSkillName();
										ItemCode::ItemCodeName TempCode = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName();

										// m_QuickSlotVector[i]는 후 클릭
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillName(m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetSkillName());
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetCoolTime(m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCoolTime());
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetConsumeCodeName(m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCodeName());

										// 선클릭한것도 바꿔주기
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillName(TempSkillName);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetCoolTime(TempCoolTime);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetConsumeCodeName(TempCode);
									}
									else
									{
										// 후 물약
										float TempCoolTime = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCoolTime();
										std::wstring TempSkillName = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetSkillName();
										ItemCode::ItemCodeName TempCode = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName();

										ItemCode::ItemCodeName TempFirstCode = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCodeName();
										GameObject* TempFirstInventoryItemObj = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetInventoryItem();

										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetCoolTime(0.0f);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetConsumeCodeName(TempFirstCode);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetInventoryItem(TempFirstInventoryItemObj);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillManager(nullptr);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillName(L"");

										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetCoolTime(TempCoolTime);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillName(TempSkillName);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetInventoryItem(nullptr);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillManager(m_SkillManager);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillName(TempSkillName);

									}

									// 바꾼후 마우스 이미지 원래대로..
									m_InvenClickMouseImage->GetComponent<Inven_MouseItem>()->SetClickItem(ItemCode::ItemCodeName::EMPTY);
									m_ClickItemInfo = nullptr;
								}
								else
								{
									// 선클릭이 물약일 경우
									// 후 클릭이 물약인지, 스킬인지 체크해야한다
									ItemCode::ItemCodeName LastClickItemCode = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCodeName();

									if (ItemCode::ItemCodeName::SKILL_BE == LastClickItemCode
										|| ItemCode::ItemCodeName::SKILL_BZ == LastClickItemCode
										|| ItemCode::ItemCodeName::SKILL_CC == LastClickItemCode
										|| ItemCode::ItemCodeName::SKILL_PM == LastClickItemCode
										|| ItemCode::ItemCodeName::SKILL_SP == LastClickItemCode)
									{
										// 후클릭이 스킬일 경우  선물 후스
										ItemCode::ItemCodeName TempFirstCode = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName();
										GameObject* TempFirstInventoryItemObj = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetInventoryItem();

										float TempLastCoolTime = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCoolTime();
										std::wstring TempLastSkillName = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetSkillName();
										ItemCode::ItemCodeName TempLastCode = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCodeName();

										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetCoolTime(TempLastCoolTime);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetConsumeCodeName(TempLastCode);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetInventoryItem(nullptr);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillManager(m_SkillManager);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillName(TempLastSkillName);

										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetCoolTime(0.0f);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetConsumeCodeName(TempFirstCode);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetInventoryItem(TempFirstInventoryItemObj);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillManager(nullptr);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillName(L"");


									}
									else
									{
										// 후클릭이 물약일 경우 or 빈칸
										ItemCode::ItemCodeName TempFirstCode = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetCodeName();
										GameObject* TempFirstInventoryItemObj = m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->GetInventoryItem();

										ItemCode::ItemCodeName TempLastCode = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetCodeName();
										GameObject* TempLastInventoryItemObj = m_ClickItemInfo->GetComponent<QuickSlotScript>()->GetInventoryItem();

										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetCoolTime(0.0f);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetConsumeCodeName(TempLastCode);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetInventoryItem(TempLastInventoryItemObj);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillManager(nullptr);
										m_QuickSlotVector[i]->GetComponent<QuickSlotScript>()->SetSkillName(L"");

										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetCoolTime(0.0f);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetConsumeCodeName(TempFirstCode);

										if (ItemCode::ItemCodeName::EMPTY != TempFirstCode)
										{
											m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetInventoryItem(TempFirstInventoryItemObj);
										}
										else
										{
											m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetInventoryItem(nullptr);
										}
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillManager(nullptr);
										m_ClickItemInfo->GetComponent<QuickSlotScript>()->SetSkillName(L"");
									}

									// 바꾼후 마우스 이미지 원래대로..
									m_InvenClickMouseImage->GetComponent<Inven_MouseItem>()->SetClickItem(ItemCode::ItemCodeName::EMPTY);
									m_ClickItemInfo = nullptr;

								}
							}
						}
					}
				}
			}
		}

	}
}


void MonsterStageScene1::Col_InventoryExit(Collision* _This, Collision* _Other)
{
	if (_This == CursorCol)
	{
		// 인벤토리
		if (_Other == InvenMoveCol)
		{
		}

		if (_Other == InvenBtCloseCol)
		{
			// 닫기
			InvenBtCloseAniScript->SetAnimation(L"Normal");

		}
		else if (_Other == InvenBtDisassembleCol)
		{
			// 톱니바퀴
			InvenBtDisassembleAniScript->SetAnimation(L"Normal");

		}
		else if (_Other == InvenBtExtractCol)
		{
			// 물약버튼
			InvenBtExtractAniScript->SetAnimation(L"Normal");
		}
		else if (_Other == InvenBtAppraiseCol)
		{
			// 돋보기
			InvenBtAppraiseAniScript->SetAnimation(L"Normal");
		}
		else if (_Other == InvenBtToadCol)
		{
			// 망치 화살표
			InvenBtToadAniScript->SetAnimation(L"Normal");
		}
		else if (_Other == InvenBtUpgradeCol)
		{
			// 강화
			InvenBtUpgradeAniScript->SetAnimation(L"Normal");

		}
		else if (_Other == InvenBtEquipCol)
		{
			// 장비

		}
		else if (_Other == InvenBtComsumCol)
		{
			// 소비

		}
		else if (_Other == InvenBtEtcCol)
		{
			// 기타

		}
		else if (_Other == InvenBtInstallCol)
		{
			// 설치

		}
		else if (_Other == InvenBtCashCol)
		{
			// 캐시
		}


	}
}

// 몬스터 충돌함수
void MonsterStageScene1::Col_MonsterEnter(Collision* _This, Collision* _Other)
{
	float BEzValue = 20.0f;
	// 아이 마네킹
	for (int i = 0; i < m_ChildMannequinMonsterCol.size(); ++i)
	{
		if (_This == m_ChildMannequinMonsterCol[i])
		{
			std::wstring SkillName = _Other->GetGroupName();

			if (L"Skill_BlizzardCol" == SkillName)
			{
				// 랜덤한 데미지 ( 150 ~ 300 );
				int NewDamage = (rand() % 150) + 2267;

				// HP감소
				m_ChildMannequinMonsterVector[i]->GetComponent<ChildManneqin>()->Damage(NewDamage);

				// 데미지 -> 스크립트랑 연결해준다.
				m_ChildMannequinMonsterVector[i]->GetComponent<MonsterDamageSkin>()->Create_DamageSkin(NewDamage);

				// 이펙트
				My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
				NewHitEffect->GetTransform()->Local_Position(m_ChildMannequinMonsterCol[i]->GetTransform()->Local_Position());
				NewHitEffect->GetTransform()->Local_Scale(Vector4(103.0f, 126.0f, 1.0f, 1.0f ));
				My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF3);
				NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
				My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
				My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
				NewHitEffectScript->SetMovePos(Vector4::Zero);
				NewHitEffectScript->SetSkillName(L"HE_BZ");
			}
			else if (L"Skill_BlazingExtinction" == SkillName)
			{
				// 랜덤한 데미지 ( 150 ~ 300 );
				int NewDamage = (rand() % 257) + 1324;

				// HP감소
				m_ChildMannequinMonsterVector[i]->GetComponent<ChildManneqin>()->Damage(NewDamage);

				// 데미지 -> 스크립트랑 연결해준다.
				m_ChildMannequinMonsterVector[i]->GetComponent<MonsterDamageSkin>()->Create_DamageSkin(NewDamage);

				// 이펙트
				My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
				NewHitEffect->GetTransform()->Local_Position(m_ChildMannequinMonsterCol[i]->GetTransform()->Local_Position());
				NewHitEffect->GetTransform()->Local_Scale(Vector4(405.0f, 308.0f, BEzValue, 1.0f));
				My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF3);
				NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
				My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
				My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
				NewHitEffectScript->SetMovePos(Vector4::Zero);
				NewHitEffectScript->SetSkillName(L"HE_BE");

				--BEzValue;
			}
			else if (L"Skill_Cataclysm" == SkillName)
			{
				// 랜덤한 데미지 ( 150 ~ 300 );
				int NewDamage = (rand() % 999) + 22179;

				// HP감소
				m_ChildMannequinMonsterVector[i]->GetComponent<ChildManneqin>()->Damage(NewDamage);

				// 데미지 -> 스크립트랑 연결해준다.
				m_ChildMannequinMonsterVector[i]->GetComponent<MonsterDamageSkin>()->Create_DamageSkin(NewDamage);

				// 이펙트
				My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
				NewHitEffect->GetTransform()->Local_Position(m_ChildMannequinMonsterCol[i]->GetTransform()->Local_Position());
				NewHitEffect->GetTransform()->Local_Scale(Vector4(260.0f, 242.0f, 1.0f, 1.0f));
				My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF3);
				NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
				My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
				My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
				NewHitEffectScript->SetMovePos(Vector4::Zero);
				NewHitEffectScript->SetSkillName(L"HE_CC");
			}
			else if (L"Skill_PoisonMist" == SkillName)
			{
				// 랜덤한 데미지 ( 150 ~ 300 );
				int NewDamage = (rand() % 60) + 50;

				// HP감소
				m_ChildMannequinMonsterVector[i]->GetComponent<ChildManneqin>()->Damage(NewDamage);

				// 데미지 -> 스크립트랑 연결해준다.
				m_ChildMannequinMonsterVector[i]->GetComponent<MonsterDamageSkin>()->Create_DamageSkin(NewDamage);

				// 이펙트
				My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
				NewHitEffect->GetTransform()->Local_Position(m_ChildMannequinMonsterCol[i]->GetTransform()->Local_Position());
				NewHitEffect->GetTransform()->Local_Scale(Vector4(260.0f, 242.0f, 1.0f, 1.0f));
				My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF3);
				NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
				My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
				My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
				NewHitEffectScript->SetMovePos(Vector4::Zero);
				NewHitEffectScript->SetSkillName(L"HE_PM");
			}
			else if (L"Skill_SoulPenetration" == SkillName)
			{
				// 랜덤한 데미지 ( 150 ~ 300 );
				int NewDamage = (rand() % 150) + 1267;

				// HP감소
				m_ChildMannequinMonsterVector[i]->GetComponent<ChildManneqin>()->Damage(NewDamage);

				// 데미지 -> 스크립트랑 연결해준다.
				m_ChildMannequinMonsterVector[i]->GetComponent<MonsterDamageSkin>()->Create_DamageSkin(NewDamage);

				// 이펙트
				My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
				NewHitEffect->GetTransform()->Local_Position(m_ChildMannequinMonsterCol[i]->GetTransform()->Local_Position());
				NewHitEffect->GetTransform()->Local_Scale(Vector4(242.0f, 242.0f, 1.0f, 1.0f));
				My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF3);
				NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
				My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
				My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
				NewHitEffectScript->SetMovePos(Vector4::Zero);
				NewHitEffectScript->SetSkillName(L"HE_SP");
			}

			// 돈 떨어트리기
			if (0 == m_WhiteRabbitMonsterVector[i]->GetComponent<WhiteSandRabbit>()->GetHP())
			{
				Vector4 MoneyPos = m_WhiteRabbitMonsterVector[i]->GetTransform()->Local_Position();
				MoneyPos.z -= 1.0f;
				My_PTR<GameObject> MoneyItem = GetScene()->GameObject_Create(L"Money");
				MoneyItem->GetTransform()->Local_Position(MoneyPos);
				MoneyItem->GetTransform()->Local_Scale(Vector4(25.0f, 24.0f, 1.0f, 1.0f));
				My_PTR<SpriteRenderer2D> MoneyRenderer = MoneyItem->Component_Add<SpriteRenderer2D>(RENDER_ORDER::MONSTER);
				MoneyRenderer->SetSprite(L"Item_Money01.png", 0);
				My_PTR<Animation2D> MoneyAnim = MoneyItem->Component_Add<Animation2D>();
				My_PTR<RectCol2D> MoneyCol = MoneyItem->Component_Add<RectCol2D>(L"Money");
				My_PTR<ItemMoveScript> MoneyMoveScript = MoneyRenderer->Component_Add<ItemMoveScript>();
				MoneyMoveScript->SetItemState(1);
				MoneyMoveScript->SetPlayer(m_GamePlayer);
				MoneyMoveScript->SetFloor(m_MoveTile);

				m_ItemObjVector.push_back(MoneyItem);
				m_ItemColVector.push_back(MoneyCol);
			}
		}
	}

	// 흰모래토끼
	BEzValue = 20.0f;
	for (int i = 0; i < m_WhiteSandRabbitMonsterCol.size(); ++i)
	{
		if (_This == m_WhiteSandRabbitMonsterCol[i])
		{
			std::wstring SkillName = _Other->GetGroupName();

			if (L"Skill_BlizzardCol" == SkillName)
			{
				SoundPlay_Effect(L"Use_Blizzard.mp3");
				// 랜덤한 데미지 ( 150 ~ 300 );
				int NewDamage = (rand() % 150) + 5267;

				// HP감소
				m_WhiteRabbitMonsterVector[i]->GetComponent<WhiteSandRabbit>()->Damage(NewDamage);
				
				// 데미지 -> 스크립트랑 연결해준다.
				m_WhiteRabbitMonsterVector[i]->GetComponent<MonsterDamageSkin>()->Create_DamageSkin(NewDamage);

				// 이펙트
				My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
				NewHitEffect->GetTransform()->Local_Position(m_WhiteSandRabbitMonsterCol[i]->GetTransform()->Local_Position());
				NewHitEffect->GetTransform()->Local_Scale(Vector4(103.0f, 126.0f, 1.0f, 1.0f));
				My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF3);
				NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
				My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
				My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
				NewHitEffectScript->SetMovePos(Vector4::Zero);
				NewHitEffectScript->SetSkillName(L"HE_BZ");
				--BEzValue;
			}
			else if (L"Skill_BlazingExtinction" == SkillName)
			{
				SoundPlay_Effect(L"Use_BlazingExtinction.mp3");

				// 데미지 -> 스크립트랑 연결해준다.
				// 랜덤한 데미지
				int NewDamage = (rand() % 257) + 6324;
				// HP감소
				m_WhiteRabbitMonsterVector[i]->GetComponent<WhiteSandRabbit>()->Damage(NewDamage);
				m_WhiteRabbitMonsterVector[i]->GetComponent<MonsterDamageSkin>()->Create_DamageSkin(NewDamage);

				// 이펙트
				My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
				NewHitEffect->GetTransform()->Local_Position(m_WhiteSandRabbitMonsterCol[i]->GetTransform()->Local_Position());
				NewHitEffect->GetTransform()->Local_Scale(Vector4(405.0f, 308.0f, BEzValue, 1.0f));
				My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF3);
				NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
				My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
				My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
				NewHitEffectScript->SetMovePos(Vector4(0.0f, 50.0f, 0.0f, 0.0f));
				NewHitEffectScript->SetSkillName(L"HE_BE");
				--BEzValue;
			}
			else if (L"Skill_Cataclysm" == SkillName)
			{
				SoundPlay_Effect(L"Use_CC2.mp3");
				// 데미지 -> 스크립트랑 연결해준다.
				// 랜덤한 데미지
				int NewDamage = (rand() % 999) +82179;
				// HP감소
				m_WhiteRabbitMonsterVector[i]->GetComponent<WhiteSandRabbit>()->Damage(NewDamage);
				m_WhiteRabbitMonsterVector[i]->GetComponent<MonsterDamageSkin>()->Create_DamageSkin(NewDamage);

				// 이펙트
				My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
				NewHitEffect->GetTransform()->Local_Position(m_WhiteSandRabbitMonsterCol[i]->GetTransform()->Local_Position());
				NewHitEffect->GetTransform()->Local_Scale(Vector4(260.0f, 242.0f, 1.0f, 1.0f));
				My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF12);
				NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
				My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
				My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
				NewHitEffectScript->SetMovePos(Vector4(0.0f, 20.0f, BEzValue, 0.0f));
				NewHitEffectScript->SetSkillName(L"HE_CC");
				--BEzValue;
			}
			else if (L"Skill_PoisonMist" == SkillName)
			{
				// 데미지 -> 스크립트랑 연결해준다.
				// 랜덤한 데미지
				int NewDamage = (rand() % 200) + 2957;
				// HP감소
				m_WhiteRabbitMonsterVector[i]->GetComponent<WhiteSandRabbit>()->Damage(NewDamage);
				m_WhiteRabbitMonsterVector[i]->GetComponent<MonsterDamageSkin>()->Create_DamageSkin(NewDamage);

				// 이펙트
				My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
				NewHitEffect->GetTransform()->Local_Position(m_WhiteSandRabbitMonsterCol[i]->GetTransform()->Local_Position());
				NewHitEffect->GetTransform()->Local_Scale(Vector4(260.0f, 242.0f, 1.0f, 1.0f));
				My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF3);
				NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
				My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
				My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
				NewHitEffectScript->SetMovePos(Vector4::Zero);
				NewHitEffectScript->SetSkillName(L"HE_PM");
				--BEzValue;
			}
			else if (L"Skill_SoulPenetration" == SkillName)
			{
				SoundPlay_Effect(L"Use_03.mp3");
				// 데미지 -> 스크립트랑 연결해준다.
				// 랜덤한 데미지
				int NewDamage = (rand() % 200) + 5957;
				// HP감소
				m_WhiteRabbitMonsterVector[i]->GetComponent<WhiteSandRabbit>()->Damage(NewDamage);
				m_WhiteRabbitMonsterVector[i]->GetComponent<MonsterDamageSkin>()->Create_DamageSkin(NewDamage);

				// 이펙트
				My_PTR<GameObject> NewHitEffect = GetScene()->GameObject_Create(L"HitEffect");
				NewHitEffect->GetTransform()->Local_Position(m_WhiteSandRabbitMonsterCol[i]->GetTransform()->Local_Position());
				NewHitEffect->GetTransform()->Local_Scale(Vector4(242.0f, 242.0f, 1.0f, 1.0f));
				My_PTR<SpriteRenderer2D> NewHitEffectRenderer = NewHitEffect->Component_Add<SpriteRenderer2D>(RenderOrder::SKILL_EFF12);
				NewHitEffectRenderer->SetSprite(L"EmptyItem.png", 0);
				My_PTR<Animation2D> NewHitEffectAnimation = NewHitEffect->Component_Add<Animation2D>();
				My_PTR<MonsterHitScript> NewHitEffectScript = NewHitEffect->Component_Add<MonsterHitScript>();
				NewHitEffectScript->SetMovePos(Vector4(0.0f, 10.0f, BEzValue, 0.0f));
				NewHitEffectScript->SetSkillName(L"HE_SP");
				--BEzValue;
			}

			// 돈 떨어트리기
			if (0 == m_WhiteRabbitMonsterVector[i]->GetComponent<WhiteSandRabbit>()->GetHP())
			{
				Vector4 MoneyPos = m_WhiteRabbitMonsterVector[i]->GetTransform()->Local_Position();
				MoneyPos.z -= 1.0f;
				My_PTR<GameObject> MoneyItem = GetScene()->GameObject_Create(L"Money");
				MoneyItem->GetTransform()->Local_Position(MoneyPos);
				MoneyItem->GetTransform()->Local_Scale(Vector4(25.0f, 24.0f, 1.0f, 1.0f));
				My_PTR<SpriteRenderer2D> MoneyRenderer = MoneyItem->Component_Add<SpriteRenderer2D>(RENDER_ORDER::MONSTER);
				MoneyRenderer->SetSprite(L"Item_Money01.png", 0);
				My_PTR<Animation2D> MoneyAnim = MoneyItem->Component_Add<Animation2D>();
				My_PTR<RectCol2D> MoneyCol = MoneyItem->Component_Add<RectCol2D>(L"Money");
				My_PTR<ItemMoveScript> MoneyMoveScript = MoneyRenderer->Component_Add<ItemMoveScript>();
				MoneyMoveScript->SetItemState(1);
				MoneyMoveScript->SetPlayer(m_GamePlayer);
				MoneyMoveScript->SetFloor(m_MoveTile);

				m_ItemObjVector.push_back(MoneyItem);
				m_ItemColVector.push_back(MoneyCol);
			}
		}
	}

}

void MonsterStageScene1::Col_MonsterStay(Collision* _This, Collision* _Other)
{

}

void MonsterStageScene1::Col_MonsterExit(Collision* _This, Collision* _Other)
{

}

// 플레이어 충돌
void MonsterStageScene1::Col_PlayerEnter(Collision* _This, Collision* _Other)
{

}

void MonsterStageScene1::Col_PlayerStay(Collision* _This, Collision* _Other)
{
	// 포탈 관련
	if (m_PlayerCol == _This)
	{
		if (m_PrePortalCol == _Other)
		{
			if (true == Input::Down(L"Key_Space"))
			{
				// 씬 옮기기 - 에스페라
				// 정보들 옮겨주기(내일추가)
				// 정보들을 옮겨준 후에 씬을 이동시킨다
				SoundPlay_Effect(L"Portal.mp3");
				// 인벤토리 정보
				MapleGlobalValue::SetInvenItemConsume(m_InvenItemConsum);
				MapleGlobalValue::SetQuickSlotVector(m_QuickSlotVector);
				Vector4 ChangePos = m_GamePlayer->GetTransform()->Local_Position();
				ChangePos.x = -691.0f;
				ChangePos.y = -210.0f;

				MapleGlobalValue::PlayerInfo(ChangePos, 0, 0, 0);
				MapleGlobalValue::SetCameraMovePos(Vector4(-300.0f, 0.0f, 0.0f, 0.0f));
				m_BGMPlayer->Stop_BGM();
				Scene::Scene_Change(L"PinkbinScene");

			}
		}
		else if (m_NextPortalCol == _Other)
		{
			if (true == Input::Down(L"Key_Space"))
			{
				// 씬 옮기기 - 에스페라
				// 정보들 옮겨주기(내일추가)
				// 정보들을 옮겨준 후에 씬을 이동시킨다
				SoundPlay_Effect(L"Portal.mp3");
				// 인벤토리 정보
				MapleGlobalValue::SetInvenItemConsume(m_InvenItemConsum);
				MapleGlobalValue::SetQuickSlotVector(m_QuickSlotVector);
				Vector4 ChangePos = m_GamePlayer->GetTransform()->Local_Position();


				MapleGlobalValue::PlayerInfo(ChangePos, 0, 0, 0);
				MapleGlobalValue::SetCameraMovePos(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
				Scene::Scene_Change(L"ArcanaTownScene");
			}
		}
	}

	// 돈관련
	for (size_t i = 0; i < m_ItemColVector.size(); ++i)
	{
		if (_Other == m_ItemColVector[i])
		{
			if (true == Input::Down(L"Key_Space"))
			{
				// 돈 먹기
				int randMoney = (rand() % 300) + 10000;
				m_InvenMoenyManagerScript->AddMeso(randMoney);
				m_ItemObjVector[i]->GetComponent<ItemMoveScript>()->SetDrop(true);
				m_ItemObjVector.erase(m_ItemObjVector.begin() + i);
				m_ItemColVector.erase(m_ItemColVector.begin() + i);

				SoundPlay_Effect(L"PickUpItem.mp3");
				
			}
		}
	}
}

void MonsterStageScene1::Col_PlayerExit(Collision* _This, Collision* _Other)
{

}

//  충돌체 링크
void MonsterStageScene1::CollisionNameLink()
{
	GetScene()->m_CollisionManager.Link(L"Player", L"Monster");
	GetScene()->m_CollisionManager.Link(L"Player", L"Money");

	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InventoryMoveObjCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtCloseCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtDisassembleCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtExtractCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtAppraiseCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtToadCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtUpgradeCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtEquipCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtComsumCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtEtcCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtInstallCol");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InvenBtCashCol");

	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_InventorySlot_Consume");
	GetScene()->m_CollisionManager.Link(L"Chu_CursorCol", L"Chu_QuickSlotCol");

	// 포탈  - Player
	GetScene()->m_CollisionManager.Link(L"Player", L"Town_PrePortalCol");
	GetScene()->m_CollisionManager.Link(L"Player", L"Town_NextPortalCol");

	// 몬스터랑 스킬
	GetScene()->m_CollisionManager.Link(L"ChildMannequinCol", L"Skill_BlizzardCol");
	GetScene()->m_CollisionManager.Link(L"ChildMannequinCol", L"Skill_BlazingExtinction");
	GetScene()->m_CollisionManager.Link(L"ChildMannequinCol", L"Skill_Cataclysm");
	GetScene()->m_CollisionManager.Link(L"ChildMannequinCol", L"Skill_PoisonMist");
	GetScene()->m_CollisionManager.Link(L"ChildMannequinCol", L"Skill_SoulPenetration");

	GetScene()->m_CollisionManager.Link(L"WhiteSandRabbitCol", L"Skill_BlizzardCol");
	GetScene()->m_CollisionManager.Link(L"WhiteSandRabbitCol", L"Skill_BlazingExtinction");
	GetScene()->m_CollisionManager.Link(L"WhiteSandRabbitCol", L"Skill_Cataclysm");
	GetScene()->m_CollisionManager.Link(L"WhiteSandRabbitCol", L"Skill_PoisonMist");
	GetScene()->m_CollisionManager.Link(L"WhiteSandRabbitCol", L"Skill_SoulPenetration");

	// 포탈  - Player
	GetScene()->m_CollisionManager.Link(L"Player", L"ChewChewIsland_PrePortalCol");
	GetScene()->m_CollisionManager.Link(L"Player", L"ChewChewIsland_NextPortalCol");
}

void MonsterStageScene1::SoundPlay_BGM(std::wstring _SoundFileName)
{
	m_BGMPlayer->Play_BGM(_SoundFileName.c_str());
}

void MonsterStageScene1::SoundPlay_Effect(std::wstring _SoundFileName)
{
	m_EffectSoundPlayer->Play(_SoundFileName.c_str());
}

void MonsterStageScene1::SoundPlaye_Voice(std::wstring _SoundFileName)
{
	m_VoiceSoundPlayer->Play(_SoundFileName.c_str());
}

//
void MonsterStageScene1::BGMSoundLoading()
{
	// 사운드플레이
	m_BGMEmptyObject = GetScene()->GameObject_Create(L"BGMPlayer");
	m_BGMEmptyObject->GetTransform()->Local_Position(Vector4::Zero);
	m_BGMEmptyObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_BGMPlayer = m_BGMEmptyObject->Component_Add<SoundPlayer>();
}

void MonsterStageScene1::EffectSoundLoading()
{
	m_EffectSoundObject = GetScene()->GameObject_Create(L"EffectSoundPlayer");
	m_EffectSoundObject->GetTransform()->Local_Position(Vector4::Zero);
	m_EffectSoundObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_EffectSoundPlayer = m_EffectSoundObject->Component_Add<SoundPlayer>();
}

void MonsterStageScene1::VoiceSoundLoading()
{
	m_VoiceSoundObject = GetScene()->GameObject_Create(L"EffectSoundPlayer");
	m_VoiceSoundObject->GetTransform()->Local_Position(Vector4::Zero);
	m_VoiceSoundObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_VoiceSoundPlayer = m_EffectSoundObject->Component_Add<SoundPlayer>();
}