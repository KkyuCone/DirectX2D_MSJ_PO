#include "Texture.h"
#include "Device.h"
#include <wincodec.h>


Texture::Texture() : XPosValue(0), YPosValue(0), EndPosValue(0)
{
}


Texture::~Texture()
{
	SAFE_RELEASE(m_pTexture2D);
	SAFE_RELEASE(m_pShaderResourceView);
}


bool Texture::Load()
{
	if (nullptr == Device::Get_Device())
	{
		EAssert(true);
		return false;
	}


	// 텍스처 확장자 검색
	if (true == IsExtension(L".DDs") || true == IsExtension(L".dds"))
	{
		if (S_OK != DirectX::LoadFromDDSFile(Const_Path(), DirectX::DDS_FLAGS_NONE, nullptr, m_Image))
		{
			// Const_Path() :파일 경로
			//  DirectX::DDS_FLAGS_NONE 이거 말고 속성 여러개 더 있는데 마소에서 만든거라서 뭘로 변환할지 선택할 수 있음
			// nullptr -> 설정 X

			EAssert(true);
			return false;
		}
	}
	else if (true == IsExtension(L".tga") || true == IsExtension(L".TGA"))
	{
		if (S_OK != DirectX::LoadFromTGAFile(Const_Path(), nullptr, m_Image))
		{
			// TGA는 마소에서 만든 것이 아니라 설정해줄게없음...그래서 매개변수 하나가 없음 ㅇㅅㅇ
			EAssert(true);
			return false;
		}
	}
	else
	{
		// 나머지 파일들.. PNG, JPGE, BMP 등..
		if (S_OK != DirectX::LoadFromWICFile(Const_Path(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image))
		{
			EAssert(true);
			return false;
		}
	}


	// 1. 할당해야 하므로 당연히 디바이스
	// 2. 원래는 직접 만들어서 할당했어야할 이미지(라이브러리가 만들어 준다.)
	// 3. 
	//size_t Arr = m_Image.GetImageCount();
	// m_Image.GetMetadata()
	//struct TexMetadata
	//{
	//	size_t          width;
	//	size_t          height;     // Should be 1 for 1D textures
	//	size_t          depth;      // Should be 1 for 1D or 2D textures
	//	size_t          arraySize;  // For cubemap, this is a multiple of 6
	//	size_t          mipLevels;
	//	uint32_t        miscFlags;
	//	uint32_t        miscFlags2;
	//	DXGI_FORMAT     format;
	//	TEX_DIMENSION   dimension;

	// 자원뷰의 종류는 4가지인데 셰이더 자원뷰는 그중 하나이다.
	// 자원뷰 종류 : RenderTargetView, DepthStencilView, ShaderResourceView, UnorderedAccessView

	// 셰이더 자원뷰 (ShaderReosurce)
	// 아래꺼는 현재 우리가 라이브러리를 사용하기있기때문에.. 
	// 기존의 CreateShaderReosurceView()와 매개변수가 다름..
	if (S_OK != DirectX::CreateShaderResourceView(Device::Get_Device(),					// 해당 디바이스
		m_Image.GetImages(), m_Image.GetImageCount(),									// 이미지, 이미지사이즈	
		m_Image.GetMetadata(), &m_pShaderResourceView))									// 이미지 메타데이터, 셰이더자원뷰
	{
		return false;
	}

	EAssert(nullptr == m_pShaderResourceView);

	m_pShaderResourceView->GetResource((ID3D11Resource**)(&m_pTexture2D));

	EAssert(nullptr == m_pTexture2D);
	return true;
}


void Texture::Update()
{
	// m_Image.GetPixelsSize(); // 색깔하나의 바이트 크기
	// m_Image.GetPixels();
	// 확실한 케이스를 만들어서 테스트 하세요.
	// 스타트가 0
	// 내숙제 해당 픽셀을 값 가져오기(정보)를..
	Device::Get_Context()->PSSetShaderResources(0, 1, &m_pShaderResourceView);
}

DXGI_FORMAT Texture::Format()
{
	return m_Image.GetMetadata().format;
}



size_t Texture::Width()
{
	return m_Image.GetMetadata().width;
}

size_t Texture::Height()
{
	return m_Image.GetMetadata().height;
}

Vector4 Texture::ImageGetPixel(unsigned int _x, unsigned int _y)
{ 
	// _x랑 y값은 포지션값이다 (내가 가져올 위치)
	size_t Test = m_Image.GetPixelsSize();			// GetPixelsSize() : 가로 * 세로 * 4 (RGBA)
	uint8_t* PixelInfo = m_Image.GetPixels();

	size_t Width = m_Image.GetMetadata().width;				// 이미지 가로 크기
	size_t Height = m_Image.GetMetadata().height;			// 이미지 세로 크기

	size_t ImageCenterX = Width / 2;
	size_t ImageCenterY = Height / 2;


	// PIXELSIZE 픽셀사이즈- >얘 칼라같음 RGBA
	XPosValue = (unsigned int)((_x + ImageCenterX) * PIXELSIZE);
	YPosValue = (unsigned int)(Width * (ImageCenterY -(_y)) * PIXELSIZE);

	EndPosValue = XPosValue + YPosValue;

	uint8_t R = PixelInfo[0 + EndPosValue];
	uint8_t G = PixelInfo[1 + EndPosValue];
	uint8_t B = PixelInfo[2 + EndPosValue];
	uint8_t A = PixelInfo[3 + EndPosValue];

	Vector4 RerturnValue = Vector4{ (float)R, (float)G, (float)B, (float)A};

	XPosValue = 0;
	YPosValue = 0;
	
	return RerturnValue;
}


// 아래 함수는 그냥 오리지널 스프라이트값..받아옴 ㅇㅅㅇ하..ㅠㅠ
uint8_t* Texture::ImageOriginGetPixel(unsigned int _x, unsigned int _y)
{
	RerturnValue[0] = 0;
	RerturnValue[1] = 0;
	RerturnValue[2] = 0;
	RerturnValue[3] = 0;


	// GetPixelsSize() : 가로 * 세로 * 4 (RGBA)
	uint8_t* PixelInfo = m_Image.GetPixels();

	size_t Width = m_Image.GetMetadata().width;				// 이미지 가로 크기

	XPosValue = (unsigned int)((_x) * PIXELSIZE);
	YPosValue = (unsigned int)(Width * (_y) * PIXELSIZE);

	EndPosValue = XPosValue + YPosValue;

	uint8_t R = PixelInfo[0 + EndPosValue];
	uint8_t G = PixelInfo[1 + EndPosValue];
	uint8_t B = PixelInfo[2 + EndPosValue];
	uint8_t A = PixelInfo[3 + EndPosValue];


	uint8_t PixelInfo2[4] = { 0, };
	PixelInfo2[0] = A;
	PixelInfo2[1] = R;
	PixelInfo2[2] = G;
	PixelInfo2[3] = B;

	return &PixelInfo[0 + EndPosValue];

	//return &PixelInfo2[0];
}
