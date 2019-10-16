#include "VertexShader.h"
#include "Device.h"



VertexShader::VertexShader() : Shader(EShader_Type::ShaderType_Vertex), m_pVertexShader(nullptr)
{
}


VertexShader::~VertexShader()
{
	SAFE_RELEASE(m_pVertexShader);
}

bool VertexShader::Load(UINT _VersionFirst, UINT _VersionSecond, const char* _FunctionName)
{
	// 파일 경로 
	//std::wstring Resource = Path::FindPath(L"Shader");
	//Resource += L"ColorRect.fx";
	int a = 0;

	m_VersionHLSLFirst = _VersionFirst;
	m_VersionHLSLSecond = _VersionSecond;

	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif // _DEBUG

	char szBuffer[256] = {};
	wsprintfA(szBuffer, "vs_%d_%d", m_VersionHLSLFirst, m_VersionHLSLSecond);

	//
	// D3DCompileFromFile함수 : 지정된 대상에 대한 HLSL 코드를 바이트 코드로 컴파일
	if (S_OK != D3DCompileFromFile(GetPath().c_str(),			// 셰이더 코드가 포함된 파일의 이름을 포함하는 상수 null로 끝나는 문자열에 대한 포인터
		nullptr,												// 셰이더 매크로를 정의하는 D3D_SHADER_MACRO  구조체의 옵션의 배열, 사용되지 않으면 null
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_FunctionName,											// 셰이더 실행이 시작되는 셰이더 진입 함수 이름
		szBuffer,
		iFlag,
		0,
		&m_pBlob,
		&m_pErrorBlob))
	{
		std::string ErrorText = (char*)m_pErrorBlob->GetBufferPointer();
		EAssert(true);
		return false;
	}


	if (S_OK != Device::Get_Device()->CreateVertexShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		nullptr,
		&m_pVertexShader))
	{
		EAssert(true);
		return false;
	}

	return true;
}


void VertexShader::Update()
{
	Shader::Update();
	Device::Get_Context()->VSSetShader(m_pVertexShader, 0, 0);
}
