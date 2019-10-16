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
	UINT OffSet = 0;		// ��� ���� �׸��ǰ�,.. IASetVertexBuffers() �Ŀ��� ���� ������ �׷ȴ����� ���� ����

	// ���ؽ� ����, �ε��� ������ ������ �׷��޶�� ���ؽ�Ʈ�� ������ �Ѱ������.
	// ���ؽ� ���ۿ� �ε��� ������ ����

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
	Device::Get_Context()->IASetPrimitiveTopology(m_DrawMode);				// ��� �׸��ž�?? ( ��, �� , �ﰢ�� �߿�.. )
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


	// ���ؽ� ���۸����
	D3D11_BUFFER_DESC VertexDesc = D3D11_BUFFER_DESC();
	VertexDesc.ByteWidth = _VertexSize * _VertexCount;
	VertexDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == VertexDesc.Usage)
	{
		VertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	VertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// ������� ������ ����. ���� ���ؽ��� ����� ���� ������ �޸𸮸� �����������.

	D3D11_SUBRESOURCE_DATA VertexSub = D3D11_SUBRESOURCE_DATA();
	VertexSub.pSysMem = _VertexMem;



	// ���� �����~
	if (S_OK != Device::Get_Device()->CreateBuffer(&VertexDesc, &VertexSub, &m_VertexBuffer))
	{
		EAssert(true);
		return false;
	}

	// �ε��� ���۸� ������~!
	D3D11_BUFFER_DESC IndexDecs = D3D11_BUFFER_DESC();
	IndexDecs.ByteWidth = _IndexCount * _IndexSize;

	IndexDecs.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;		// �ʷ�å p.39 ǥ,  ���̳����� CPU�� ���� GPU���� �д´�.

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == IndexDecs.Usage)
	{
		// CPUAccessFlags �� CPU ���� �÷���
		// ���̳����� CPU�� �ڿ��� �����ϰ� GPU�� �װ��� �Һ���...
		// CPU�� �ڿ��� ������ ���⶧���� WRITE�� ������ CPU�� �ڿ��� ���� ���Ѵ�!! �׷��Ƿ� READ�� �ƴ�
		IndexDecs.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  // D3D11_USAGE�� �������� �������� ������ ����ؼ� ���־���Ѵ�.
	}

	IndexDecs.BindFlags = D3D11_BIND_INDEX_BUFFER;			// �����÷��� �ʷ�å p.41

	D3D11_SUBRESOURCE_DATA IndexSub = D3D11_SUBRESOURCE_DATA();
	IndexSub.pSysMem = _IndexMem;

	if (S_OK != Device::Get_Device()->CreateBuffer(&IndexDecs, &IndexSub, &m_IndexBuffer))
	{
		EAssert(true);
		return false;
	}

	return true;
}