#pragma once
#include "Resource.h"
#include "DXHeader.h"
#include "STLHelper.h"
#include "ConstBuffer.h"

class Shader :public Resource
{
public:
	static UINT Sizeof_Format(DXGI_FORMAT _Format);

public:
	enum EShader_Type
	{
		ShaderType_Vertex,
		ShaderType_Pixel
	};

public:
	class InputLayout : public ReferenceBase
	{
	private:
		Shader* m_pShader;
		UINT m_Offset;
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_InputLayout;
		ID3D11InputLayout* m_pLayout = nullptr;

	public:
		void Layout_Add(const char* _LayoutName, UINT _Index, DXGI_FORMAT _Format, UINT _InputSlot, UINT _InstanceDataStepRate, 
			D3D11_INPUT_CLASSIFICATION _Input = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA);

		void Layout_Create();
		void Update();

	public:
		InputLayout(Shader* _Shader);
		~InputLayout();
	};

protected:
	EShader_Type m_eShaderType;

	// ID3DBlob은 데이터 버퍼로 사용 가능.
	// 메쉬 최적화 및 로드 작업을 수행하는 동안 정점, 인접성 같은 정보를 저장하는데 사용 가능.
	// 또는 이런 객체는 정점, 지오메트리, 픽셀 쉐이더를 컴파일하는 API에서 객체 코드 및 오류 메시지를 반환하는데 사용됨.

	ID3DBlob* m_pBlob;					// 쉐이더 코드를 컴파일해서 디바이스에게 넘겼을때 디바이스가 주는 포인터
	ID3DBlob* m_pErrorBlob;				// 쉐이더 코드 컴파일이 실패했을때 디바이스가 에러의 내용을 넘겨줄 포인터

	UINT m_VersionHLSLFirst;			// HLSL버전을 담당. (첫번째 자리수),    HLSL -> 1~4는 DX9까지 사용, 11부터는 5도 제공
	UINT m_VersionHLSLSecond;

	My_PTR<InputLayout> m_Layout;

public:
	ID3DBlob* Get_Blob()
	{
		return m_pBlob;
	}

public:
	virtual void Update() = 0;

public:
	void Layout_Add(const char* _Name, UINT _Index, DXGI_FORMAT _Format, UINT _InputSlot, UINT _InstanceDataStepRate, 
		D3D11_INPUT_CLASSIFICATION _Input = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA);

	void Layout_End(const char* _Name, UINT _Index, DXGI_FORMAT _Format, UINT _InputSlot, UINT _InstanceDataStepRate,
		D3D11_INPUT_CLASSIFICATION _Input = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA);


	//////////////////////////// 상수버퍼
private:
	std::unordered_map<std::wstring, My_PTR<ConstBuffer>> m_ConstBufferMap;

public:
	My_PTR<ConstBuffer> Find_ConstBuffer(const wchar_t* _Name)
	{
		return Map_Find_PTR<My_PTR<ConstBuffer>>(m_ConstBufferMap, _Name);
	}

	template<typename T>
	void ConstBuffer_Create(const wchar_t* _Name, UINT _Rigister, D3D11_USAGE _Usage)
	{
		if (nullptr != Find_ConstBuffer(_Name))
		{
			EAssert(true);
			return;
		}

		ConstBuffer* NewBuffer = new ConstBuffer();

		if (false == NewBuffer->Create<T>(_Rigister, _Usage))
		{
			EAssert(true);
			return;
		}

		m_ConstBufferMap.insert(std::unordered_map<std::wstring, My_PTR<ConstBuffer>>::value_type(_Name, NewBuffer));
		return;
	}


public:
	Shader(EShader_Type _eShaderType);
	virtual ~Shader() = 0;
};

