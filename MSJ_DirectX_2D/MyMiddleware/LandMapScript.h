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

	My_PTR<LandRenderer> m_CurLandMapRender;		// 현재 맵타일깔아주는 Renderer .... 
	std::list<My_PTR<LandRenderer>> m_LandMapList;	// 맵타일 깔아놓은 애들..

	bool m_bBigTile;		// 선택된 타일의 크기 여부 (큰타일인지 작은타일인지)

	bool m_bSlope;			// 경사타일의 생성 유무( false라면 일만 절벽 타일 )
	bool m_bDownSlope;		// 하단 경사타일의 생성 유무
	bool m_bSlimTile;		// 얇은 타일의 유무
	bool m_bSmallTile;		// 작은 타일의 유무

	int SaveSceneX;
	int SaveSCeneY;

	// 직접 찍을 수 있는타일
	std::vector<std::wstring> m_BigTileNames;
	std::vector<std::wstring> m_SmallTileNames;

	// 자동타일 전체묶음 'ㅅ'
	std::unordered_map<std::wstring, std::vector<std::wstring>> m_AllAutoTileMap;

	// 자동타일 - 일반
	std::vector<std::wstring> m_RSlopAutoTileVec;	// 오른쪽 경사타일
	std::vector<std::wstring> m_LSlopAutoTileVec;	// 왼쪽 경사타일
	std::vector<std::wstring> m_UpAutoTileVec;		// 위쪽 타일
	std::vector<std::wstring> m_LeftAutoTileVec;	// 왼쪽 타일
	std::vector<std::wstring> m_RightAutoTileVec;	// 오른쪽 타일
	std::vector<std::wstring> m_DownAutoTileVec;	// 아래 타일(기본)
	std::vector<std::wstring> m_RDSlopAutoTileVec;	// 오른쪽 아래 타일(방향존재)  오른쪽 하단 경사 타일
	std::vector<std::wstring> m_LDSlopAutoTileVec;	// 왼쪽 아래 타일(방향존재)

	// 자동타일 - 작은타일
	std::vector<std::wstring> m_SDownAutoTileVec;	// 작은 아래타일 (이거 하나뿐임)

	std::vector<std::wstring> m_SDownAutoLHalfTileVec;	// 좌하단
	std::vector<std::wstring> m_SDownAutoRHalfTileVec;	// 우하단

	std::vector<std::wstring> m_SUpAutoLHalfTileVec;	// 좌상단
	std::vector<std::wstring> m_SUpAutoRHalfTileVec;	// 우상단


	// 현재 선택된 폴더와 타일 이름(기준)
	std::wstring m_CurFolderName;
	std::wstring m_CurTileImageName;
	//std::vector<std::wstring> m_PathName;

	std::unordered_map<std::wstring, std::vector<std::wstring>> m_ALlTileMap;		 // 전체타일
	std::unordered_map<std::wstring, std::vector<std::wstring>> m_BigTileMap;		// 내가 찍을 수 있는 큰타일
	std::unordered_map<std::wstring, std::vector<std::wstring>> m_SmallTileMap;		// 내가 찍을 수 있는 작은타일
	std::unordered_map<std::wstring, std::vector<std::wstring>> m_SlimTileMap;		// 얇은 타일

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


	// 카메라 관련
	void SetSceneSize(int _X, int _Y);


public:
	bool Init() override;
	bool Loading() override;			//  Scene이 로딩되지 않았을 때 한번만 실행 (로딩되어있으면 안함)
	void Start() override;							// Scene이 체인지 되었을때 한번만 실행
	void Update() override;						// 모든 게임 오브젝트들(Actor들...)의 업데이트가 실행되기 전에 한번 실행
	void LateUpdate() override;					// 모든 게임 오브젝트들의 업데이트가 끝나고 한번 실행
	void End() override;							// 현재 실행중인 씬에서 다른씬으로 변경되었을때 한번만 실행
	void DebugRender() override;

public:
	LandMapScript();
	~LandMapScript();
};

