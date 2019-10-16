#pragma once
#include "Device.h"
#include "Camera.h"
#include "Renderer.h"

#include <map>
#include <set>

// 렌더 순서와 해당 렌더 그룹이름을 담당할 클래스  (구조체처럼 쓰임)
class RenderKeyValue
{
public:
	int Order;					// 렌더 순서
	std::wstring GroupName;		// 렌더 그룹 이름
};

class RenderManager
{
public:
	friend class Scene;
	friend class Camera;
	friend class Renderer;

	// 카메라
private:
	std::multimap<int, My_PTR<Camera>> m_CameraOrderGroup;			// 카메라 모음 ㅇㅅㅇ

	std::multimap<int, My_PTR<Camera>>::iterator m_StartCamera;
	std::multimap<int, My_PTR<Camera>>::iterator m_EndCamera;

private:
	void Insert_Camera(Camera* _Camera);						// 카메라 추가



	// 렌더
private:
	std::map<int, std::list<My_PTR<Renderer>>> m_RendererOrderGroup;			// 렌더러 모음 ㅇㅅㅇ

	std::map<int, std::list<My_PTR<Renderer>>>::iterator RendererGroupOrder_StartIter;
	std::map<int, std::list<My_PTR<Renderer>>>::iterator RendererGroupOrder_EndIter;

	std::list<My_PTR<Renderer>>::iterator RendererGroup_StartIter;
	std::list<My_PTR<Renderer>>::iterator RendererGroup_EndIter;


private:
	void Camera_Render(const std::multimap<int, My_PTR<Camera>>::iterator& _Iter);
	void Group_Render(My_PTR<Camera> _Camera, std::list<My_PTR<Renderer>>& _List);

private:
	void Insert_Renderer(Renderer* _Renderer);

private:
	void Init();
	void Render();

public:
	void RenderOrder_Create(int _Order);
	void Release();							// 지우기

private:
	RenderManager();
	~RenderManager();
};

