#include "Core.h"
#include "Window.h"
#include "Device.h"
#include "RenderManager.h"
#include "Path.h"
#include <time.h>
#include "SoundResource.h"

// Static 변수
bool Core::m_MyCoreLive = true;


// 함수

void Core::Start(My_CoreBuilder* _CoreBuilder, HINSTANCE _hInstance, const wchar_t* _WindowName, Vector2_Int _Size)
{
	srand((unsigned int)time(0));
	//Path::Init();
	SoundResource::SoundInit();
	SoundResource::SoundBGMInit();

	Path::RootToCreatePath(L"Shader");

	Window::m_hInst = _hInstance;
	Window::Init(_WindowName);			// 윈도우 이름 설정
	Window::Size(_Size);
	Device::Init();
	//RenderManager::Init();

	_CoreBuilder->Build();

	// 메세지 루프를 돌린다.
	// 기본 메시지 루프입니다.
	MSG msg;

	while (true == m_MyCoreLive)
	{
		// 메세지를 처리안할수는 없다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); // 
		}
		else
		{
			// 데드타임
			Window::Progress();
		}
	}
}


void Core::Start(My_CoreBuilder* _CoreBuilder, HINSTANCE _hInstance, const wchar_t* _WindowName, HWND _OtherWindow)
{
	//Path::Init();
	SoundResource::SoundInit();
	SoundResource::SoundBGMInit();

	Path::RootToCreatePath(L"Shader");

	Window::m_hInst = _hInstance;
	Window::Init(_OtherWindow,_WindowName);
	Device::Init();

	_CoreBuilder->Build();
}