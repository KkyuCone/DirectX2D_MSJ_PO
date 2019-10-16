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


	// �ؽ�ó Ȯ���� �˻�
	if (true == IsExtension(L".DDs") || true == IsExtension(L".dds"))
	{
		if (S_OK != DirectX::LoadFromDDSFile(Const_Path(), DirectX::DDS_FLAGS_NONE, nullptr, m_Image))
		{
			// Const_Path() :���� ���
			//  DirectX::DDS_FLAGS_NONE �̰� ���� �Ӽ� ������ �� �ִµ� ���ҿ��� ����Ŷ� ���� ��ȯ���� ������ �� ����
			// nullptr -> ���� X

			EAssert(true);
			return false;
		}
	}
	else if (true == IsExtension(L".tga") || true == IsExtension(L".TGA"))
	{
		if (S_OK != DirectX::LoadFromTGAFile(Const_Path(), nullptr, m_Image))
		{
			// TGA�� ���ҿ��� ���� ���� �ƴ϶� �������ٰԾ���...�׷��� �Ű����� �ϳ��� ���� ������
			EAssert(true);
			return false;
		}
	}
	else
	{
		// ������ ���ϵ�.. PNG, JPGE, BMP ��..
		if (S_OK != DirectX::LoadFromWICFile(Const_Path(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image))
		{
			EAssert(true);
			return false;
		}
	}


	// 1. �Ҵ��ؾ� �ϹǷ� �翬�� ����̽�
	// 2. ������ ���� ���� �Ҵ��߾���� �̹���(���̺귯���� ����� �ش�.)
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

	// �ڿ����� ������ 4�����ε� ���̴� �ڿ���� ���� �ϳ��̴�.
	// �ڿ��� ���� : RenderTargetView, DepthStencilView, ShaderResourceView, UnorderedAccessView

	// ���̴� �ڿ��� (ShaderReosurce)
	// �Ʒ����� ���� �츮�� ���̺귯���� ����ϱ��ֱ⶧����.. 
	// ������ CreateShaderReosurceView()�� �Ű������� �ٸ�..
	if (S_OK != DirectX::CreateShaderResourceView(Device::Get_Device(),					// �ش� ����̽�
		m_Image.GetImages(), m_Image.GetImageCount(),									// �̹���, �̹���������	
		m_Image.GetMetadata(), &m_pShaderResourceView))									// �̹��� ��Ÿ������, ���̴��ڿ���
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
	// m_Image.GetPixelsSize(); // �����ϳ��� ����Ʈ ũ��
	// m_Image.GetPixels();
	// Ȯ���� ���̽��� ���� �׽�Ʈ �ϼ���.
	// ��ŸƮ�� 0
	// ������ �ش� �ȼ��� �� ��������(����)��..
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
	// _x�� y���� �����ǰ��̴� (���� ������ ��ġ)
	size_t Test = m_Image.GetPixelsSize();			// GetPixelsSize() : ���� * ���� * 4 (RGBA)
	uint8_t* PixelInfo = m_Image.GetPixels();

	size_t Width = m_Image.GetMetadata().width;				// �̹��� ���� ũ��
	size_t Height = m_Image.GetMetadata().height;			// �̹��� ���� ũ��

	size_t ImageCenterX = Width / 2;
	size_t ImageCenterY = Height / 2;


	// PIXELSIZE �ȼ�������- >�� Į���� RGBA
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


// �Ʒ� �Լ��� �׳� �������� ��������Ʈ��..�޾ƿ� ��������..�Ф�
uint8_t* Texture::ImageOriginGetPixel(unsigned int _x, unsigned int _y)
{
	RerturnValue[0] = 0;
	RerturnValue[1] = 0;
	RerturnValue[2] = 0;
	RerturnValue[3] = 0;


	// GetPixelsSize() : ���� * ���� * 4 (RGBA)
	uint8_t* PixelInfo = m_Image.GetPixels();

	size_t Width = m_Image.GetMetadata().width;				// �̹��� ���� ũ��

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
