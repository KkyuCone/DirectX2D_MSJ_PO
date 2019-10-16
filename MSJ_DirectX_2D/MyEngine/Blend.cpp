#include "Blend.h"
#include "Device.h"



Blend::Blend() : m_pBlendState(nullptr), m_CustomColor(VCOLOR::Black), m_SampleMask(0xffffffff)
{
}


Blend::~Blend()
{
	SAFE_RELEASE(m_pBlendState);
}

//

bool Blend::Create()
{
	m_Decs.AlphaToCoverageEnable = false;			// Blend 옵션을 독립적으로 안쓰겠다 -> false
	m_Decs.IndependentBlendEnable = false;

	m_Decs.RenderTarget[0].BlendEnable = true;

	m_Decs.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	//D3D11_BLEND_OP_ADD

	m_Decs.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	m_Decs.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	m_Decs.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	m_Decs.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	m_Decs.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	m_Decs.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;


	if (S_OK != Device::Get_Device()->CreateBlendState(&m_Decs, &m_pBlendState))
	{
		EAssert(true);
		return false;
	}

	return true;
}

void Blend::Update()
{
	// 0b 00000000 11111111 11111111 11111111
	// 0x ff
	//    r        g        b        a

	Device::Get_Context()->OMSetBlendState(m_pBlendState, m_CustomColor.p_float, m_SampleMask);
}
