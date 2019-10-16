#include "Sampler.h"
#include "Device.h"


Sampler::Sampler() : m_pState(nullptr)
{
}


Sampler::~Sampler()
{
	SAFE_RELEASE(m_pState);
}

bool Sampler::Create()
{
	// POINT 픽셀단위 보간을 하지 않는다.
	// LINEAR 이미지간 선형 보간을 한다. 색깔이 섞이게 된다.
	// ANISOTROPIC 이방성 보간을 사용한다. 색깔이 섞이게 된다.

	m_Desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
	//m_Desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_Desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	m_Desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	m_Desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

	//m_Desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
	//m_Desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
	//m_Desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;

	//  D3D11_TEXTURE_ADDRESS_BORDER로 하면 틈새 간격이 생김
	// 그 틈새 간격의 색상이 무엇인지.. -> _Desc.BorderColor
	memcpy_s(m_Desc.BorderColor, sizeof(VCOLOR), &VCOLOR::Red, sizeof(VCOLOR));

	m_Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	m_Desc.MaxAnisotropy = 0;


	// LOD : Level Of Detail
	// 멀리서 봤을떄 보간 값을 어느정도 해줄것인가..
	m_Desc.MinLOD = -FLT_MAX;			// 최소 디테일 값
	m_Desc.MaxLOD = FLT_MAX;			// 가장 디테일을 줄 픽셀의 보간값

	// 밉맵을 사용해 LOD를 할때..(설명 보충 필요함)
	m_Desc.MipLODBias = 1.0f;

	if (S_OK != Device::Get_Device()->CreateSamplerState(&m_Desc, &m_pState))
	{
		return false;
	}

	return true;
}

void Sampler::Update()
{
	Device::Get_Context()->PSSetSamplers(0, 1, &m_pState);
}
