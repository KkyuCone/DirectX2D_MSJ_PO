#pragma once
#include <Windows.h>
#include <typeinfo>

#include "Device.h"
#include "Resource.h"
//#include"Shader.h"

class ConstBuffer : public Resource
{
private:
	UINT m_Register;
	const type_info* m_TypeInfo;
	D3D11_BUFFER_DESC m_ConstBufferDecs;
	ID3D11Buffer* m_pConstBuffer;
	D3D11_MAPPED_SUBRESOURCE m_SubResource;

public:
	template<typename T>
	bool ClassType_Check()
	{
		return m_TypeInfo == &typeid(T);
	}

	template<typename T>
	bool ConstBuffer::Create(UINT _Register, D3D11_USAGE _Usage)
	{
		m_Register = _Register;
		m_TypeInfo = &typeid(T);

		m_ConstBufferDecs.ByteWidth = sizeof(T);
		m_ConstBufferDecs.Usage = _Usage;
		m_ConstBufferDecs.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;

		if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == m_ConstBufferDecs.Usage)
		{
			m_ConstBufferDecs.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		if (S_OK != Device::Get_Device()->CreateBuffer(&m_ConstBufferDecs, nullptr, &m_pConstBuffer))
		{
			HRESULT a = Device::Get_Device()->CreateBuffer(&m_ConstBufferDecs, nullptr, &m_pConstBuffer);
			return false;
		}

		return true;
	}

	template<typename T>
	void ConstBuffer::Update(const T& _Value)
	{
		Device::Get_Context()->Map(m_pConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_SubResource);		// 지금부터 내가 열심히 쓸거니까 방해하지마!
		memcpy_s(m_SubResource.pData, sizeof(T), &_Value, sizeof(T));
		Device::Get_Context()->Unmap(m_pConstBuffer, 0);			// 할일 다끝났어~
	}

	void Vertex_Set();
	void Pixel_Set();
	//void Shader_Set(Shader::EShader_Type& _Type);
public:
	ConstBuffer() {}
	~ConstBuffer() { SAFE_RELEASE(m_pConstBuffer); }
};

