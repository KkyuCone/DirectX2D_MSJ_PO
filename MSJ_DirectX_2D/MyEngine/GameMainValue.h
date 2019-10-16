#pragma once
#include "Device.h"
#include "Window.h"
#include "Scene.h"
#include "Camera.h"

class GameMainValue
{
	// �� Ŭ������ ������ �Ѳ�����.. �θ����� ������� ������
	// ����� ���� ���� �ֱ⶧���� �����Ƽ�..�Ѳ����� �Ѱž� ������
public:
	static ID3D11Device*		PDevice()		{ return Device::Get_Device(); }
	static ID3D11DeviceContext* PContext()		{ return Device::Get_Context(); }
	static HWND					HWND()			{ return Window::WindowHWND(); }
	static My_PTR<Scene>		MainScene()		{ return Scene::Get_CurrentScene(); }
	static Camera*				MainCamera()	{ return Scene::Get_CurrentScene()->GetCamera(); }
public:
	GameMainValue();
	~GameMainValue();
};

