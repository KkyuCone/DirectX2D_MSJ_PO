#pragma once
#include "DXHeader.h"

class VertexColor
{
public:
	Vector4 Position;
	VCOLOR Color;

	// 이 클래스 크기 반환
	static UINT TypeSize()
	{
		return (UINT)sizeof(VertexColor);
	}
};


// Uv
class UvVertexColor
{
public:
	Vector4 Position;
	VCOLOR Color;
	Vector2 Uv;

	// 이 클래스 크기 반환
	static UINT TypeSize()
	{
		return (UINT)sizeof(UvVertexColor);
	}
};

// Texture
class TextureVertex
{
public:
	Vector4 Position;
	Vector2 Uv;

	static UINT TypeSize()
	{
		return (UINT)sizeof(TextureVertex);
	}
};

// VertexColor[4]

// 16바이트..
// 인덱서 버퍼 클래스
// 컴퓨터는 바보라서 'ㅅ' 하나하나 다 알려줘야됨~,  예를들어 점 3개가 있는데 이걸 삼각형으로 안만들어줌 !!
// -> 그래서 하나하나 알려줘야함..
class IDX16
{
public:
	// + 변수이름에 _(언더바)는 사용할 수 있고 그 외 특수문자는 사용 불가능하다.
	WORD _1, _2, _3;

	// 원소 하나 크기를 리턴해줌.
	static UINT MemberSize()
	{
		return (UINT)sizeof(WORD);
	}

	static DXGI_FORMAT Format()
	{
		return DXGI_FORMAT_R16_UINT;
	}

public:
	IDX16() : _1(0), _2(0), _3(0) {}
	IDX16(WORD __1, WORD __2, WORD __3) : _1(__1), _2(__2), _3(__3) {}
};

class IDX32
{
public:
	DWORD _1, _2, _3;

	UINT MemberSize()
	{
		return (UINT)sizeof(DWORD);
	}
};
