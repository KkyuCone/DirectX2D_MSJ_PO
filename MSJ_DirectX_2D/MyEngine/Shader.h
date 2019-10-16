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

	// ID3DBlob�� ������ ���۷� ��� ����.
	// �޽� ����ȭ �� �ε� �۾��� �����ϴ� ���� ����, ������ ���� ������ �����ϴµ� ��� ����.
	// �Ǵ� �̷� ��ü�� ����, ������Ʈ��, �ȼ� ���̴��� �������ϴ� API���� ��ü �ڵ� �� ���� �޽����� ��ȯ�ϴµ� ����.

	ID3DBlob* m_pBlob;					// ���̴� �ڵ带 �������ؼ� ����̽����� �Ѱ����� ����̽��� �ִ� ������
	ID3DBlob* m_pErrorBlob;				// ���̴� �ڵ� �������� ���������� ����̽��� ������ ������ �Ѱ��� ������

	UINT m_VersionHLSLFirst;			// HLSL������ ���. (ù��° �ڸ���),    HLSL -> 1~4�� DX9���� ���, 11���ʹ� 5�� ����
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


	//////////////////////////// �������
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

