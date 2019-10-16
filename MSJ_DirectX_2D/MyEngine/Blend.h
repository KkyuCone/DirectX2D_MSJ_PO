#pragma once
#include "Resource.h"
#include "DXHeader.h"

class Blend : public Resource
{
private:
	ID3D11BlendState* m_pBlendState;
	D3D11_BLEND_DESC m_Decs;
	VCOLOR m_CustomColor;

	UINT m_SampleMask;

public:
	DEFINITION_CREATOR(Blend, std::wstring)
		DEFINITION_CREATOR_FUCTION()
		DEFINITION_CREATOR_FUNCTION_PARAMETER()
	DEFINITION_CREATOR_END

public:
	bool Create();
	void Update();

public:
	Blend();
	~Blend();
};

