#include "Mesh.h"
#include "Device.h"


Mesh::Mesh() : m_VertexBuffer(nullptr), m_IndexBuffer(nullptr)
{
}


Mesh::~Mesh()
{
	SAFE_RELEASE(m_VertexBuffer);
	SAFE_RELEASE(m_IndexBuffer);
}

////////////////////////////////////////////////////////////////////////////////////////


void Mesh::Update()
{
	int a = 0;
	UINT OffSet = 0;		// 어디서 부터 그릴건가,.. IASetVertexBuffers() 후에는 내가 어디까지 그렸는지에 대해 나옴

	// 버텍스 버퍼, 인덱스 버퍼의 내용을 그려달라고 컨텍스트에 정보를 넘겨줘야함.
	// 버텍스 버퍼와 인덱스 버퍼의 세팅

	ID3D11DeviceContext* m_pC = Device::Get_Context();

	if (this == nullptr) 
	{
		int a = 0;
	}


	if (m_VertexBuffer == nullptr)
	{
		int r = 0;
	}

	ID3D11Buffer* b = m_VertexBuffer;

	UINT c = m_VertexSize;
	UINT d = OffSet;


	Device::Get_Context()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &m_VertexSize, &OffSet);
	Device::Get_Context()->IASetPrimitiveTopology(m_DrawMode);				// 어떻게 그릴거얌?? ( 점, 선 , 삼각형 중에.. )
	Device::Get_Context()->IASetIndexBuffer(m_IndexBuffer, m_IndexBufferFormat, 0);
}

void Mesh::Render()
{
	Device::Get_Context()->DrawIndexed(m_IndexCount, 0, 0);
}


bool Mesh::Create(UINT _VertexCount, UINT _VertexSize, D3D11_USAGE _VertexUsage, void* _VertexMem
	, UINT _IndexCount, UINT _IndexSize, D3D11_USAGE _IndexUsage, void* _IndexMem
	, DXGI_FORMAT _IndexFormat, D3D11_PRIMITIVE_TOPOLOGY _DrawMode)
{
	int a = 0;
	m_VertexSize = _VertexSize;
	m_VertexCount = _VertexCount;
	m_IndexSize = _IndexSize;
	m_IndexCount = _IndexCount;
	m_IndexBufferFormat = _IndexFormat;
	m_DrawMode = _DrawMode;


	// 버텍스 버퍼만들기
	D3D11_BUFFER_DESC VertexDesc = D3D11_BUFFER_DESC();
	VertexDesc.ByteWidth = _VertexSize * _VertexCount;
	VertexDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == VertexDesc.Usage)
	{
		VertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	VertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// 현재까지 정보만 만듦. 이제 버텍스를 만들기 위해 참조할 메모리를 지정해줘야함.

	D3D11_SUBRESOURCE_DATA VertexSub = D3D11_SUBRESOURCE_DATA();
	VertexSub.pSysMem = _VertexMem;



	// 버퍼 만들기~
	if (S_OK != Device::Get_Device()->CreateBuffer(&VertexDesc, &VertexSub, &m_VertexBuffer))
	{
		EAssert(true);
		return false;
	}

	// 인덱스 버퍼를 만들어보장~!
	D3D11_BUFFER_DESC IndexDecs = D3D11_BUFFER_DESC();
	IndexDecs.ByteWidth = _IndexCount * _IndexSize;

	IndexDecs.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;		// 초록책 p.39 표,  다이나믹은 CPU가 쓰고 GPU에서 읽는다.

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == IndexDecs.Usage)
	{
		// CPUAccessFlags 는 CPU 접근 플래그
		// 다이나믹은 CPU가 자원을 생산하고 GPU가 그것을 소비함...
		// CPU가 자원의 내용을 쓰기때문에 WRITE로 해줬음 CPU는 자원을 읽지 못한다!! 그러므로 READ가 아님
		IndexDecs.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  // D3D11_USAGE의 값에따라 읽은건지 쓸건지 고려해서 값넣어야한다.
	}

	IndexDecs.BindFlags = D3D11_BIND_INDEX_BUFFER;			// 연결플래그 초록책 p.41

	D3D11_SUBRESOURCE_DATA IndexSub = D3D11_SUBRESOURCE_DATA();
	IndexSub.pSysMem = _IndexMem;

	if (S_OK != Device::Get_Device()->CreateBuffer(&IndexDecs, &IndexSub, &m_IndexBuffer))
	{
		EAssert(true);
		return false;
	}

	return true;
}