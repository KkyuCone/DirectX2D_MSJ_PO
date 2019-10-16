#include "Window.h"
#include "Core.h"
#include "Device.h"
#include "Scene.h"
#include "RenderManager.h"
#include "Time.h"
#include "Input.h"
#include "SoundResource.h"

HINSTANCE Window::m_hInst;
HWND Window::m_hWnd;
HDC Window::m_hDC;
Vector2_Int Window::m_Size = Vector2();
bool Window::m_bFullSize = false;

bool Window::m_bIsTool = true;
int Window::m_MouseWheelValue = 0;


LRESULT Window::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	int a = 0;
	switch (_message)
	{
	case WM_MOUSEWHEEL:
		a = 10;
		break;
	case WM_DESTROY:
		Core::m_MyCoreLive = false;
		break;
	default:
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}

	return TRUE;
}

void Window::My_RegisterClass()
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"YomiWindow";
	wcex.hIconSm = nullptr;
	//ShowCursor(FALSE);

	RegisterClassExW(&wcex);
}

bool Window::My_CreateWindow(const wchar_t * _WindowName)
{
	m_hWnd = CreateWindowW(L"YomiWindow", _WindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

	// 
	if (!m_hWnd)
	{
		return false;
	}

	WindowSize_Change(1024,768);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}

void Window::Init(const wchar_t * _WindowName)
{
	My_RegisterClass();
	My_CreateWindow(_WindowName);
	m_hDC = GetDC(m_hWnd);
}

void Window::Init(HWND _hWnd, const wchar_t* _WindowName)
{
	My_RegisterClass();
	m_hWnd = _hWnd;
	SetWindowText(_hWnd, _WindowName);

	RECT RC;
	GetClientRect(_hWnd, &RC);

	m_Size.ix = RC.right;
	m_Size.iy = RC.bottom;

	m_hDC = GetDC(m_hWnd);
}

// ������ ũ�⸦ �������ִ� �Լ� (����, ����)
void Window::WindowSize_Change(int _x, int _y)
{
	SetWindowPos(m_hWnd, nullptr, 0, 0, _x, _y, SWP_NOZORDER | SWP_NOMOVE);
}

// Progress
void Window::Progress()
{
	// ���ð��� -> �Ž�
	// ������� -> ����
	// ����� -> �����
	// ���� -> 0~1f�� ������ ��� ���ý��� ������ȯ�ǰ� 
	// ����Ʈ -> 
	// ������ȯ ������ ����
	// ���ð���  ->                ���� ����               -> �� ����    -> ���� ����  -> �����
	// ����      -> ũ�� -> ���� -> �̵� -> ���� -> �θ�    -> ��(ī�޶�) ->   ����     -> ����Ʈ(�����)
	// x 100 y 100
	// 0 == ax + b + y;
	// x y
	// ����1
	// ������ �������� �̿��Ͽ�
	// ������ �����ִ��� �� �Ʒ��� �����ִ��� �˾Ƴ��� ���� ����� �Ͷ�.
	//if (true)
	//{
	//	TextOut(m_hDC, 100, 100, L"True", lstrlen(L"True"));
	//}
	//else {
	//	TextOut(m_hDC, 100, 100, L"false", lstrlen(L"True"));
	//}

	// ������Ʈ ���� (��)
	Time::Update();
	Input::Update();
	Scene::Scene_Update();
	SoundResource::SoundUpdate();
	SoundResource::SoundBGMUpdate();
	// ������Ʈ���� ������Ʈ
	// �׸��� 
	// ���� ������ �������Ҷ� �����Ͷ����� ���󹹶� �߸�
	// �����Ͷ����� ����°�.
	// ���� �����.(���ؽ� �����)
	// ���� �̵���Ű��(���� ���)
	// ī�޶� ��� �����(�� ���)
	// ������� �����

	//Device::ClearTarget();

	// �����ϱ�
	//Scene::Scene_Render();
	// ������ȯ�� �ϱ����� ������ ������Ʈ���� �غ�.

	// ������ȯ(���ؽ� ���̴����� �̷������.)
	// �����Ͷ�����(�ȼ��� �����ϰ�)
	// �ȼ�ó��(�ȼ� ���̴����� ����� �ȼ��� ó���Ѵ�.)
	// OutPut������ �غ�Ǿ��ִ� ����Ÿ�ٿ� �� �ȼ��� �׷��ش�.

	// output�������� ����Ʈ�� ���� ����Ʈ ���

	//RenderManager::Render();

	//Device::SwapChain_Present();

	// ������
	
	
}

void Window::Size(Vector2_Int _Size)
{
	m_Size = _Size;

	RECT Rect = { 0, 0, _Size.ix, _Size.iy };

	// �� �����쿡 �ٻ���� ����� ũ�⸦ ����.
	// �� ������ ���� ������ �־��ֱ⿡ ũ�� ���Ҷ� ����ؾ��Ѵ�.

	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, SWP_NOZORDER | SWP_NOMOVE);
}
