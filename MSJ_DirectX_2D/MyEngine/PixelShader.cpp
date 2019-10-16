#include "PixelShader.h"
#include "Device.h"



PixelShader::PixelShader() : Shader(EShader_Type::ShaderType_Pixel), m_pPixelShader(nullptr)
{
}


PixelShader::~PixelShader()
{
	SAFE_RELEASE(m_pPixelShader);
}

bool PixelShader::Load(UINT _VersionFirst, UINT _VersionSecond, const char* _FunctionName)
{
	//std::wstring Resource = Path::FindPath(L"Shader");
	//Resource += L"ColorRect.fx";
	int a = 0;

	m_VersionHLSLFirst = _VersionFirst;
	m_VersionHLSLSecond = _VersionSecond;

	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif // _DEBUG

	// 쉐이더(HLSL) 컴파일 버전
	char szBuffer[256] = {};
	wsprintfA(szBuffer, "ps_%d_%d", m_VersionHLSLFirst, m_VersionHLSLSecond);

	// 첫번째 인자 패스(경로)
	// 두번째 인자 define(쉐이더 코드 안에서도 ifdef를 사용할수 있다. 근데 보통은 안쓰므로)
	// D3D_COMPILE_STANDARD_FILE_INCLUDE 쉐이더안에서도 include를 사용할수 있다.

	int b = 0;

	if (S_OK != D3DCompileFromFile(GetPath().c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_FunctionName, szBuffer, iFlag, 0, &m_pBlob, &m_pErrorBlob))
	{
		std::string ErrorText = (char*)m_pErrorBlob->GetBufferPointer();
		EAssert(true);
		return false;
	}

	if (S_OK != Device::Get_Device()->CreatePixelShader(
		m_pBlob->GetBufferPointer(),		// BLOB의 데이터의 포인터를 얻어 옴
		m_pBlob->GetBufferSize(),			// BLOB의 데이터 사이즈를 바이트 단위로 가져옴
		nullptr,
		&m_pPixelShader))
	{
		EAssert(true);
		return false;
	}

	return true;
}

void PixelShader::Update()
{
	Shader::Update();


	Device::Get_Context()->PSSetShader(m_pPixelShader, 0, 0);
}
