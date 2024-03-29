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


	// 努什坦 溌舌切 伊事
	if (true == IsExtension(L".DDs") || true == IsExtension(L".dds"))
	{
		if (S_OK != DirectX::LoadFromDDSFile(Const_Path(), DirectX::DDS_FLAGS_NONE, nullptr, m_Image))
		{
			// Const_Path() :督析 井稽
			//  DirectX::DDS_FLAGS_NONE 戚暗 源壱 紗失 食君鯵 希 赤澗汽 原社拭辞 幻窮暗虞辞 校稽 痕発拝走 識澱拝 呪 赤製
			// nullptr -> 竺舛 X

			EAssert(true);
			return false;
		}
	}
	else if (true == IsExtension(L".tga") || true == IsExtension(L".TGA"))
	{
		if (S_OK != DirectX::LoadFromTGAFile(Const_Path(), nullptr, m_Image))
		{
			// TGA澗 原社拭辞 幻窮 依戚 焼艦虞 竺舛背匝惟蒸製...益掘辞 古鯵痕呪 馬蟹亜 蒸製 しさし
			EAssert(true);
			return false;
		}
	}
	else
	{
		// 蟹袴走 督析級.. PNG, JPGE, BMP 去..
		if (S_OK != DirectX::LoadFromWICFile(Const_Path(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image))
		{
			EAssert(true);
			return false;
		}
	}


	// 1. 拝雁背醤 馬糠稽 雁尻備 巨郊戚什
	// 2. 据掘澗 送羨 幻級嬢辞 拝雁梅嬢醤拝 戚耕走(虞戚崎君軒亜 幻級嬢 層陥.)
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

	// 切据坂税 曽嫌澗 4亜走昔汽 捨戚希 切据坂澗 益掻 馬蟹戚陥.
	// 切据坂 曽嫌 : RenderTargetView, DepthStencilView, ShaderResourceView, UnorderedAccessView

	// 捨戚希 切据坂 (ShaderReosurce)
	// 焼掘襖澗 薄仙 酔軒亜 虞戚崎君軒研 紫遂馬奄赤奄凶庚拭.. 
	// 奄糎税 CreateShaderReosurceView()人 古鯵痕呪亜 陥硯..
	if (S_OK != DirectX::CreateShaderResourceView(Device::Get_Device(),					// 背雁 巨郊戚什
		m_Image.GetImages(), m_Image.GetImageCount(),									// 戚耕走, 戚耕走紫戚綜	
		m_Image.GetMetadata(), &m_pShaderResourceView))									// 戚耕走 五展汽戚斗, 捨戚希切据坂
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
	// m_Image.GetPixelsSize(); // 事薗馬蟹税 郊戚闘 滴奄
	// m_Image.GetPixels();
	// 溌叔廃 追戚什研 幻級嬢辞 砺什闘 馬室推.
	// 什展闘亜 0
	// 鎧寿薦 背雁 波漆聖 葵 亜閃神奄(舛左)研..
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
	// _x櫛 y葵精 匂走芝葵戚陥 (鎧亜 亜閃臣 是帖)
	size_t Test = m_Image.GetPixelsSize();			// GetPixelsSize() : 亜稽 * 室稽 * 4 (RGBA)
	uint8_t* PixelInfo = m_Image.GetPixels();

	size_t Width = m_Image.GetMetadata().width;				// 戚耕走 亜稽 滴奄
	size_t Height = m_Image.GetMetadata().height;			// 戚耕走 室稽 滴奄

	size_t ImageCenterX = Width / 2;
	size_t ImageCenterY = Height / 2;


	// PIXELSIZE 波漆紫戚綜- >剰 町虞旭製 RGBA
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


// 焼掘 敗呪澗 益撹 神軒走確 什覗虞戚闘葵..閤焼身 しさし馬..ばば
uint8_t* Texture::ImageOriginGetPixel(unsigned int _x, unsigned int _y)
{
	RerturnValue[0] = 0;
	RerturnValue[1] = 0;
	RerturnValue[2] = 0;
	RerturnValue[3] = 0;


	// GetPixelsSize() : 亜稽 * 室稽 * 4 (RGBA)
	uint8_t* PixelInfo = m_Image.GetPixels();

	size_t Width = m_Image.GetMetadata().width;				// 戚耕走 亜稽 滴奄

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
