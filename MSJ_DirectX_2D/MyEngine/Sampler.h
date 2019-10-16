#pragma once
#include "Resource.h"
#include "DXHeader.h"

class Sampler : public Resource
{
private:
	D3D11_SAMPLER_DESC m_Desc;
	ID3D11SamplerState* m_pState;

public:
	ID3D11SamplerState* GetPState()
	{
		return m_pState;
	}

public:
	DEFINITION_CREATOR(Sampler, std::wstring)
		DEFINITION_CREATOR_FUCTION()
		DEFINITION_CREATOR_FUNCTION_PARAMETER()
	DEFINITION_CREATOR_END

public:
	bool Create();
	void Update();

public:
	Sampler();
	~Sampler();
};

