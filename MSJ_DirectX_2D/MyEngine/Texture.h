#pragma once
#include "Resource.h"
#include "DXHeader.h"
#include <DirectXTex.h>

#define PIXELSIZE 4


class Texture : public Resource
{
private:
	unsigned int XPosValue;
	unsigned int YPosValue;
	unsigned int EndPosValue;

	uint8_t RerturnValue[4] { 0, };

private:
	DirectX::ScratchImage m_Image;
	ID3D11Texture2D* m_pTexture2D;

	// 쉐이더에 리소스로 넣어주려면 아래 쉐이더리소스뷰를 만들어야한다.
	ID3D11ShaderResourceView* m_pShaderResourceView;

	// 
public:
	Vector4 ImageGetPixel(unsigned int _x, unsigned int _y);
	uint8_t* ImageOriginGetPixel(unsigned int _x, unsigned int _y);

	DXGI_FORMAT Format();
	size_t Width();
	size_t Height();

	ID3D11Texture2D* GetTexture2D()
	{
		return m_pTexture2D;
	}


public:
	DEFINITION_LOADER(Texture, std::wstring)
		DEFINITION_PATH_LOADER_FUNCTION()
		DEFINITION_PATH_LOADER_FUNCTION_PARAMETER()
	DEFINITION_LOADER_END



public:
	bool Load();
	void Update();

public:
	Texture();
	~Texture();
};

