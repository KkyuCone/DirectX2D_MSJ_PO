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

// 윈도우 크기를 변경해주는 함수 (가로, 세로)
void Window::WindowSize_Change(int _x, int _y)
{
	SetWindowPos(m_hWnd, nullptr, 0, 0, _x, _y, SWP_NOZORDER | SWP_NOMOVE);
}

// Progress
void Window::Progress()
{
	// 로컬공간 -> 매쉬
	// 월드공간 -> 월드
	// 뷰행렬 -> 뷰공간
	// 투영 -> 0~1f의 값으로 모든 버택스가 공간변환되고 
	// 뷰포트 -> 
	// 공간변환 파이프 라인
	// 로컬공간  ->                월드 공간               -> 뷰 공간    -> 투영 공간  -> 모니터
	// 로컬      -> 크기 -> 자전 -> 이동 -> 공전 -> 부모    -> 뷰(카메라) ->   투영     -> 뷰포트(모니터)
	// x 100 y 100
	// 0 == ax + b + y;
	// x y
	// 숙제1
	// 직선의 방정식을 이용하여
	// 선위에 점이있는지 선 아래에 점이있는지 알아내는 공식 만들어 와라.
	//if (true)
	//{
	//	TextOut(m_hDC, 100, 100, L"True", lstrlen(L"True"));
	//}
	//else {
	//	TextOut(m_hDC, 100, 100, L"false", lstrlen(L"True"));
	//}

	// 업데이트 구조 (씬)
	Time::Update();
	Input::Update();
	Scene::Scene_Update();
	SoundResource::SoundUpdate();
	SoundResource::SoundBGMUpdate();
	// 오브젝트들의 업데이트
	// 그리고 
	// 내일 할일은 랜더링할때 레스터라이저 뭐라뭐라 뜨면
	// 레스터라이저 만드는것.
	// 판자 만들기.(버텍스 만들기)
	// 판자 이동시키기(월드 행렬)
	// 카메라 행렬 만들기(뷰 행렬)
	// 투영행렬 만들기

	//Device::ClearTarget();

	// 랜더하기
	//Scene::Scene_Render();
	// 공간변환을 하기위한 정보만 업데이트에서 준비.

	// 공간변환(버텍스 쉐이더에서 이루어진다.)
	// 레스터라이저(픽셀을 추출하고)
	// 픽셀처리(픽셀 쉐이더에서 추출된 픽셀을 처리한다.)
	// OutPut머저에 준비되어있는 랜더타겟에 그 픽셀을 그려준다.

	// output머저에서 뷰포트를 통한 뷰포트 행렬

	//RenderManager::Render();

	//Device::SwapChain_Present();

	// 릴리즈
	
	
}

void Window::Size(Vector2_Int _Size)
{
	m_Size = _Size;

	RECT Rect = { 0, 0, _Size.ix, _Size.iy };

	// 이 윈도우에 바사이즈가 고려된 크기를 리턴.
	// 바 사이즈 쪽을 음수로 넣어주기에 크기 정할때 고려해야한다.

	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, SWP_NOZORDER | SWP_NOMOVE);
}
