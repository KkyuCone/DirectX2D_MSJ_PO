#include "TestGameScene.h"

#include <GameObject.h>
#include <DXHeader.h>
#include <Camera.h>
#include <Animation2D.h>
#include <SpriteRenderer2D.h>

// 내꺼
#include <PlayerMove.h>
#include <PlayerAnimation.h>
#include <SkillManager.h>
#include <UIMove.h>

// 몬스터
#include <ChildManneqin.h>
#include <WhiteSandRabbit.h>
#include <TransformSlime.h>
#include <Velaris.h>
#include <PinkbinRegen.h>

// 배경
#include <BG_TownScript.h>

//
#include <Texture.h>
#include <Sprite.h>
#include <RectCol2D.h>
#include <GameDebug.h>
#include <InGameCamera.h>

#include <SoundPlayer.h>


TestGameScene::TestGameScene() 
	: m_GamePlayer(nullptr), m_Monster(nullptr)
	, m_Background(nullptr), m_MoveTile(nullptr)
	, m_MainCamera(nullptr), m_CamScript(nullptr), m_CameraCom(nullptr)
	, m_StatusBarBackground(nullptr), m_StatusBarBase(nullptr), m_StatusQuickSlot(nullptr)
	, m_StatusBarGauge(nullptr), m_GaugeGraduation(nullptr)
	, m_HPGray(nullptr), m_MPGray(nullptr), m_ExpGray(nullptr)
{
}


TestGameScene::~TestGameScene()
{
}


