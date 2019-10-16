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
	bool Loading() override;			//  Scene�� �ε����� �ʾ��� �� �ѹ��� ���� (�ε��Ǿ������� ����)
	void Start() override;							// Scene�� ü���� �Ǿ����� �ѹ��� ����
	void Update() override;						// ��� ���� ������Ʈ��(Actor��...)�� ������Ʈ�� ����Ǳ� ���� �ѹ� ����
	void LateUpdate() override;					// ��� ���� ������Ʈ���� ������Ʈ�� ������ �ѹ� ����
	void End() override;							// ���� �������� ������ �ٸ������� ����Ǿ����� �ѹ��� ����
	void DebugRender() override;


private:
	void ImageLoading();
	void PathFileLoad(std::wstring _RootPathFolder);
	void SetMapRenderFolderPath();
public:
	MapScript();
	~MapScript();
};

