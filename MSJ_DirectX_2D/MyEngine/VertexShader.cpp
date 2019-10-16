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
	// ���� ��� 
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
	// D3DCompileFromFile�Լ� : ������ ��� ���� HLSL �ڵ带 ����Ʈ �ڵ�� ������
	if (S_OK != D3DCompileFromFile(GetPath().c_str(),			// ���̴� �ڵ尡 ���Ե� ������ �̸��� �����ϴ� ��� null�� ������ ���ڿ��� ���� ������
		nullptr,												// ���̴� ��ũ�θ� �����ϴ� D3D_SHADER_MACRO  ����ü�� �ɼ��� �迭, ������ ������ null
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_FunctionName,											// ���̴� ������ ���۵Ǵ� ���̴� ���� �Լ� �̸�
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
