#pragma once
#include "Device.h"
#include "Window.h"
#include "Scene.h"
#include "Camera.h"

class GameMainValue
{
	// 이 클래스는 앞으로 한꺼번에.. 부를려고 만든거임 ㅇㅅㅇ
	// 헤더가 각각 따로 있기때문에 귀찮아서..한꺼번에 한거얌 ㅇㅅㅇ
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

