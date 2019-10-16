#pragma once
#include "Resource.h"
#include "DXHeader.h"

class Mesh : public Resource
{
private:
	// ����� ���� �Լ� ��ü �����ϱ�
	int a = 0;
	DEFINITION_CREATOR(Mesh, std::wstring)
		DEFINITION_CREATOR_FUCTION(UINT _VertexCount, UINT _VertexSize, D3D11_USAGE _VertexUsage, void* _VertexMem, UINT _IndexCount, UINT _IndexSize, D3D11_USAGE _IndexUsage, void* _IndexMem, DXGI_FORMAT _IndexFormat, D3D11_PRIMITIVE_TOPOLOGY _DrawMode = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		DEFINITION_CREATOR_FUNCTION_PARAMETER(_VertexCount, _VertexSize, _VertexUsage, _VertexMem, _IndexCount, _IndexSize, _IndexUsage, _IndexMem, _IndexFormat, _DrawMode)
		DEFINITION_CREATOR_END

public:
	bool Create(UINT _VertexCount, UINT _VertexSize, D3D11_USAGE _VertexUsage, void* _VertexMem, UINT _IndexCount, UINT _IndexSize, D3D11_USAGE _IndexUsage, void* _IndexMem, DXGI_FORMAT _IndexFormat, D3D11_PRIMITIVE_TOPOLOGY _DrawMode = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//class Creator
	//{
	//public:
	//	My_PTR<Mesh> operator()(UINT _VertexCount, UINT _VertexSize, D3D11_USAGE _VertexUsage, void* _VertexMem
	//		, UINT _IndexCount, UINT _IndexSize, D3D11_USAGE _IndexUsage, void* _IndexMem, DXGI_FORMAT _IndexFormat
	//		, D3D11_PRIMITIVE_TOPOLOGY _DrawMode = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//};

private:
	// ���ؽ� ���� �����
	ID3D11Buffer* m_VertexBuffer;

	D3D11_PRIMITIVE_TOPOLOGY m_DrawMode;			// �׸��� ��� ( ������ �׸�����, ������ �׸����� �ƴϸ� �׿� �ٸ�������� �׸�����)
	UINT m_VertexSize;								// ���ؽ� ������ sizeof(COLORVertex)
	UINT m_VertexCount;								// ���ؽ� ���� 4


	// �ε��� ���� �����
	ID3D11Buffer* m_IndexBuffer;
	DXGI_FORMAT m_IndexBufferFormat;			// �ε��� ���� ���� WORD
	UINT m_IndexSize;							// �ε��� ������
	UINT m_IndexCount;							// �ε��� ���� ���� ����

public:
	void Update();
	void Render();

private:
	Mesh();
	~Mesh();
};

