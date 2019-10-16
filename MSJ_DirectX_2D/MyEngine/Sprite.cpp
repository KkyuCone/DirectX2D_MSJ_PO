#include "Sprite.h"
#include "Texture.h"
#include "Sampler.h"



Sprite::Sprite()
{
}


Sprite::~Sprite()
{
}


bool Sprite::Create(const wchar_t* _TextureName, unsigned int _CountX, unsigned int _CountY, const wchar_t* _SamplerName)
{
	// ��Ƽ ��������Ʈ�� ����ǵ�
	// 0, 0���� �ڸ� �� �����ϱ� ����ó��
	if (_CountX == 0 || _CountY == 0)
	{
		return false;
	}

	if (nullptr == (m_Texture = ResourceManager<Texture>::Find(_TextureName)) ||
		nullptr == (m_Sampler = ResourceManager<Sampler>::Find(_SamplerName)))
	{
		int a = 0;
		EAssert(true);
		return false;
	}


	// Uv���� ���� ����� �˾Ƴ��Ŵ�.
	Vector2 TextureUvSize;
	TextureUvSize.x = 1.0f / (float)_CountX;			// 0.0~1.0f���� ���� ���´�. �׷��� �ڸ�
	TextureUvSize.y = 1.0f / (float)_CountY;

	// ��Ƽ��������Ʈ ����� (�ڸ���)
	for (size_t y = 0; y < _CountY; ++y)
	{
		for (size_t x = 0; x < _CountX; ++x)
		{
			Vector4 Uv;
			Uv.Size = TextureUvSize;
			Uv.Pos.x = TextureUvSize.x * (float)x;
			Uv.Pos.y = TextureUvSize.y * (float)y;
			m_UvMap.push_back(Uv);						// �־���
		}
	}

	return true;
}

size_t Sprite::UvPush(Vector4 _vUv)
{
	m_UvMap.push_back(_vUv);

	return m_UvMap.size() - 1;
}

Vector4 Sprite::UvCal(size_t _Index)
{
	return m_UvMap[_Index];
}

void Sprite::Update()
{
	// ������ ������ �Ѵ�. '��'
	if (nullptr == m_Texture || nullptr == m_Sampler)
	{
		EAssert(true);
		return;
	}

	m_Texture->Update();
	m_Sampler->Update();
}
