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
	// 멀티 스프라이트를 만들건데
	// 0, 0으론 자를 수 없으니까 예외처리
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


	// Uv값을 통한 사이즈를 알아낼거다.
	Vector2 TextureUvSize;
	TextureUvSize.x = 1.0f / (float)_CountX;			// 0.0~1.0f사이 값이 나온다. 그렇게 자름
	TextureUvSize.y = 1.0f / (float)_CountY;

	// 멀티스프라이트 만들기 (자르기)
	for (size_t y = 0; y < _CountY; ++y)
	{
		for (size_t x = 0; x < _CountX; ++x)
		{
			Vector4 Uv;
			Uv.Size = TextureUvSize;
			Uv.Pos.x = TextureUvSize.x * (float)x;
			Uv.Pos.y = TextureUvSize.y * (float)y;
			m_UvMap.push_back(Uv);						// 넣어줌
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
	// 없으면 터지게 한다. 'ㅅ'
	if (nullptr == m_Texture || nullptr == m_Sampler)
	{
		EAssert(true);
		return;
	}

	m_Texture->Update();
	m_Sampler->Update();
}
