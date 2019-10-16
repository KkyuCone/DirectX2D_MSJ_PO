#include "Core.h"
#include "Window.h"
#include "Device.h"
#include "RenderManager.h"
#include "Path.h"
#include <time.h>
#include "SoundResource.h"

// Static ����
bool Core::m_MyCoreLive = true;


// �Լ�

void Core::Start(My_CoreBuilder* _CoreBuilder, HINSTANCE _hInstance, const wchar_t* _WindowName, Vector2_Int _Size)
{
	srand((unsigned int)time(0));
	//Path::Init();
	SoundResource::SoundInit();
	SoundResource::SoundBGMInit();

	Path::RootToCreatePath(L"Shader");

	Window::m_hInst = _hInstance;
	Window::Init(_WindowName);			// ������ �̸� ����
	Window::Size(_Size);
	Device::Init();
	//RenderManager::Init();

	_CoreBuilder->Build();

	// �޼��� ������ ������.
	// �⺻ �޽��� �����Դϴ�.
	MSG msg;

	while (true == m_MyCoreLive)
	{
		// �޼����� ó�����Ҽ��� ����.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); // 
		}
		else
		{
			// ����Ÿ��
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