#pragma once
#include <Scene.h>

class LandRenderer;
class ToolCamera;
class LandMapScript : public Scene::SceneScript
{
public:
	static bool m_bPickStart;

private:
	My_PTR<GameObject> m_CameraObject;
	My_PTR<Camera> m_CameraPTR;
	My_PTR<ToolCamera> m_CameraScript;

	My_PTR<GameObject> m_LandMapObject;

	My_PTR<LandRenderer> m_CurLandMapRender;		// ���� ��Ÿ�ϱ���ִ� Renderer .... 
	std::list<My_PTR<LandRenderer>> m_LandMapList;	// ��Ÿ�� ��Ƴ��� �ֵ�..

	bool m_bBigTile;		// ���õ� Ÿ���� ũ�� ���� (ūŸ������ ����Ÿ������)

	bool m_bSlope;			// ���Ÿ���� ���� ����( false��� �ϸ� ���� Ÿ�� )
	bool m_bDownSlope;		// �ϴ� ���Ÿ���� ���� ����
	bool m_bSlimTile;		// ���� Ÿ���� ����
	bool m_bSmallTile;		// ���� Ÿ���� ����

	int SaveSceneX;
	int SaveSCeneY;

	// ���� ���� �� �ִ�Ÿ��
	std::vector<std::wstring> m_BigTileNames;
	std::vector<std::wstring> m_SmallTileNames;

	// �ڵ�Ÿ�� ��ü���� '��'
	std::unordered_map<std::wstring, std::vector<std::wstring>> m_AllAutoTileMap;

	// �ڵ�Ÿ�� - �Ϲ�
	std::vector<std::wstring> m_RSlopAutoTileVec;	// ������ ���Ÿ��
	std::vector<std::wstring> m_LSlopAutoTileVec;	// ���� ���Ÿ��
	std::vector<std::wstring> m_UpAutoTileVec;		// ���� Ÿ��
	std::vector<std::wstring> m_LeftAutoTileVec;	// ���� Ÿ��
	std::vector<std::wstring> m_RightAutoTileVec;	// ������ Ÿ��
	std::vector<std::wstring> m_DownAutoTileVec;	// �Ʒ� Ÿ��(�⺻)
	std::vector<std::wstring> m_RDSlopAutoTileVec;	// ������ �Ʒ� Ÿ��(��������)  ������ �ϴ� ��� Ÿ��
	std::vector<std::wstring> m_LDSlopAutoTileVec;	// ���� �Ʒ� Ÿ��(��������)

	// �ڵ�Ÿ�� - ����Ÿ��
	std::vector<std::wstring> m_SDownAutoTileVec;	// ���� �Ʒ�Ÿ�� (�̰� �ϳ�����)

	std::vector<std::wstring> m_SDownAutoLHalfTileVec;	// ���ϴ�
	std::vector<std::wstring> m_SDownAutoRHalfTileVec;	// ���ϴ�

	std::vector<std::wstring> m_SUpAutoLHalfTileVec;	// �»��
	std::vector<std::wstring> m_SUpAutoRHalfTileVec;	// ����


	// ���� ���õ� ������ Ÿ�� �̸�(����)
	std::wstring m_CurFolderName;
	std::wstring m_CurTileImageName;
	//std::vector<std::wstring> m_PathName;

	std::unordered_map<std::wstring, std::vector<std::wstring>> m_ALlTileMap;		 // ��üŸ��
	std::unordered_map<std::wstring, std::vector<std::wstring>> m_BigTileMap;		// ���� ���� �� �ִ� ūŸ��
	std::unordered_map<std::wstring, std::vector<std::wstring>> m_SmallTileMap;		// ���� ���� �� �ִ� ����Ÿ��
	std::unordered_map<std::wstring, std::vector<std::wstring>> m_SlimTileMap;		// ���� Ÿ��

private:
	void ImageLoading();
	void PathFileLoad(std::wstring _RootPathFolder);
	bool ClickSceneRangeExcess();

	void ALLAutoTileVectorClear();

	void FirstMapRender();
	void RemainderMapRender();
	void DrawTileNamesInit(std::wstring _TileImageName);
	void AutoTileMapAdd(std::wstring _TileType, std::vector<std::wstring> _TileVector);

	void MouseWheelEvent();


public:
	void SetFileName(std::wstring _FileName);
	void SetFolderName(std::wstring _Folder);
	void SetPathName(std::wstring _PathName);

	void SetBigTile(bool _Value);
	void SetSlopCheck(int _Value);
	void SetDownSlopCheck(int _Value);
	void SetSlimCheck(int _Value);
	std::wstring RandomTile();

	void ImageSave(std::wstring _FileName);


	// ī�޶� ����
	void SetSceneSize(int _X, int _Y);


public:
	bool Init() override;
	bool Loading() override;			//  Scene�� �ε����� �ʾ��� �� �ѹ��� ���� (�ε��Ǿ������� ����)
	void Start() override;							// Scene�� ü���� �Ǿ����� �ѹ��� ����
	void Update() override;						// ��� ���� ������Ʈ��(Actor��...)�� ������Ʈ�� ����Ǳ� ���� �ѹ� ����
	void LateUpdate() override;					// ��� ���� ������Ʈ���� ������Ʈ�� ������ �ѹ� ����
	void End() override;							// ���� �������� ������ �ٸ������� ����Ǿ����� �ѹ��� ����
	void DebugRender() override;

public:
	LandMapScript();
	~LandMapScript();
};

