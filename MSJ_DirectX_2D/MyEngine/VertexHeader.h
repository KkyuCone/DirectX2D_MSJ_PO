#pragma once
#include "DXHeader.h"

class VertexColor
{
public:
	Vector4 Position;
	VCOLOR Color;

	// �� Ŭ���� ũ�� ��ȯ
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

	// �� Ŭ���� ũ�� ��ȯ
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

// 16����Ʈ..
// �ε��� ���� Ŭ����
// ��ǻ�ʹ� �ٺ��� '��' �ϳ��ϳ� �� �˷���ߵ�~,  ������� �� 3���� �ִµ� �̰� �ﰢ������ �ȸ������ !!
// -> �׷��� �ϳ��ϳ� �˷������..
class IDX16
{
public:
	// + �����̸��� _(�����)�� ����� �� �ְ� �� �� Ư�����ڴ� ��� �Ұ����ϴ�.
	WORD _1, _2, _3;

	// ���� �ϳ� ũ�⸦ ��������.
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
