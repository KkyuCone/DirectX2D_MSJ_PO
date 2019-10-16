#pragma once
#include <Windows.h>
#include "DXHeader.h"

class Core;
class Window
{
public:
	friend Core;

private:
	static HINSTANCE m_hInst;
	static HWND m_hWnd;
	static HDC m_hDC;
	static Vector2_Int m_Size;
	static bool m_bFullSize;
	
public:
	static bool m_bIsTool;
	static int m_MouseWheelValue;

public:
	static HWND WindowHWND()
	{
		return m_hWnd;
	}

	static bool IsSceneFullSize()
	{
		return m_bFullSize;
	}

private:
	static void My_RegisterClass();
	static bool My_CreateWindow(const wchar_t* _WindowName);
	
	static void Init(const wchar_t* _WindowName);
	static void Init(HWND _hWnd, const wchar_t* _WindowName);
	static LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

	// 윈도우 크기를 변경해줄 함수
	static void WindowSize_Change(int _x, int _y);

public:
	// Progress 추가~
	static void Progress();

public:
	static void Size(Vector2_Int _Size);
	static Vector2_Int Size() { return m_Size; }
	static Vector2 FloatSize() { return m_Size.ConvertToFloat(); }

private:
	Window() {};
	Window(HWND _hWnd) {};
	~Window() {};
};

