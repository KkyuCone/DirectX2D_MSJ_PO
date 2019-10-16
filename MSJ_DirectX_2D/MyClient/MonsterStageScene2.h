#pragma once
#include <Scene.h>
#include <RenderOrder.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class EsferaMonsterPos
{
public:
	friend class MonsterStageScene2;
public:
	Vector4 MonsterPos;
	bool m_bTile;
	bool m_life;

public:
	EsferaMonsterPos() {};
	~EsferaMonsterPos() {};

};

class Collision;
class GameObject;
class InGameCamera;
class UIMove;
class PlayerMove;
class SkillManager;
class MonsterStageScene2 : public Scene::SceneScript, public RenderOrder
{
private:
	My_PTR<GameObject> m_GamePlayer;
	My_PTR<GameObject> m_PlayerCoatArm;		// 코트 팔부분
	My_PTR<GameObject> m_PlayerArm;			// 팔
	My_PTR<GameObject> m_PlayerEye;			// 눈
	My_PTR<GameObject> m_PlayerHair;		// 헤어 오브젝트
	My_PTR<GameObject> m_PlayerHead;		// 머리 (대머리)
	My_PTR<GameObject> m_PlayerCoat;		// 코트 상반신 전체
	My_PTR<GameObject> m_PlayerBody;		// 몸

	My_PTR<PlayerMove> m_PlayerMoveScript;
	My_PTR<SkillManager> m_SkillManager;
private:
	My_PTR<GameObject> m_ChildManeequin;
	My_PTR<GameObject> m_TransformSlime;

private:
	My_PTR<GameObject> m_Background;
	My_PTR<GameObject> m_MoveTile;

private:
	My_PTR<GameObject> m_MainCamera;
	My_PTR<Camera> m_CameraCom;
	My_PTR<InGameCamera> m_CamScript;

private:
	My_PTR<GameObject> m_UICamera;
	My_PTR<Camera> m_UICameraCom;

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
	My_PTR<GameObject> m_PrePortal;
	My_PTR<GameObject> m_NextPortal;

private:
	// 몬스터 포스
	std::vector<EsferaMonsterPos> m_VelarisMosnterPos;
	std::unordered_map<int, GameObject*> m_VelarisMonsterMap;

	std::vector<EsferaMonsterPos> m_SlimeMonsterPos;
	std::unordered_map<int, GameObject*> m_SlimeMonsterMap;

	// 몬스터들

private:
	void BGLoading();
	void UILoading();
	void CreateCamera();
	void PortalLoading();
	void PlayerLoading();
	void CameraScriptLoading();
	void MonsterPosInit();			// 몬스터 포스 초기화
	void VelarisPosInit();
	void SlimePosInit();
	void MonsterLoading();

private:
	void CharacterChildInit(My_PTR<GameObject> _GameObject, const wchar_t* _ObjName, const wchar_t* _ImageName, My_PTR<GameObject> _Parent, int _RenderOrder, float _Z);

public:
	bool Loading() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void End() override;
	void DebugRender() override;
public:
	MonsterStageScene2();
	~MonsterStageScene2();
};

