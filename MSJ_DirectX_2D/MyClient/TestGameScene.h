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
	My_PTR<GameObject> m_StatusBarBackground;		// �⺻ ���
	My_PTR<GameObject> m_StatusBarBase;				// ���� ���
	My_PTR<GameObject> m_StatusQuickSlot;			// ������

	My_PTR<GameObject> m_StatusBarGauge;			// ��������
	My_PTR<GameObject> m_GaugeGraduation;
	My_PTR<GameObject> m_HPGray;					// HP
	My_PTR<GameObject> m_MPGray;					// MP
	My_PTR<GameObject> m_ExpGray;					// ����ġ

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
	// ĳ���� �κ� �����������.. !
	My_PTR<GameObject> m_GamePlayer;		// �� ĳ���� ������Ʈ
	My_PTR<GameObject> m_PlayerCoatArm;		// ��Ʈ �Ⱥκ�
	My_PTR<GameObject> m_PlayerArm;			// ��
	My_PTR<GameObject> m_PlayerEye;			// ��
	My_PTR<GameObject> m_PlayerHair;		// ��� ������Ʈ
	My_PTR<GameObject> m_PlayerHead;		// �Ӹ� (��Ӹ�)
	My_PTR<GameObject> m_PlayerCoat;		// ��Ʈ ��ݽ� ��ü
	My_PTR<GameObject> m_PlayerBody;		// ��


	My_PTR<GameObject> m_Monster;			// ����

	My_PTR<GameObject> m_CMMonster;			// ���� ����ŷ ����
	My_PTR<GameObject> m_WSRMosnter;		// �� ���䳢 ����
	My_PTR<GameObject> m_TSMonster;			// ������ ������ ����
	My_PTR<GameObject> m_VMonster;			// ���󸮽�
	My_PTR<GameObject> m_PinkbinMonster;

	std::vector<My_PTR<GameObject>> m_MonstersVector;			// ���͵��� ��Ƶ� ����

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
	// �浹
	void ColTestEnter(Collision* _This, Collision* _Other);
	void ColTestStay(Collision* _This, Collision* _Other);
	void ColTestExit(Collision* _This, Collision* _Other);

public:
	TestGameScene();
	~TestGameScene();
};

