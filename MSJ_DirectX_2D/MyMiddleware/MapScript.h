#pragma once
#include <Scene.h>

class MapRenderer;
class ToolCamera;
class MapScript : public Scene::SceneScript
{
public:
	static std::wstring SelectTile;
	// static My_PTR<MapRenderer> MapRender;
	static bool m_bPickStart;

private:
	My_PTR<GameObject> m_CamerObject;
	My_PTR<Camera> m_CaemraPTR;
	My_PTR<ToolCamera> m_CameraScript;
	My_PTR<GameObject> m_MapObject;

	std::vector<std::wstring> ALLFolderPath;

	My_PTR<MapRenderer> m_CurRender;

	std::list<My_PTR<MapRenderer>> m_RandList;


public:
	bool Init() override;
	bool Loading() override;			//  Scene이 로딩되지 않았을 때 한번만 실행 (로딩되어있으면 안함)
	void Start() override;							// Scene이 체인지 되었을때 한번만 실행
	void Update() override;						// 모든 게임 오브젝트들(Actor들...)의 업데이트가 실행되기 전에 한번 실행
	void LateUpdate() override;					// 모든 게임 오브젝트들의 업데이트가 끝나고 한번 실행
	void End() override;							// 현재 실행중인 씬에서 다른씬으로 변경되었을때 한번만 실행
	void DebugRender() override;


private:
	void ImageLoading();
	void PathFileLoad(std::wstring _RootPathFolder);
	void SetMapRenderFolderPath();
public:
	MapScript();
	~MapScript();
};

