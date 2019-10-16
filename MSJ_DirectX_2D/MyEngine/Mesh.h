#pragma once
#include "Resource.h"
#include "DXHeader.h"

class Mesh : public Resource
{
private:
	// 만들기 전용 함수 객체 생성하기
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
	// 버텍스 버퍼 만들기
	ID3D11Buffer* m_VertexBuffer;

	D3D11_PRIMITIVE_TOPOLOGY m_DrawMode;			// 그리기 방법 ( 선으로 그릴건지, 면으로 그릴건지 아니면 그외 다른방법으로 그릴건지)
	UINT m_VertexSize;								// 버텍스 사이즈 sizeof(COLORVertex)
	UINT m_VertexCount;								// 버텍스 개수 4


	// 인덱스 버퍼 만들기
	ID3D11Buffer* m_IndexBuffer;
	DXGI_FORMAT m_IndexBufferFormat;			// 인덱스 버퍼 포맷 WORD
	UINT m_IndexSize;							// 인덱스 사이즈
	UINT m_IndexCount;							// 인덱스 버퍼 원소 개수

public:
	void Update();
	void Render();

private:
	Mesh();
	~Mesh();
};

