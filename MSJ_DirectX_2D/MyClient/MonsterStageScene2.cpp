#include "MonsterStageScene2.h"
#include <Scene.h>
#include <TownPortalAniScript.h>

#include <TransformSlime.h>
#include <Velaris.h>
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


MonsterStageScene2::MonsterStageScene2()
	: m_GamePlayer(nullptr)
	, m_PlayerCoatArm(nullptr), m_PlayerArm(nullptr), m_PlayerEye(nullptr), m_PlayerHair(nullptr), m_PlayerHead(nullptr), m_PlayerCoat(nullptr), m_PlayerBody(nullptr)
	, m_PlayerMoveScript(nullptr), m_SkillManager(nullptr)
	, m_ChildManeequin(nullptr), m_TransformSlime(nullptr)
	, m_Background(nullptr), m_MoveTile(nullptr)
	, m_MainCamera(nullptr), m_CameraCom(nullptr), m_CamScript(nullptr)
	, m_UICamera(nullptr), m_UICameraCom(nullptr)
	, m_StatusBarBackground(nullptr), m_StatusBarBase(nullptr), m_StatusQuickSlot(nullptr)
	, m_StatusBarGauge(nullptr), m_GaugeGraduation(nullptr)
	, m_HPGray(nullptr), m_MPGray(nullptr), m_ExpGray(nullptr)
	, m_PrePortal(nullptr), m_NextPortal(nullptr)
{
}


MonsterStageScene2::~MonsterStageScene2()
{
}


bool MonsterStageScene2::Loading()
{
	BGLoading();
	UILoading();
	PortalLoading();
	CreateCamera();
	PlayerLoading();
	CameraScriptLoading();
	MonsterPosInit();
	MonsterLoading();

	return true;
}

void MonsterStageScene2::Start()
{
	int a = 0;
}

void MonsterStageScene2::Update()
{

}

void MonsterStageScene2::LateUpdate()
{

}

void MonsterStageScene2::End()
{

}

void MonsterStageScene2::DebugRender()
{
	// FPS
	wchar_t CurFPS[256];
	swprintf_s(CurFPS, L"FPS = %d", Time::FPS());
	GameDebug::DrawStr(CurFPS, 20, { 10.0f, 30.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);

	// 캐릭터 포스
	wchar_t PosXString[256];
	float PlayerX = m_GamePlayer->GetTransform()->Local_Position().x;
	swprintf_s(PosXString, L"X = %d", (int)PlayerX);
	GameDebug::DrawStr(PosXString, 20, { 10.0f, 60.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);

	wchar_t PosYString[256];
	float PlayerY = m_GamePlayer->GetTransform()->Local_Position().y;
	swprintf_s(PosYString, L"Y = %d", (int)PlayerY);
	GameDebug::DrawStr(PosYString, 20, { 150.0f, 60.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);
}

//

void MonsterStageScene2::BGLoading()
{
	// 뒷배경
	m_Background = GetScene()->GameObject_Create(L"Esfera_Bacground");
	m_Background->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 300.0f));
	m_Background->GetTransform()->Local_Scale(Vector4(3520.0f, 1480.0f, 1.0f));
	m_Background->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BACKGROUND)->SetSprite(L"Esfera_BG.png", 0);
	My_PTR<Animation2D> BGANi = m_Background->Component_Add<Animation2D>();

	// 충돌할 타일
	m_MoveTile = GetScene()->GameObject_Create(L"Esfera_MoveTile");
	m_MoveTile->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 400.0f));
	m_MoveTile->GetTransform()->Local_Scale(Vector4(3520.0f, 1480.0f, 1.0f));
	m_MoveTile->Component_Add<SpriteRenderer2D>(RENDER_ORDER::COLTILE)->SetSprite(L"Esfera_MoveTile.png", 0);
}

void MonsterStageScene2::UILoading()
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

void MonsterStageScene2::CreateCamera()
{
	// 메인 카메라
	m_MainCamera = GetScene()->GameObject_Create(L"MainCamera");
	m_CameraCom = m_MainCamera->Component_Add<Camera>(0);
	m_CameraCom->SettingViewGroup(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30);
	m_CameraCom->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -10.0f));

	// UI 카메라
	m_UICamera = GetScene()->GameObject_Create(L"UICamera");
	m_UICameraCom = m_UICamera->Component_Add<Camera>(1);
	m_UICameraCom->SettingViewGroup(50, 51, 52, 53, 54, 55, 56, 57, 58, 59);
	m_UICameraCom->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -10.0f));
}

