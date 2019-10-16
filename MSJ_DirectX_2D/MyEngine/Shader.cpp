#include "Shader.h"
#include "Device.h"



////////////////////////////////////// InputLayout
Shader::InputLayout::InputLayout(Shader* _Shader) : m_Offset(0), m_pShader(_Shader), m_pLayout(nullptr)
{

}

Shader::InputLayout::~InputLayout()
{
	SAFE_RELEASE(m_pLayout);
}

void Shader::InputLayout::Layout_Add(const char* _LayoutName, UINT _Index, DXGI_FORMAT _Format, UINT _InputSlot, UINT _InstanceDataStepRate,
	D3D11_INPUT_CLASSIFICATION _Input)
{
	D3D11_INPUT_ELEMENT_DESC InputLayout;

	// POSITION + COLOR 
	// ���ؽ� ���۴� ���ؽ����� ������ ����(ũ��, �����..)�� ���� �ֱ⶧���� 
	// Layout�� ���ؼ� �긦 ��� �ؾ��ϴ��� �˷��ش�.( ��ġ, ��𼭺��� �ؾ��ϴ���.. �� ) -> ���ؾ��ϴ����� ���� ������ �߰�����.


	// ���� ��� �ﰢ���� �׸��� ���ؼ� 3���� ���� GPU�� �������ϴµ�
	// D3D11���� ��ġ ���� ���ؽ� ������ ���� ���ҽ�(���ؽ� ����)�� ����. 
	// ���ؽ� ������ �����ϴµ� ���Ǵ� ���۴� ���ؽ����۶�� �Ҹ�.
	//
	// ���ؽ��� ��ġ ������ �����µ� �븻, Į��, �ؽ�ó��ǥ �� �� ���� �� �ִ�.
	// ���ؽ� ���̾ƿ��� �� �Ӽ����� �޸𸮿� ������ ����� ���� ��.
	//
	// GPU���� ���ؽ� ������ ������ ���ؽ� ���۸� ������ �� ���� �޸� ����� ������. 
	// ��� GPU���� ������ ��Ȯ�� �Ӽ��� �����ϱ� ���ؼ� ���ؽ� ���̾ƿ��� �˾ƾ� ��.
	// �̸� ���� Input layout�� �����.
	//
	// Input layout�� GPU���ٰ� ���ؽ� ����ü�� ���̾ƿ��� �����ϴ� D3D������Ʈ ��.
	// ���ؽ� �Ӽ��� D3D11_INPUT_ELEMENT_DESC ����ü�� ���� �Ǿ�����.
	// D3D11_INPUT_ELEMENT_DESC ����ü�� �Ʒ��� ������ҵ�� �̷�����.
	//
	// SemanticName : �ø�ƽ �̸��� ��ҹ��ڸ� �������� ����. �� ��Ҵ� �ڽ��� ���ϴ°���.. ������ �����ϴ� ���ڿ��� ���.
	//
	// SemanticIndex : �ø�ƽ �̸��� ����. ���ؽ��� ���� �Ӽ��� ������ ���� �� ����.
	//					���� ��� �ø�ƽ �̸��� POSITION0, POSITION1ó�� �߰��ϴ� ��� 2���� ������ �ø�ƽ �̸� POSITION�� ����ϰ�
	//					���� �ٸ� �ø�ƽ �ε����� 0, 1�� ����
	//
	// *Format       :  �� ��ҿ��� ����ϴ� ������Ÿ���� ����.
	//					DXGI_FORMAT_R32G32B32_FLOAT�� ��쿡 3���� 32��Ʈ �ε��Ҽ����� ����.
	//					�� ��Ҵ� �� 12����Ʈ�� ������ ( 32��Ʈ(4����Ʈ) * 3(����,�׸�,���))
	//
	// InputSlot    : D3D11 ���ø����̼��� ���ؽ������͸� ���ؽ����۸� ���ؼ� GPU�� �ѱ��.
	//				  D3D11���� ���ÿ� �������� ���ؽ� ���۰� GPU�� ���۵� �� �ִ�. ( 16������ )
	//				  ������ ���ؽ����۴� 0~15�� ������ �Է½��� ���ڿ� ���ε��.
	//				  InputSlot�ʵ�� GPU ���� �� ��Ҹ� � ���ؽ����۷κ��� �����;��ϴ����� ���� �˷���.
	//			
	// *AlignedByteOffset : ���ؽ��� ���ؽ� ���ۿ� ����Ǵµ� �̰� �޸� ����� ����.
	//					   ���⼭ �̰��� GPU���� �� ��Ұ� �޸𸮵�� ������������ �󸶳� ������ ��ġ�� �ִ����� ���� �˷���.
	//
	// InputSlotClass : ��� �Ϲ������� D3D11_INPUT_PER_VERTEX_DATA ���� ����. 
	//				    ���� ���ø����̼��� �ν��Ͻ��� ����Ѵٸ� D3D11_INPUT_PER_INSTANCE_DATA�� �����ؾ���.
	//					������ �ؾ� ���ؽ����۰� �ν��Ͻ������͸� �����Ѵٰ� �˷��� �� �ֱ� ����.
	//
	// InstanceDataStepRate : �̰� ���� �ν��Ͻ��� ���� ���Ǵµ� ������� ������ 0���� �����ϸ� �ȴ�.

	InputLayout.SemanticName = _LayoutName;
	InputLayout.SemanticIndex = _Index;
	InputLayout.Format = _Format;
	InputLayout.InputSlot = _InputSlot;
	InputLayout.InputSlotClass = _Input;
	InputLayout.InstanceDataStepRate = _InstanceDataStepRate;
	InputLayout.AlignedByteOffset = m_Offset;
	m_InputLayout.push_back(InputLayout);

	UINT LayoutSize = Sizeof_Format(InputLayout.Format);

	if (0 == LayoutSize)
	{
		EAssert(true);
		return;
	}

	m_Offset += LayoutSize;
}

