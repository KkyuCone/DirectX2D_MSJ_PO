#pragma once
#include "DXHeader.h"
#include "GameFont.h"
#include <list>

class GameDebug
{
public:
	static std::list<std::wstring> m_AllMessage;

public:
	// static void PushMsg(const wchar_t* _Str, float _fSize, HVEC2 _Pos, HVEC4 _Color, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_TOP);
	// static void DrawDebug();
public:
	static void DrawStr(const wchar_t* _String, float _fSize, Vector2 _Position, Vector4 _Color, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_TOP);

	//static void DrawRect(Vector4 _Pos, Vector4 _Scale, Vector4 _Color = Vector4::Red, float _Angle = 0.0f);
	//static void DrawRect(My_PTR<Mesh>)
public:
	GameDebug();
	~GameDebug();
};

