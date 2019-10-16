#pragma once
#include "Device.h"
#include "Camera.h"
#include "Renderer.h"

#include <map>
#include <set>

// ���� ������ �ش� ���� �׷��̸��� ����� Ŭ����  (����üó�� ����)
class RenderKeyValue
{
public:
	int Order;					// ���� ����
	std::wstring GroupName;		// ���� �׷� �̸�
};

class RenderManager
{
public:
	friend class Scene;
	friend class Camera;
	friend class Renderer;

	// ī�޶�
private:
	std::multimap<int, My_PTR<Camera>> m_CameraOrderGroup;			// ī�޶� ���� ������

	std::multimap<int, My_PTR<Camera>>::iterator m_StartCamera;
	std::multimap<int, My_PTR<Camera>>::iterator m_EndCamera;

private:
	void Insert_Camera(Camera* _Camera);						// ī�޶� �߰�



	// ����
private:
	std::map<int, std::list<My_PTR<Renderer>>> m_RendererOrderGroup;			// ������ ���� ������

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
	void Release();							// �����

private:
	RenderManager();
	~RenderManager();
};

