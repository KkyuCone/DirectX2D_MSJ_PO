#pragma once
#include <Windows.h>
#include "DXHeader.h"

class Window;
class Core
{
public:
	friend Window;

public:
	class My_CoreBuilder
	{
	public:
		virtual bool Build() = 0;

	protected:
		My_CoreBuilder() {}
		~My_CoreBuilder() {}
	};

private:
	static bool m_MyCoreLive;

public:
	static void Start(My_CoreBuilder* _CoreBuilder, HINSTANCE _hInstance, const wchar_t* _WindowName, Vector2_Int _Size);
	static void Start(My_CoreBuilder* _CoreBuilder, HINSTANCE _hInstance, const wchar_t* _WindowName, HWND _OtherWindow);

	template<typename BuilderType>
	static void Start(HINSTANCE _hInstance, const wchar_t* _WindowName, Vector2_Int _Size)
	{
		BuilderType Builder;
		Start(&Builder, _hInstance, _WindowName, _Size);
	}

	template<typename BuilderType>
	static void Start(HINSTANCE _hInstance, const wchar_t* _WindowName, HWND _OtherWindow)
	{
		BuilderType Builder;
		Start(&Builder, _hInstance, _WindowName, _OtherWindow);
	}


public:
	Core() {};
	~Core() {};
};

// WinMain�� �ֱ⶧���� ������ ������Ʈ��� �Ѵ�.
// define�� �ٹٲ��� �Ұ����ѵ� \(��������)�� �̿��ϸ� �����ϴ�.
// �Ʒ�ó�� ����ϸ� �Լ��� ȣ������ �ʰ� ����� �� �ִ�.
#include <crtdbg.h>

#define MYCORESTART(MyCoreBuilderType, MyWindowName, Size)\
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); \
	new int; \
	MyCoreBuilderType MyBuilder; \
	Core::Start(&MyBuilder, hInstance, MyWindowName, Size); \
	return 0; \
}