// Start~Update
bool TestGameScene::Loading()
{

	BGLoading();
	UILoading();

	// 카메라 로딩
	CreateCamera();
	UIScriptLoading();

	// 충돌관련 - 링크해주기
	GetScene()->m_CollisionManager.Link(L"Player", L"Monster");

	// 플레이어 로딩
	PlayerLoading();
	CameraScriptLoading();

	// 몬스터로딩

	// 콜라이더 로딩

	// 사운드 로딩
	BGMSoundLoading();


	 
#pragma region MyRegion
	//m_GamePlayer = GetScene()->GameObject_Create(L"Player");
	//m_GamePlayer->GetTransform()->Local_Scale(Vector4(128.0f, 128.0f, 100.0f));
	//m_GamePlayer->Component_Add<SpriteRenderer2D>(2)->SetSprite(L"AllCharacter.png", 0);
	//m_GamePlayer->Component_Add<SpriteRenderer2D>(3)->SetSprite(L"Arm.png", 0);
	//m_GamePlayer->Component_Add<SpriteRenderer2D>(5)->SetSprite(L"Body.png", 0);
	//m_GamePlayer->Component_Add<SpriteRenderer2D>(4)->SetSprite(L"Coat.png", 0);
	//m_GamePlayer->Component_Add<Animation2D>();
	//m_GamePlayer->Component_Add<MoveScript>();
	//m_GamePlayer->DebugMode_On();
#pragma endregion

	// 충돌체
	// 플레이어
	My_PTR<RectCol2D> ColPlayer = m_GamePlayer->Component_Add<RectCol2D>(L"Player");
	ColPlayer->Local_SubScale(Vector4(50.0f, 50.0f, 50.0f));
	ColPlayer->Local_SubPosition(Vector4(0.0f, -10.0f, 0.0f));
	ColPlayer->PushCollisionEnter(&TestGameScene::ColTestEnter, this);
	ColPlayer->PushCollisionStay(&TestGameScene::ColTestStay, this);
	ColPlayer->PushCollisionExit(&TestGameScene::ColTestExit, this);
	//ColPlayer->SetColSize(Vector2{ 300.0f, 300.0f });


	// 아이 마네킹
	m_CMMonster = GetScene()->GameObject_Create(L"ChildManeeqin");
	m_CMMonster->GetTransform()->Local_Position(Vector4(-350.0f, 200.0f, 100.0f));
	m_CMMonster->GetTransform()->Local_Scale(Vector4(118.0f, 137.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> CMMonRen = m_CMMonster->Component_Add<SpriteRenderer2D>(RENDER_ORDER::MONSTER);
	CMMonRen->SetSprite(L"ChildMannequin.png", 0);
	My_PTR<Animation2D> CMMonAni = m_CMMonster->Component_Add<Animation2D>();
	My_PTR<ChildManneqin> CMScript = m_CMMonster->Component_Add<ChildManneqin>();
	CMScript->SetFloor(m_MoveTile);
	CMScript->SetPlayer(m_GamePlayer);
	CMScript->SetBackground(m_Background->GetTransform()->Local_Scale());

	// 흰 모래토끼
	m_WSRMosnter = GetScene()->GameObject_Create(L"WhiteSandRabbit");
	m_WSRMosnter->GetTransform()->Local_Position(Vector4(-350.0f, 200.0f, 100.0f));
	m_WSRMosnter->GetTransform()->Local_Scale(Vector4(106.0f, 86.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> WSRRenderer = m_WSRMosnter->Component_Add<SpriteRenderer2D>(RENDER_ORDER::MONSTER);
	WSRRenderer->SetSprite(L"WhiteSandRabbit.png", 0);
	My_PTR<Animation2D> WSRAni = m_WSRMosnter->Component_Add<Animation2D>();
	My_PTR<WhiteSandRabbit> WSRScript = m_WSRMosnter->Component_Add<WhiteSandRabbit>();
	WSRScript->SetFloor(m_MoveTile);
	WSRScript->SetPlayer(m_GamePlayer);
	WSRScript->SetBackground(m_Background->GetTransform()->Local_Scale());

	// 슬라임
	m_TSMonster = GetScene()->GameObject_Create(L"TransfromSlime");
	m_TSMonster->GetTransform()->Local_Position(Vector4(-450.0f, 0.0f, 100.0f));
	m_TSMonster->GetTransform()->Local_Scale(Vector4(306.0f, 276.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> TSRenderer = m_TSMonster->Component_Add<SpriteRenderer2D>(RENDER_ORDER::MONSTER);
	TSRenderer->SetSprite(L"TransformSlime.png", 0);
	My_PTR<Animation2D> TSAni = m_TSMonster->Component_Add<Animation2D>();
	My_PTR<TransformSlime> TSScript = m_TSMonster->Component_Add<TransformSlime>();
	TSScript->SetUpTilePos(true);		// 타일위의 몹인지 여부
	TSScript->SetFloor(m_MoveTile);
	TSScript->SetPlayer(m_GamePlayer);
	TSScript->SetBackground(m_Background->GetTransform()->Local_Scale());


	// 벨라리스
	m_VMonster = GetScene()->GameObject_Create(L"TransfromSlime1");
	m_VMonster->GetTransform()->Local_Position(Vector4(-450.0f, 300.0f, 100.0f));
	m_VMonster->GetTransform()->Local_Scale(Vector4(409.0f, 271.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> VelRenderer = m_VMonster->Component_Add<SpriteRenderer2D>(RENDER_ORDER::MONSTER);
	VelRenderer->SetSprite(L"Velaris.png", 0);
	My_PTR<Animation2D> VelAni = m_VMonster->Component_Add<Animation2D>();
	My_PTR<Velaris> VelScript = m_VMonster->Component_Add<Velaris>();
	VelScript->SetUpTilePos(true);		// 타일위의 몹인지 여부
	VelScript->SetFloor(m_MoveTile);
	VelScript->SetPlayer(m_GamePlayer);
	VelScript->SetBackground(m_Background->GetTransform()->Local_Scale());


	// 핑크빈 (리젠)
	m_PinkbinMonster = GetScene()->GameObject_Create(L"PinkbinRegen");
	m_PinkbinMonster->GetTransform()->Local_Position(Vector4(-400.0f, 0.0f, 100.0f));
	m_PinkbinMonster->GetTransform()->Local_Scale(Vector4(451.0f, 347.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> PinkRenderer = m_PinkbinMonster->Component_Add<SpriteRenderer2D>(RENDER_ORDER::MONSTER);
	PinkRenderer->SetSprite(L"Pinkbin_Regen.png", 0);
	My_PTR<Animation2D> PinkAni = m_PinkbinMonster->Component_Add<Animation2D>();
	My_PTR<PinkbinRegen> PinkScript = m_PinkbinMonster->Component_Add<PinkbinRegen>();
	PinkScript->SetUpTilePos(false);
	PinkScript->SetFloor(m_MoveTile);
	PinkScript->SetPlayer(m_GamePlayer);
	PinkScript->SetBackground(m_Background->GetTransform()->Local_Scale());


	// 몬스터
	m_Monster = GetScene()->GameObject_Create(L"Monster");
	m_Monster->GetTransform()->Local_Scale(Vector4(128.0f, 128.0f, 100.0f));
	m_Monster->GetTransform()->Local_Position(Vector4(400.0f, 200.0f, 100.0f));

	m_Monster->Component_Add<SpriteRenderer2D>(RENDER_ORDER::MONSTER)->SetSprite(L"AllCharacter.png", 0);
	m_Monster->Component_Add<Animation2D>();
	//m_Monster->Component_Add<RectCol2D>(L"Monster");			// 충돌
	m_Monster->DebugMode_On();

	My_PTR<RectCol2D> ColMonster = m_Monster->Component_Add<RectCol2D>(L"Monster");

	m_SkillManager->SetMonster(m_Monster);// 몬스터 넣어주기


	return true;
}

void TestGameScene::Start()
{
}

void TestGameScene::Update()
{

}

void TestGameScene::LateUpdate()
{
}

void TestGameScene::End()
{

}

////////////////////////////////////////////////////////////////////// 로딩관련

void TestGameScene::BGLoading()
{
	// 배경
	m_Background = GetScene()->GameObject_Create(L"AC_Background");
	m_Background->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 300.0f));
	m_Background->GetTransform()->Local_Scale(Vector4(3130.0f, 925.0f, 1.0f));
	m_Background->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BACKGROUND)->SetSprite(L"AC_Background.png", 0);

	//m_Background = GetScene()->GameObject_Create(L"AC_Background");
	//m_Background->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 300.0f));
	//m_Background->GetTransform()->Local_Scale(Vector4(3130.0f, 925.0f, 1.0f));
	//m_Background->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BACKGROUND)->SetSprite(L"ArcanaBGAni_1.png", 0);
	//My_PTR<Animation2D> BGANi = m_Background->Component_Add<Animation2D>();
	//My_PTR<BG_TownScript> BGScript = m_Background->Component_Add<BG_TownScript>();

	// 충돌할 타일
	m_MoveTile = GetScene()->GameObject_Create(L"AC_MoveTile");
	m_MoveTile->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 400.0f));
	m_MoveTile->GetTransform()->Local_Scale(Vector4(3130.0f, 925.0f, 1.0f));
	m_MoveTile->Component_Add<SpriteRenderer2D>(RENDER_ORDER::COLTILE)->SetSprite(L"AC_MoveTile.png", 0);
}

void TestGameScene::UILoading()
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

// UI 스크립트 로딩
void TestGameScene::UIScriptLoading()
{
	m_StatusBarBackgroundScript = m_StatusBarBackground->Component_Add<UIMove>();
	m_StatusBarBackgroundScript->SetCamera(m_MainCamera);
	m_StatusBarBackgroundScript->SetBasePos(m_StatusBarBackground->GetTransform()->Local_Position());

	m_StatusBarBaseScript = m_StatusBarBase->Component_Add<UIMove>();
	m_StatusBarBaseScript->SetCamera(m_MainCamera);
	m_StatusBarBaseScript->SetBasePos(m_StatusBarBase->GetTransform()->Local_Position());

	//m_StatusQuickSlotScript = m_StatusQuickSlot->Component_Add<UIMove>();
	//m_StatusQuickSlotScript->SetCamera(m_MainCamera);
	//m_StatusQuickSlotScript->SetBasePos(m_StatusQuickSlot->GetTransform()->Local_Position());

	m_GaugeGraduationScript = m_GaugeGraduation->Component_Add<UIMove>();
	m_GaugeGraduationScript->SetCamera(m_MainCamera);
	m_GaugeGraduationScript->SetBasePos(m_GaugeGraduation->GetTransform()->Local_Position());

	m_StatusBarGaugeScript = m_StatusBarGauge->Component_Add<UIMove>();
	m_StatusBarGaugeScript->SetCamera(m_MainCamera);
	m_StatusBarGaugeScript->SetBasePos(m_StatusBarGauge->GetTransform()->Local_Position());

	m_HPGrayScript = m_HPGray->Component_Add<UIMove>();
	m_HPGrayScript->SetCamera(m_MainCamera);
	m_HPGrayScript->SetBasePos(m_HPGray->GetTransform()->Local_Position());

	m_MPGrayScript = m_MPGray->Component_Add<UIMove>();
	m_MPGrayScript->SetCamera(m_MainCamera);
	m_MPGrayScript->SetBasePos(m_MPGray->GetTransform()->Local_Position());

	m_ExpGrayScript = m_ExpGray->Component_Add<UIMove>();
	m_ExpGrayScript->SetCamera(m_MainCamera);
	m_ExpGrayScript->SetBasePos(m_ExpGray->GetTransform()->Local_Position());

}

// 카메라 생성
void TestGameScene::CreateCamera()
{
	m_MainCamera = GetScene()->GameObject_Create(L"MainCamera");

	m_CameraCom = m_MainCamera->Component_Add<Camera>(0);
	m_CameraCom->SettingViewGroup(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	m_CameraCom->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -10.0f));
}


void TestGameScene::PlayerLoading()
{
	// 캐릭터 부모
	m_GamePlayer = GetScene()->GameObject_Create(L"Player");
	m_GamePlayer->GetTransform()->Local_Position(Vector4(0.0f, 100.0f, 100.0f));
	m_GamePlayer->GetTransform()->Local_Scale(Vector4(128.0f, 128.0f, 1.0f));

	m_PlayerMoveScript = m_GamePlayer->Component_Add<PlayerMove>();
	m_PlayerMoveScript->AddFloor(m_MoveTile);
	//m_GamePlayer->GetComponent<PlayerMove>()->AddFloor(m_MoveTile);
	m_SkillManager = m_GamePlayer->Component_Add<SkillManager>();
	m_SkillManager->SetPlayerScript(m_PlayerMoveScript->GetPlayerMoveScript());
	m_SkillManager->SetMapWidth(m_Background->GetTransform()->Local_Scale().x);
	m_GamePlayer->DebugMode_On();


	CharacterChildInit(m_PlayerCoatArm, L"Player_CoatArm", L"CoatArm.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 1.0f);		// 코트 팔
	CharacterChildInit(m_PlayerArm, L"Player_Arm", L"Arm.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 2.0f);					// 팔
	CharacterChildInit(m_PlayerArm, L"Player_Weapon", L"Weapon_Rabbit.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 3.0f);		// 무기
	CharacterChildInit(m_PlayerArm, L"Player_Eye", L"Eye.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 4.0f);					// 눈
	CharacterChildInit(m_PlayerArm, L"Playe_Hair", L"Hair.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 5.0f);					// 헤어
	CharacterChildInit(m_PlayerArm, L"Player_Head", L"Head.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 6.0f);					// 머리
	CharacterChildInit(m_PlayerArm, L"Player_Coat", L"Coat.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 7.0f);					// 코트옷
	CharacterChildInit(m_PlayerArm, L"Player_Body", L"Body.png", m_GamePlayer, RENDER_ORDER::PLAYER_CHILD, 8.0f);					// 바디
}

void TestGameScene::CharacterChildInit(My_PTR<GameObject> _GameObject, const wchar_t* _ObjName, const wchar_t* _ImageName, My_PTR<GameObject> _Parent, int _RenderOrder, float _Z)
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


void TestGameScene::CameraScriptLoading()
{
	m_CamScript = m_MainCamera->Component_Add<InGameCamera>();
	m_CamScript->SetCurMap(m_Background);
	m_CamScript->SetPlayer(m_GamePlayer);
}


////////////////////////////////////////////////////////////

// 충돌
void TestGameScene::ColTestEnter(Collision* _This, Collision* _Other)
{
	int a = 0;
	_This->GetGameObject()->Death();		// 부딪히면 사망하게하기

	//_Other->GetTransform()->Local_Scale(_Other->GetTransform()->Local_Scale() * 0.8f);		// 점점 줄어들게하기

}

void TestGameScene::ColTestStay(Collision* _This, Collision* _Other)
{
	int a = 0;
	//_Other->GetTransform()->Local_Scale(_Other->GetTransform()->Local_Scale() * 0.8f);		// 점점 줄어들게하기
}

void TestGameScene::ColTestExit(Collision* _This, Collision* _Other)
{
	int a = 0;
}


void TestGameScene::DebugRender()
{
	GameDebug::DrawStr(L"하하하ㅏ 크햐햐햐햐햫ㅅ 폰트 나쁜넘ㅁ 으아얌ㄹ너아러ㅑㄴ거ㅑㅇㄴㅁㄹ 너시러", 20.0f, { 100.f, 40.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);
}

void TestGameScene::SoundPlay_BGM()
{
	m_BGMPlayer->Play(L"ArcanaBoss.mp3");
}

//
void TestGameScene::BGMSoundLoading()
{
	// 사운드플레이
	SoundPlay_BGM();

	m_BGMEmptyObject = GetScene()->GameObject_Create(L"BGMPlayer");
	m_BGMEmptyObject->GetTransform()->Local_Position(Vector4::Zero);
	m_BGMEmptyObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_BGMPlayer = m_BGMEmptyObject->Component_Add<SoundPlayer>();
}