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
	// POINT �ȼ����� ������ ���� �ʴ´�.
	// LINEAR �̹����� ���� ������ �Ѵ�. ������ ���̰� �ȴ�.
	// ANISOTROPIC �̹漺 ������ ����Ѵ�. ������ ���̰� �ȴ�.

	m_Desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
	//m_Desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_Desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	m_Desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	m_Desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

	//m_Desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
	//m_Desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
	//m_Desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;

	//  D3D11_TEXTURE_ADDRESS_BORDER�� �ϸ� ƴ�� ������ ����
	// �� ƴ�� ������ ������ ��������.. -> _Desc.BorderColor
	memcpy_s(m_Desc.BorderColor, sizeof(VCOLOR), &VCOLOR::Red, sizeof(VCOLOR));

	m_Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	m_Desc.MaxAnisotropy = 0;


	// LOD : Level Of Detail
	// �ָ��� ������ ���� ���� ������� ���ٰ��ΰ�..
	m_Desc.MinLOD = -FLT_MAX;			// �ּ� ������ ��
	m_Desc.MaxLOD = FLT_MAX;			// ���� �������� �� �ȼ��� ������

	// �Ӹ��� ����� LOD�� �Ҷ�..(���� ���� �ʿ���)
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
