#pragma once
#include <Scene.h>
#include <RenderOrder.h>

class Collision;
class GameObject;
class InGameCamera;
class UIMove;
class PlayerMove;
class SkillManager;
class SoundPlayer;
class TestGameScene : public Scene::SceneScript, public RenderOrder
{
private:
	My_PTR<GameObject> m_BGMEmptyObject;
	My_PTR<SoundPlayer> m_BGMPlayer;
private:
	My_PTR<GameObject> m_Background;
	//My_PTR<GameObject> m_TileObject;
	My_PTR<GameObject> m_MoveTile;
	My_PTR<PlayerMove> m_PlayerMoveScript;
	My_PTR<SkillManager> m_SkillManager;

private:
	My_PTR<GameObject> m_MainCamera;
	My_PTR<Camera> m_CameraCom;
	My_PTR<InGameCamera> m_CamScript;

private:
	// UI
	My_PTR<GameObject> m_StatusBarBackground;		// 기본 배경
	My_PTR<GameObject> m_StatusBarBase;				// 상태 배경
	My_PTR<GameObject> m_StatusQuickSlot;			// 퀵슬롯

	My_PTR<GameObject> m_StatusBarGauge;			// 게이지바
	My_PTR<GameObject> m_GaugeGraduation;
	My_PTR<GameObject> m_HPGray;					// HP
	My_PTR<GameObject> m_MPGray;					// MP
	My_PTR<GameObject> m_ExpGray;					// 경험치

private:
	// UI Script
	My_PTR<UIMove> m_StatusBarBackgroundScript;
	My_PTR<UIMove> m_StatusBarBaseScript;
	My_PTR<UIMove> m_StatusQuickSlotScript;

	My_PTR<UIMove> m_StatusBarGaugeScript;
	My_PTR<UIMove> m_GaugeGraduationScript;
	My_PTR<UIMove> m_HPGrayScript;
	My_PTR<UIMove> m_MPGrayScript;
	My_PTR<UIMove> m_ExpGrayScript;

private:
	// 캐릭터 부분 순서대로했음.. !
	My_PTR<GameObject> m_GamePlayer;		// 빈 캐릭터 오브젝트
	My_PTR<GameObject> m_PlayerCoatArm;		// 코트 팔부분
	My_PTR<GameObject> m_PlayerArm;			// 팔
	My_PTR<GameObject> m_PlayerEye;			// 눈
	My_PTR<GameObject> m_PlayerHair;		// 헤어 오브젝트
	My_PTR<GameObject> m_PlayerHead;		// 머리 (대머리)
	My_PTR<GameObject> m_PlayerCoat;		// 코트 상반신 전체
	My_PTR<GameObject> m_PlayerBody;		// 몸


	My_PTR<GameObject> m_Monster;			// 몬스터

	My_PTR<GameObject> m_CMMonster;			// 아이 마네킹 몬스터
	My_PTR<GameObject> m_WSRMosnter;		// 흰 모래토끼 몬스터
	My_PTR<GameObject> m_TSMonster;			// 변형된 슬라임 몬스터
	My_PTR<GameObject> m_VMonster;			// 벨라리스
	My_PTR<GameObject> m_PinkbinMonster;

	std::vector<My_PTR<GameObject>> m_MonstersVector;			// 몬스터들을 모아둔 벡터

private:
	void SoundPlay_BGM();

public:
	/*void CharacterChildInit(My_PTR<GameObject> _GameObject, const wchar_t* _ObjName, const wchar_t* _ImageName, My_PTR<GameObject> _Parent);*/
	void CharacterChildInit(My_PTR<GameObject> _GameObject, const wchar_t* _ObjName, const wchar_t* _ImageName, My_PTR<GameObject> _Parent, int _RenderOrder, float _Z);

private:
	void BGLoading();
	void UILoading();
	void UIScriptLoading();
	void CreateCamera();
	void PlayerLoading();
	void CameraScriptLoading();
	void BGMSoundLoading();


public:
	bool Loading() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void End() override;

	void DebugRender() override;

public:
	// 충돌
	void ColTestEnter(Collision* _This, Collision* _Other);
	void ColTestStay(Collision* _This, Collision* _Other);
	void ColTestExit(Collision* _This, Collision* _Other);

public:
	TestGameScene();
	~TestGameScene();
};