void MonsterStageScene2::PortalLoading()
{
	//m_PrePortal = GetScene()->GameObject_Create(L"PrePortal_Esfera");
	//m_PrePortal->GetTransform()->Local_Position(Vector4(-1600.0f, -495.0f, 0.0f, 1.0f));
	//m_PrePortal->GetTransform()->Local_Scale(Vector4(89.0f, 257.0f, 1.0f, 1.0f));
	//My_PTR<SpriteRenderer2D> PrePortalRenderer = m_PrePortal->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_PORTAL);
	//PrePortalRenderer->SetSprite(L"Portal.png", 0);
	//My_PTR<Animation2D> PrePortalAnim = m_PrePortal->Component_Add<Animation2D>();
	//My_PTR<TownPortalAniScript> PrePortalAniScript = m_PrePortal->Component_Add<TownPortalAniScript>();


	// 에스페라는 next가 pre임
	m_NextPortal = GetScene()->GameObject_Create(L"NextPortal_Esfera");
	m_NextPortal->GetTransform()->Local_Position(Vector4(1600.0f, -495.0f, 0.0f, 1.0f));
	m_NextPortal->GetTransform()->Local_Scale(Vector4(89.0f, 257.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> NextPortalRenderer = m_NextPortal->Component_Add<SpriteRenderer2D>(RENDER_ORDER::UI_PORTAL);
	NextPortalRenderer->SetSprite(L"Portal.png", 0);
	My_PTR<Animation2D> NextPortalAnim = m_NextPortal->Component_Add<Animation2D>();
	My_PTR<TownPortalAniScript> NextPortalAniScript = m_NextPortal->Component_Add<TownPortalAniScript>();

}

void MonsterStageScene2::PlayerLoading()
{
	// 캐릭터 부모
	m_GamePlayer = GetScene()->GameObject_Create(L"Player");
	m_GamePlayer->GetTransform()->Local_Position(Vector4(0.0f, 100.0f, 100.0f));
	m_GamePlayer->GetTransform()->Local_Scale(Vector4(128.0f, 128.0f, 1.0f));

	m_PlayerMoveScript = m_GamePlayer->Component_Add<PlayerMove>();
	m_PlayerMoveScript->AddFloor(m_MoveTile);
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

void MonsterStageScene2::CameraScriptLoading()
{
	m_CamScript = m_MainCamera->Component_Add<InGameCamera>();
	m_CamScript->SetCurMap(m_Background);
	m_CamScript->SetPlayer(m_GamePlayer);
}

void MonsterStageScene2::MonsterPosInit()
{
	VelarisPosInit();
	SlimePosInit();
}

void MonsterStageScene2::VelarisPosInit()
{
	Vector4 NewPos = Vector4(-1535.0f, -480.0f, 150.0f, 1.0f);
	std::vector<Vector4> NewPosVector;
	NewPosVector.push_back(NewPos);
	
	NewPos.x = -1182.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -889.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -725.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -556.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -248.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 11.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 229.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 513.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 778.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 1000.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 1255.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	//

	NewPos.y = 300.0f;

	NewPos.x = -585.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -451.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -244.0f;
	NewPosVector.push_back(NewPos);

	//

	NewPos.y = 100.0f;

	NewPos.x = -1469.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -1248.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);


	for (int i = 0; i < NewPosVector.size(); ++i)
	{
		EsferaMonsterPos NewVelPos;
		NewVelPos.MonsterPos = NewPosVector[i];
		NewVelPos.m_bTile = true;
		NewVelPos.m_life = true;

		if (NewPosVector[i].y <= -480.0f)
		{
			NewVelPos.m_bTile = false;
		}

		m_VelarisMosnterPos.push_back(NewVelPos);
	}

}
void MonsterStageScene2::SlimePosInit()
{
	Vector4 NewPos = Vector4(-1087.0f, 80.0f, 110.0f, 1.0f);
	std::vector<Vector4> NewPosVector;
	NewPosVector.push_back(NewPos);

	NewPos.x = -1380.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);


	//

	NewPos.y = -100.0f;

	NewPos.x = -1440.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -1325.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -1233.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -1141.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -1091.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	//
	NewPos.y = -120.0f;

	NewPos.x = 34.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 141.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 260.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 933.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 472.0f;
	NewPos.z -= 1.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = 592.0f;
	NewPos.z -= 719.0f;
	NewPosVector.push_back(NewPos);

	//
	NewPos.y = -100.0f;

	NewPos.x = -1258.0f;
	NewPos.z -= 719.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -1105.0f;
	NewPos.z -= 719.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -996.0f;
	NewPos.z -= 719.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -867.0f;
	NewPos.z -= 719.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -760.0f;
	NewPos.z -= 719.0f;
	NewPosVector.push_back(NewPos);

	NewPos.x = -621.0f;
	NewPos.z -= 719.0f;
	NewPosVector.push_back(NewPos);


	for (int i = 0; i < NewPosVector.size(); ++i)
	{
		EsferaMonsterPos NewVelPos;
		NewVelPos.MonsterPos = NewPosVector[i];
		NewVelPos.m_bTile = true;
		NewVelPos.m_life = true;

		m_SlimeMonsterPos.push_back(NewVelPos);
	}

}

void MonsterStageScene2::MonsterLoading()
{
	// 몬스터 로딩
	// 벨라리스
	for (int i = 0; i < (int)(m_VelarisMosnterPos.size()); ++i)
	{
		std::wstring MonName = L"Velaris_";
		std::wstring MonNumber = std::to_wstring(i);
		MonName += MonNumber;

		My_PTR<GameObject> NewVelaris = GetScene()->GameObject_Create(MonName.c_str());
		NewVelaris->GetTransform()->Local_Position(m_VelarisMosnterPos[i].MonsterPos);
		NewVelaris->GetTransform()->Local_Scale(Vector4(409.0f, 271.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> NewVelarisRenderer = NewVelaris->Component_Add<SpriteRenderer2D>(RENDER_ORDER::MONSTER);
		NewVelarisRenderer->SetSprite(L"Velaris.png", 0);
		My_PTR<Animation2D> NewVelarisAnim = NewVelaris->Component_Add<Animation2D>();
		My_PTR<Velaris> NewVelarisScript = NewVelaris->Component_Add<Velaris>();

		NewVelarisScript->SetUpTilePos(m_VelarisMosnterPos[i].m_bTile);		// 타일위의 몹인지 여부
		NewVelarisScript->SetFloor(m_MoveTile);
		NewVelarisScript->SetPlayer(m_GamePlayer);
		NewVelarisScript->SetBackground(m_Background->GetTransform()->Local_Scale());

		m_VelarisMonsterMap.insert(std::map<int, GameObject*>::value_type(i, NewVelaris));


		// HP바
		// 베이스
		std::wstring EmptyName = MonName;
		EmptyName += L"_HPBarEmtpyOBjCM";
		My_PTR<GameObject> HPBarEmptyObj = GetScene()->GameObject_Create(EmptyName.c_str());
		HPBarEmptyObj->GetTransform()->Local_Position(NewVelaris->GetTransform()->Local_Position());
		HPBarEmptyObj->GetTransform()->Local_Scale(Vector4::One);
		My_PTR<HPBarMoveScript> HPBarEmtpyObjScript = HPBarEmptyObj->Component_Add<HPBarMoveScript>();
		HPBarEmtpyObjScript->SetMosnter(NewVelaris);
		HPBarEmtpyObjScript->SetPivot(30.0f);

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
		DamageScript->SetHP(NewVelarisScript->GetHP());


		NewVelarisScript->SetHPBar(DamageScript);
	}


	// 슬라임
	for (int i = 0; i < (int)(m_SlimeMonsterPos.size()); ++i)
	{
		std::wstring MonName = L"Slime_";
		std::wstring MonNumber = std::to_wstring(i);
		MonName += MonNumber;

		My_PTR<GameObject> NewSlime = GetScene()->GameObject_Create(MonName.c_str());
		NewSlime->GetTransform()->Local_Position(m_SlimeMonsterPos[i].MonsterPos);
		NewSlime->GetTransform()->Local_Scale(Vector4(306.0f, 276.0f, 1.0f, 1.0f));
		My_PTR<SpriteRenderer2D> NewSlimeRenderer = NewSlime->Component_Add<SpriteRenderer2D>(RENDER_ORDER::MONSTER);
		NewSlimeRenderer->SetSprite(L"TransformSlime.png", 0);
		My_PTR<Animation2D> NewSlimeAnim = NewSlime->Component_Add<Animation2D>();
		My_PTR<TransformSlime> NewSlimeScript = NewSlime->Component_Add<TransformSlime>();

		NewSlimeScript->SetUpTilePos(m_SlimeMonsterPos[i].m_bTile);		// 타일위의 몹인지 여부
		NewSlimeScript->SetFloor(m_MoveTile);
		NewSlimeScript->SetPlayer(m_GamePlayer);
		NewSlimeScript->SetBackground(m_Background->GetTransform()->Local_Scale());

		m_SlimeMonsterMap.insert(std::map<int, GameObject*>::value_type(i, NewSlime));


		// HP바
		// 베이스
		std::wstring EmptyName = MonName;
		EmptyName += L"_HPBarEmtpyOBjCM";
		My_PTR<GameObject> HPBarEmptyObj = GetScene()->GameObject_Create(EmptyName.c_str());
		HPBarEmptyObj->GetTransform()->Local_Position(NewSlime->GetTransform()->Local_Position());
		HPBarEmptyObj->GetTransform()->Local_Scale(Vector4::One);
		My_PTR<HPBarMoveScript> HPBarEmtpyObjScript = HPBarEmptyObj->Component_Add<HPBarMoveScript>();
		HPBarEmtpyObjScript->SetMosnter(NewSlime);
		HPBarEmtpyObjScript->SetPivot(20.0f);

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
		DamageScript->SetHP(NewSlimeScript->GetHP());


		NewSlimeScript->SetHPBar(DamageScript);
	}
}


//

void MonsterStageScene2::CharacterChildInit(My_PTR<GameObject> _GameObject, const wchar_t* _ObjName, const wchar_t* _ImageName, My_PTR<GameObject> _Parent, int _RenderOrder, float _Z)
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