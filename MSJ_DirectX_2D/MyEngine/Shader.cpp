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
	// 버텍스 버퍼는 버텍스들의 간단한 정보(크기, 몇개인지..)만 갖고 있기때문에 
	// Layout을 통해서 얘를 어떻게 해야하는지 알려준다.( 위치, 어디서부터 해야하는지.. 등 ) -> 뭘해야하는지에 대한 정보를 추가해줌.


	// 예를 들어 삼각형을 그리기 위해서 3개의 점을 GPU로 보내야하는데
	// D3D11에서 위치 값은 버텍스 정보는 버퍼 리소스(버텍스 버퍼)에 저장. 
	// 버텍스 정보를 저장하는데 사용되는 버퍼는 버텍스버퍼라고 불림.
	//
	// 버텍스는 위치 정보를 가지는데 노말, 칼라, 텍스처좌표 등 도 가질 수 있다.
	// 버텍스 레이아웃은 이 속성들이 메모리에 놓여진 방법을 정의 함.
	//
	// GPU에게 버텍스 정보를 보관한 버텍스 버퍼를 전송할 때 보통 메모리 덩어리로 전송함. 
	// 고로 GPU에서 버퍼의 정확한 속성을 추출하기 위해서 버텍스 레이아웃을 알아야 함.
	// 이를 위해 Input layout을 사용함.
	//
	// Input layout는 GPU에다가 버텍스 구조체의 레이아웃을 설명하는 D3D오브젝트 임.
	// 버텍스 속성은 D3D11_INPUT_ELEMENT_DESC 구조체로 구성 되어있음.
	// D3D11_INPUT_ELEMENT_DESC 구조체는 아래의 구성요소들로 이뤄졌다.
	//
	// SemanticName : 시맨틱 이름은 대소문자를 구분하지 않음. 이 요소는 자신이 뭘하는건지.. 목적을 설명하는 문자열을 담당.
	//
	// SemanticIndex : 시맨틱 이름을 보충. 버텍스는 동일 속성을 여러개 가질 수 있음.
	//					예를 들어 시맨틱 이름에 POSITION0, POSITION1처럼 추가하는 대신 2개는 동일한 시맨틱 이름 POSITION을 사용하고
	//					서로 다른 시맨틱 인덱스인 0, 1를 가짐
	//
	// *Format       :  이 요소에서 사용하는 데이터타입을 정의.
	//					DXGI_FORMAT_R32G32B32_FLOAT의 경우에 3개의 32비트 부동소수점을 가짐.
	//					이 요소는 총 12바이트의 길이임 ( 32비트(4바이트) * 3(레드,그린,블루))
	//
	// InputSlot    : D3D11 어플리케이션은 버텍스데이터를 버텍스버퍼를 통해서 GPU로 넘긴다.
	//				  D3D11에선 동시에 여러개의 버텍스 버퍼가 GPU로 전송될 수 있다. ( 16개까지 )
	//				  각각의 버텍스버퍼는 0~15개 까지의 입력슬롯 숫자에 바인드됨.
	//				  InputSlot필드는 GPU 에게 이 요소를 어떤 버텍스버퍼로부터 가져와야하는지에 대해 알려줌.
	//			
	// *AlignedByteOffset : 버텍스는 버텍스 버퍼에 저장되는데 이건 메모리 덩어리일 뿐임.
	//					   여기서 이값은 GPU에게 이 요소가 메모리덩어리 시작지점부터 얼마나 떨어진 위치에 있는지에 대해 알려줌.
	//
	// InputSlotClass : 얘는 일반적으로 D3D11_INPUT_PER_VERTEX_DATA 값을 가짐. 
	//				    만약 어플리케이션이 인스턴싱을 사용한다면 D3D11_INPUT_PER_INSTANCE_DATA로 설정해야함.
	//					설정을 해야 버텍스버퍼가 인스턴스데이터를 포함한다고 알려줄 수 있기 때문.
	//
	// InstanceDataStepRate : 이거 또한 인스턴싱을 위해 사용되는데 사용하지 않으면 0으로 설정하면 된다.

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
	// Input Layout을 만든다.
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