void Shader::InputLayout::Layout_Create()
{
	int a = 0;
	// Input Layout�� �����.
	// CreateInputLayout(
	if (S_OK != Device::Get_Device()->CreateInputLayout(&m_InputLayout[0], (UINT)m_InputLayout.size(), m_pShader->Get_Blob()->GetBufferPointer(), m_pShader->Get_Blob()->GetBufferSize(), &m_pLayout))
	{
		EAssert(true);
		return;
	}
}

void Shader::InputLayout::Update()
{
	Device::Get_Context()->IASetInputLayout(m_pLayout);
}


/////////////////////////////////////////////////////////////  Shader

Shader::Shader(EShader_Type _eShaderType)
	: m_eShaderType(_eShaderType), m_pBlob(nullptr), m_pErrorBlob(nullptr), m_VersionHLSLFirst(5), m_VersionHLSLSecond(0)
{

}


Shader::~Shader()
{
	SAFE_RELEASE(m_pBlob);
	SAFE_RELEASE(m_pErrorBlob);
}


void Shader::Update()
{
	if (nullptr != m_Layout)
	{
		m_Layout->Update();
	}
}


void Shader::Layout_Add(const char* _Name, UINT _Index, DXGI_FORMAT _Format, UINT _InputSlot, UINT _InstanceDataStepRate,
	D3D11_INPUT_CLASSIFICATION _Input)
{
	if (nullptr == m_Layout)
	{
		m_Layout = new InputLayout(this);
	}

	m_Layout->Layout_Add(_Name, _Index, _Format, _InputSlot, _InstanceDataStepRate, _Input);
}

void Shader::Layout_End(const char* _Name, UINT _Index, DXGI_FORMAT _Format, UINT _InputSlot, UINT _InstanceDataStepRate,
	D3D11_INPUT_CLASSIFICATION _Input)
{
	if (nullptr == m_Layout)
	{
		m_Layout = new InputLayout(this);
	}

	m_Layout->Layout_Add(_Name, _Index, _Format, _InstanceDataStepRate, _Input);

	m_Layout->Layout_Create();
}